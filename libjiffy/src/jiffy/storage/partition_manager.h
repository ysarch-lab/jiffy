#ifndef JIFFY_PARTITION_MANAGER_H
#define JIFFY_PARTITION_MANAGER_H

#include "jiffy/utils/property_map.h"
#include "jiffy/storage/chain_module.h"

namespace jiffy {
namespace storage {

class partition_builder {
 public:
  /**
   * @brief Virtual destructor.
   */
  virtual ~partition_builder() = default;

  /**
   * @brief Builds a partition.
   * @param name Partition name
   * @param metadata Partition metadata
   * @param conf Partition configuration
   * @return An instance of the partition.
   */
  virtual std::shared_ptr<chain_module> build(const std::string &name,
                                              const std::string &metadata,
                                              const utils::property_map &conf) = 0;
};

class partition_manager {
 public:
  /* Maps partition type to its implementation builder */
  typedef std::unordered_map<std::string, std::shared_ptr<partition_builder>> partition_map;

  /**
   * @brief Register a partition implementation
   * @param type Type of partition.
   * @param impl The implementation builder.
   */
  static void register_impl(const std::string &type,
                            std::shared_ptr<partition_builder> impl);

  /**
   * @brief Build a partition of specified type.
   * @param type Type of partition.
   * @param conf Partition configuration.
   * @return An instance of the partition.
   */
  static std::shared_ptr<chain_module> build_partition(const std::string &type,
                                                       const std::string &name,
                                                       const std::string &metadata,
                                                       const utils::property_map &conf);

 private:
  /**
   * @brief Get the mapping between partition types and their implementation builders.
   * @return The mapping between partition types and their implementation builders.
   */
  static partition_map &implementations();
};

#define REGISTER_IMPLEMENTATION(type, impl)                                       \
class impl##_builder : public partition_builder {                                 \
 public:                                                                          \
  static_assert(std::is_base_of<chain_module, impl>::value,                       \
      "Partition implementation must be a subclass of chain_module");             \
                                                                                  \
  virtual ~impl##_builder() = default;                                            \
                                                                                  \
  std::shared_ptr<chain_module> build(const std::string& name,                    \
                                      const std::string& metadata,                \
                                      const utils::property_map& conf) override { \
    return std::make_shared<impl>(name, metadata, conf);                          \
  }                                                                               \
};                                                                                \
                                                                                  \
class impl##_registeration_stub {                                                 \
  public:                                                                         \
   impl##_registeration_stub() {                                                  \
     partition_manager::register_impl(type, std::make_shared<impl##_builder>());  \
   }                                                                              \
};                                                                                \
                                                                                  \
static impl##_registeration_stub impl##_registeration_stub_singleton

}
}

#endif //JIFFY_PARTITION_MANAGER_H