#ifndef JIFFY_BLOCK_REQUEST_HANDLER_H
#define JIFFY_BLOCK_REQUEST_HANDLER_H

#include <atomic>

#include "block_request_service.h"
#include "block_response_client.h"
#include "../chain_module.h"

namespace jiffy {
namespace storage {
/* Block request handler class
 * Inherited from block_request_serviceIf */
class block_request_handler : public block_request_serviceIf {
 public:

  /**
   * @brief Constructor
   * @param client Block response client
   * @param client_id_gen Client identifier generator
   * @param blocks Data blocks
   */

  explicit block_request_handler(std::shared_ptr<block_response_client> client,
                                 std::atomic<int64_t> &client_id_gen,
                                 std::vector<std::shared_ptr<chain_module>> &blocks);

  /**
   * @brief Fetch client identifier and add one to the atomic pointer
   * @return Client identifier
   */

  int64_t get_client_id() override;

  /**
   * @brief Register the client with the block
   * Save the block identifier and client identifier to the request handler and
   * add client to the block response client map
   * @param block_id Block identifier
   * @param client_id Client identifier
   */

  void register_client_id(int32_t block_id, int64_t client_id) override;

  /**
   * @brief Request a command, starting from either the head or tail of the chain
   * @param seq Sequence identifier
   * @param block_id Block identifier
   * @param cmd_id Command identifier
   * @param arguments Command arguments
   */

  void command_request(const sequence_id &seq,
                       int32_t block_id,
                       int32_t cmd_id,
                       const std::vector<std::string> &arguments) override;

  /**
   * @brief Fetch the registered block identifier
   * @return Registered block identifier
   */

  int32_t registered_block_id() const;

  /**
   * @brief Fetch the registered client identifier
   * @return Registered client identifier
   */

  int64_t registered_client_id() const;

 private:
  /* Block response client */
  std::shared_ptr<block_response_client> client_;
  /* Registered partition identifier */
  int32_t registered_block_id_;
  /* Registered client identifier */
  int64_t registered_client_id_;
  /* Client identifier generator */
  std::atomic<int64_t> &client_id_gen_;
  /* Data blocks */
  std::vector<std::shared_ptr<chain_module>> &blocks_;
};

}
}

#endif //JIFFY_BLOCK_REQUEST_HANDLER_H
