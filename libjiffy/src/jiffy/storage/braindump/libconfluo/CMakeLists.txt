set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-parameter")
if (NOT APPLE AND UNIX)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed -ldl")
endif ()

set(CONFLUO_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/confluo)
set(UTILS_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../libutils/utils)

include_directories(${GTEST_INCLUDE_DIR} ${CONFLUO_INCLUDE} ${UTILS_INCLUDE} ${Boost_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR})

include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/libconfluo
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/conf
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/lazy
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/utils
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/planner
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/threads)

add_library(confluo STATIC
        ${CMAKE_CURRENT_LIST_DIR}/confluo/read_tail.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/atomic_multilog.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/type_properties.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/numeric.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/mutable_value.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/raw_data.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/key_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/immutable_value.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/string_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/data_type.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/serde_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/byte_string.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/arithmetic_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/primitive_types.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/relational_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/types/type_manager.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/filter_log.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/trigger.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/atomic_multilog_metadata.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/planner/query_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/planner/query_plan.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/planner/query_planner.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/aggregate/aggregate.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/aggregate/aggregate_manager.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/aggregate/aggregate_info.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/aggregate/aggregate_ops.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/exceptions.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/parser/expression_compiler.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/parser/trigger_parser.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/parser/aggregate_parser.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/parser/expression_parser.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/parser/schema_parser.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/encoded_ptr.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/swappable_encoded_ptr.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/memory_stat.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/swappable_ptr.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/ptr_metadata.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/reference_counts.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/allocator.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/storage.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/ptr_aux_block.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/storage_allocator.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/storage/storage_utils.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/aggregated_reflog.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/archival_actions.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/atomic_multilog_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/load_utils.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/archival_mode.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/io/incremental_file_writer.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/io/incremental_file_offset.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/io/incremental_file_stream.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/io/incremental_file_reader.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/index_log_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/filter_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/index_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/monolog_linear_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/filter_log_archiver.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/archival_metadata.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/archival/archival_utils.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/flatten.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/record_offset_range.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/cursor
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/cursor/offset_cursors.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/cursor/record_cursors.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/cursor/batched_cursor.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/cursor/alert_cursor.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/bitmap
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/bitmap/bitmap_array.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/bitmap/delta_encoded_array.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/bitmap/bitmap.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/unique_byte_array.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/string_map.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/reflog.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/data_log.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/lazy
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/lazy/stream.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog_exp2.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog_iterator.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog_exp2_linear.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog_linear.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/monolog/monolog_linear_bucket.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/radix_tree.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/count_sketch.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/frequency_functions.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/hash_manager.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/priority_queue.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/sketch_utils.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/container/sketch/universal_sketch.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/field.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/record.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/record_batch.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/schema.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/column.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/index_state.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/column_snapshot.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/schema/schema_snapshot.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/threads/thread_manager.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/threads/task_pool.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/threads/periodic_task.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/index_log.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/alert_index.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression/confluo_encoder.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression/delta_encoder.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression/lz4_decoder.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression/lz4_encoder.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/compression/delta_decoder.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/confluo_store.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/trigger_log.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/conf
        ${CMAKE_CURRENT_LIST_DIR}/confluo/conf/configuration_params.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/conf/defaults.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/alert.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/filter.h
        ${CMAKE_CURRENT_LIST_DIR}/confluo/univ_sketch_log.h
        ${CMAKE_CURRENT_LIST_DIR}/src/confluo_store.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/atomic_multilog.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/filter.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/read_tail.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/trigger.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/atomic_multilog_metadata.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/alert.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/alert_index.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/aggregated_reflog.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/aggregate/aggregate.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/aggregate/aggregate_info.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/aggregate/aggregate_manager.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/aggregate/aggregate_ops.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/compression/confluo_encoder.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/data_log.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/reflog.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/cursor/alert_cursor.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/cursor/offset_cursors.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/cursor/record_cursors.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/sketch/universal_sketch.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/container/sketch/hash_manager.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/parser/aggregate_parser.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/parser/expression_compiler.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/parser/expression_parser.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/parser/schema_parser.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/parser/trigger_parser.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/planner/query_ops.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/planner/query_plan.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/planner/query_planner.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/column.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/field.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/index_state.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/record.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/record_batch.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/schema.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/schema/schema_snapshot.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/io/incremental_file_offset.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/io/incremental_file_stream.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/io/incremental_file_reader.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/io/incremental_file_writer.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/archival_actions.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/archival_metadata.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/archival_utils.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/atomic_multilog_archiver.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/filter_archiver.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/filter_log_archiver.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/index_archiver.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/index_log_archiver.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/archival/load_utils.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/reference_counts.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/ptr_aux_block.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/ptr_metadata.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/memory_stat.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/storage.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/storage/storage_allocator.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/threads/periodic_task.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/threads/task_pool.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/threads/thread_manager.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/byte_string.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/data_type.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/immutable_value.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/primitive_types.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/mutable_value.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/raw_data.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/numeric.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/type_properties.cc
        ${CMAKE_CURRENT_LIST_DIR}/src/types/type_manager.cc)

### add lz4
set(EXTERNAL_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC ${CMAKE_CXX_FLAGS_${UPPERCASE_BUILD_TYPE}}")
set(EXTERNAL_C_FLAGS "${CMAKE_C_FLAGS} -fPIC ${CMAKE_C_FLAGS_${UPPERCASE_BUILD_TYPE}}")

include (ExternalProject)

ExternalProject_Add(lz4
        URL https://github.com/lz4/lz4/archive/v1.8.2.tar.gz
        CONFIGURE_COMMAND ""
        BUILD_IN_SOURCE 1
        BUILD_COMMAND make -C lib lib MOREFLAGS=-fPIC
        INSTALL_COMMAND ""
)
ExternalProject_Get_Property(lz4 SOURCE_DIR BINARY_DIR)
set(lz4_INCLUDE_DIR "${SOURCE_DIR}/lib")
set(lz4_STATIC_LIB "${BINARY_DIR}/lib/${CMAKE_STATIC_LIBRARY_PREFIX}lz4${CMAKE_STATIC_LIBRARY_SUFFIX}")
include_directories(SYSTEM ${lz4_INCLUDE_DIR})
###

target_link_libraries(confluo confluoutils ${CMAKE_THREAD_LIBS_INIT} ${lz4_STATIC_LIB})
add_dependencies(confluo lz4)

find_package(Boost 1.45.0 COMPONENTS program_options)

# install
install(TARGETS confluo
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(DIRECTORY confluo/
        DESTINATION include
        FILES_MATCHING PATTERN "*")
