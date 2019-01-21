#include <catch.hpp>
#include <thrift/transport/TTransportException.h>
#include <thread>
#include "jiffy/storage/manager/storage_management_server.h"
#include "jiffy/storage/manager/storage_management_client.h"
#include "jiffy/storage/manager/storage_manager.h"
#include "jiffy/storage/hashtable/hash_table_partition.h"
#include "test_utils.h"
#include "jiffy/storage/service/block_server.h"
#include "jiffy/storage/hashtable/hash_slot.h"
#include "jiffy/directory/fs/directory_tree.h"
#include "jiffy/directory/fs/directory_server.h"
#include "jiffy/client/jiffy_client.h"
#include "jiffy/storage/notification/notification_server.h"
#include "jiffy/directory/lease/lease_server.h"
#include "jiffy/storage/service/chain_server.h"
#include "jiffy/directory/fs/sync_worker.h"
#include "jiffy/directory/lease/lease_expiry_worker.h"

using namespace jiffy::client;
using namespace jiffy::storage;
using namespace jiffy::directory;
using namespace jiffy::utils;

#define NUM_BLOCKS 3
#define HOST "127.0.0.1"
#define DIRECTORY_SERVICE_PORT 9090
#define DIRECTORY_LEASE_PORT 9091
#define STORAGE_SERVICE_PORT 9092
#define STORAGE_MANAGEMENT_PORT 9093
#define STORAGE_NOTIFICATION_PORT 9094
#define STORAGE_CHAIN_PORT 9095
#define LEASE_PERIOD_MS 100
#define LEASE_PERIOD_US (LEASE_PERIOD_MS * 1000)

void test_kv_ops(std::shared_ptr<hash_table_client> kv) {
  for (size_t i = 0; i < 1000; i++) {
    REQUIRE(kv->put(std::to_string(i), std::to_string(i)) == "!ok");
  }

  for (size_t i = 0; i < 1000; i++) {
    REQUIRE(kv->get(std::to_string(i)) == std::to_string(i));
  }

  for (size_t i = 1000; i < 2000; i++) {
    REQUIRE(kv->get(std::to_string(i)) == "!key_not_found");
  }

  for (size_t i = 0; i < 1000; i++) {
    REQUIRE(kv->update(std::to_string(i), std::to_string(i + 1000)) == std::to_string(i));
  }

  for (size_t i = 1000; i < 2000; i++) {
    REQUIRE(kv->update(std::to_string(i), std::to_string(i + 1000)) == "!key_not_found");
  }

  for (size_t i = 0; i < 1000; i++) {
    REQUIRE(kv->remove(std::to_string(i)) == std::to_string(i + 1000));
  }

  for (size_t i = 1000; i < 2000; i++) {
    REQUIRE(kv->remove(std::to_string(i)) == "!key_not_found");
  }

  for (size_t i = 0; i < 1000; i++) {
    REQUIRE(kv->get(std::to_string(i)) == "!key_not_found");
  }
}

TEST_CASE("jiffy_client_lease_worker_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    REQUIRE_NOTHROW(client.create_hash_table("/a/file.txt", "local://tmp"));
    REQUIRE(client.fs()->exists("/a/file.txt"));
    usleep(LEASE_PERIOD_US);
    REQUIRE(client.fs()->exists("/a/file.txt"));
    usleep(LEASE_PERIOD_US);
    REQUIRE(client.fs()->exists("/a/file.txt"));
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_create_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    auto kv = client.create_hash_table("/a/file.txt", "/tmp");
    REQUIRE(client.fs()->exists("/a/file.txt"));
    test_kv_ops(kv);
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_open_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    client.create_hash_table("/a/file.txt", "/tmp");
    REQUIRE(client.fs()->exists("/a/file.txt"));
    auto kv = client.open("/a/file.txt");
    test_kv_ops(kv);
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_flush_remove_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    client.create_hash_table("/file.txt", "local://tmp");
    REQUIRE(client.lease_worker().has_path("/file.txt"));
    REQUIRE_NOTHROW(client.sync("/file.txt", "local://tmp"));
    REQUIRE(client.lease_worker().has_path("/file.txt"));
    REQUIRE_NOTHROW(client.remove("/file.txt"));
    REQUIRE_FALSE(client.lease_worker().has_path("/file.txt"));
    REQUIRE_FALSE(client.fs()->exists("/file.txt"));
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_close_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    client.create_hash_table("/file.txt", "local://tmp");
    client.create_hash_table("/file1.txt", "local://tmp", 1, 1, data_status::PINNED);
    client.create_hash_table("/file2.txt", "local://tmp", 1, 1, data_status::MAPPED);
    REQUIRE(client.lease_worker().has_path("/file.txt"));
    REQUIRE(client.lease_worker().has_path("/file1.txt"));
    REQUIRE(client.lease_worker().has_path("/file2.txt"));
    REQUIRE_NOTHROW(client.close("/file.txt"));
    REQUIRE_NOTHROW(client.close("/file1.txt"));
    REQUIRE_NOTHROW(client.close("/file2.txt"));
    REQUIRE_FALSE(client.lease_worker().has_path("/file.txt"));
    REQUIRE_FALSE(client.lease_worker().has_path("/file1.txt"));
    REQUIRE_FALSE(client.lease_worker().has_path("/file2.txt"));
    usleep(LEASE_PERIOD_US);
    REQUIRE(client.fs()->exists("/file.txt"));
    REQUIRE(client.fs()->exists("/file1.txt"));
    REQUIRE(client.fs()->exists("/file2.txt"));
    usleep(LEASE_PERIOD_US * 2); // Ensure lease expiry
    REQUIRE_FALSE(client.fs()->exists("/file.txt"));
    REQUIRE(client.fs()->exists("/file1.txt"));
    REQUIRE(client.fs()->exists("/file2.txt"));
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_notification_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    std::string op1 = "put", op2 = "remove";
    std::string key = "key1", value = "value1";

    client.fs()->create("/a/file.txt", "hashtable", "/tmp", 1, 1, 0, 0, {"0_65536"}, {"regular"});
    auto n1 = client.listen("/a/file.txt");
    auto n2 = client.listen("/a/file.txt");
    auto n3 = client.listen("/a/file.txt");

    n1->subscribe({op1});
    n2->subscribe({op1, op2});
    n3->subscribe({op2});

    auto kv = client.open("/a/file.txt");
    kv->put(key, value);
    kv->remove(key);

    REQUIRE(n1->get_notification() == std::make_pair(op1, key));
    REQUIRE(n2->get_notification() == std::make_pair(op1, key));
    REQUIRE(n2->get_notification() == std::make_pair(op2, key));
    REQUIRE(n3->get_notification() == std::make_pair(op2, key));

    REQUIRE_THROWS_AS(n1->get_notification(100), std::out_of_range);
    REQUIRE_THROWS_AS(n2->get_notification(100), std::out_of_range);
    REQUIRE_THROWS_AS(n3->get_notification(100), std::out_of_range);

    n1->unsubscribe({op1});
    n2->unsubscribe({op2});

    kv->put(key, value);
    kv->remove(key);

    REQUIRE(n2->get_notification() == std::make_pair(op1, key));
    REQUIRE(n3->get_notification() == std::make_pair(op2, key));

    REQUIRE_THROWS_AS(n1->get_notification(100), std::out_of_range);
    REQUIRE_THROWS_AS(n2->get_notification(100), std::out_of_range);
    REQUIRE_THROWS_AS(n3->get_notification(100), std::out_of_range);
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}

TEST_CASE("jiffy_client_chain_replication_test", "[put][get][update][remove]") {
  auto alloc = std::make_shared<sequential_block_allocator>();
  auto block_names = test_utils::init_block_names(NUM_BLOCKS,
                                                  STORAGE_SERVICE_PORT,
                                                  STORAGE_MANAGEMENT_PORT,
                                                  STORAGE_NOTIFICATION_PORT,
                                                  STORAGE_CHAIN_PORT);
  alloc->add_blocks(block_names);
  auto blocks = test_utils::init_kv_blocks(block_names);
  auto sm = std::make_shared<storage_manager>();
  auto tree = std::make_shared<directory_tree>(alloc, sm);

  auto storage_server = block_server::create(blocks, HOST, STORAGE_SERVICE_PORT);
  std::thread storage_serve_thread([&storage_server] { storage_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_SERVICE_PORT);

  auto mgmt_server = storage_management_server::create(blocks, HOST, STORAGE_MANAGEMENT_PORT);
  std::thread mgmt_serve_thread([&mgmt_server] { mgmt_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_MANAGEMENT_PORT);

  auto notif_server = notification_server::create(blocks, HOST, STORAGE_NOTIFICATION_PORT);
  std::thread notif_serve_thread([&notif_server] { notif_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_NOTIFICATION_PORT);

  auto chain_server = chain_server::create(blocks, HOST, STORAGE_CHAIN_PORT);
  std::thread chain_serve_thread([&chain_server] { chain_server->serve(); });
  test_utils::wait_till_server_ready(HOST, STORAGE_CHAIN_PORT);

  auto dir_server = directory_server::create(tree, HOST, DIRECTORY_SERVICE_PORT);
  std::thread dir_serve_thread([&dir_server] { dir_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_SERVICE_PORT);

  auto lease_server = lease_server::create(tree, LEASE_PERIOD_MS, HOST, DIRECTORY_LEASE_PORT);
  std::thread lease_serve_thread([&lease_server] { lease_server->serve(); });
  test_utils::wait_till_server_ready(HOST, DIRECTORY_LEASE_PORT);

  lease_expiry_worker lmgr(tree, LEASE_PERIOD_MS, LEASE_PERIOD_MS);
  lmgr.start();

  sync_worker syncer(tree, 1000);
  syncer.start();

  {
    jiffy_client client(HOST, DIRECTORY_SERVICE_PORT, DIRECTORY_LEASE_PORT);
    auto kv = client.create_hash_table("/a/file.txt", "/tmp", 1, NUM_BLOCKS);
    REQUIRE(kv->status().chain_length() == 3);
    test_kv_ops(kv);
  }

  storage_server->stop();
  if (storage_serve_thread.joinable()) {
    storage_serve_thread.join();
  }

  mgmt_server->stop();
  if (mgmt_serve_thread.joinable()) {
    mgmt_serve_thread.join();
  }

  notif_server->stop();
  if (notif_serve_thread.joinable()) {
    notif_serve_thread.join();
  }

  chain_server->stop();
  if (chain_serve_thread.joinable()) {
    chain_serve_thread.join();
  }

  dir_server->stop();
  if (dir_serve_thread.joinable()) {
    dir_serve_thread.join();
  }

  lease_server->stop();
  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
}
