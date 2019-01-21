#ifndef JIFFY_KV_SERVICE_H
#define JIFFY_KV_SERVICE_H

#include <cstddef>
#include <string>

#include "../persistent/persistent_service.h"
#include "partition.h"

namespace jiffy {
namespace storage {

/* Storage management operation virtual class */
class storage_management_ops {
 public:
  virtual ~storage_management_ops() = default;

  virtual void setup_block(const std::string &block_name,
                             const std::string &path,
                             const std::string &partition_type,
                             const std::string &partition_name,
                             const std::string &partition_metadata,
                             const std::vector<std::string> &chain,
                             int32_t role,
                             const std::string &next_block_name) = 0;

  virtual std::string path(const std::string &block_name) = 0;

  virtual void load(const std::string &block_name, const std::string &backing_path) = 0;

  virtual void sync(const std::string &block_name, const std::string &backing_path) = 0;

  virtual void dump(const std::string &block_name, const std::string &backing_path) = 0;

  virtual void reset(const std::string &block_name) = 0;

  virtual std::size_t storage_capacity(const std::string &block_name) = 0;

  virtual std::size_t storage_size(const std::string &block_name) = 0;

  virtual void resend_pending(const std::string &block_name) = 0;

  virtual void forward_all(const std::string &block_name) = 0;

};

}
}

#endif //JIFFY_KV_SERVICE_H
