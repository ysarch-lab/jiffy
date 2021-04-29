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
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/read_tail.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/atomic_multilog.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/type_properties.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/numeric.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/mutable_value.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/raw_data.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/key_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/immutable_value.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/string_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/data_type.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/serde_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/byte_string.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/arithmetic_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/primitive_types.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/relational_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/types/type_manager.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/filter_log.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/trigger.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/atomic_multilog_metadata.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/planner/query_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/planner/query_plan.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/planner/query_planner.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate/aggregate.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate/aggregate_manager.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate/aggregate_info.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregate/aggregate_ops.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/exceptions.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser/expression_compiler.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser/trigger_parser.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser/aggregate_parser.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser/expression_parser.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/parser/schema_parser.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/encoded_ptr.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/swappable_encoded_ptr.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/memory_stat.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/swappable_ptr.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/ptr_metadata.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/reference_counts.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/allocator.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/storage.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/ptr_aux_block.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/storage_allocator.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/storage/storage_utils.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/aggregated_reflog.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/archival_actions.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/atomic_multilog_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/load_utils.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/archival_mode.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io/incremental_file_writer.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io/incremental_file_offset.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io/incremental_file_stream.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/io/incremental_file_reader.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/index_log_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/filter_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/index_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/monolog_linear_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/filter_log_archiver.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/archival_metadata.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/archival/archival_utils.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/flatten.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/record_offset_range.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor/offset_cursors.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor/record_cursors.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor/batched_cursor.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/cursor/alert_cursor.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap/bitmap_array.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap/delta_encoded_array.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/bitmap/bitmap.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/unique_byte_array.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/string_map.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/reflog.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/data_log.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/lazy
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/lazy/stream.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog_exp2.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog_iterator.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog_exp2_linear.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog_linear.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/monolog/monolog_linear_bucket.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/radix_tree.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/count_sketch.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/frequency_functions.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/hash_manager.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/priority_queue.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/sketch_utils.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/container/sketch/universal_sketch.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/field.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/record.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/record_batch.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/schema.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/column.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/index_state.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/column_snapshot.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/schema/schema_snapshot.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/threads/thread_manager.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/threads/task_pool.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/threads/periodic_task.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/index_log.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/alert_index.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression/confluo_encoder.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression/delta_encoder.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression/lz4_decoder.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression/lz4_encoder.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/compression/delta_decoder.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/confluo_store.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/trigger_log.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/conf
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/conf/configuration_params.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/conf/defaults.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/alert.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/filter.h
        ${CMAKE_SOURCE_DIR}/libconfluo/confluo/univ_sketch_log.h
        ${CMAKE_SOURCE_DIR}/libconfluo/src/confluo_store.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/atomic_multilog.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/filter.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/read_tail.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/trigger.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/atomic_multilog_metadata.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/alert.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/alert_index.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/aggregated_reflog.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/aggregate/aggregate.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/aggregate/aggregate_info.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/aggregate/aggregate_manager.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/aggregate/aggregate_ops.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/compression/confluo_encoder.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/data_log.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/reflog.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/cursor/alert_cursor.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/cursor/offset_cursors.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/cursor/record_cursors.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/sketch/universal_sketch.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/container/sketch/hash_manager.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/parser/aggregate_parser.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/parser/expression_compiler.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/parser/expression_parser.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/parser/schema_parser.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/parser/trigger_parser.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/planner/query_ops.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/planner/query_plan.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/planner/query_planner.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/column.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/field.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/index_state.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/record.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/record_batch.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/schema.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/schema/schema_snapshot.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/io/incremental_file_offset.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/io/incremental_file_stream.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/io/incremental_file_reader.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/io/incremental_file_writer.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/archival_actions.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/archival_metadata.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/archival_utils.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/atomic_multilog_archiver.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/filter_archiver.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/filter_log_archiver.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/index_archiver.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/index_log_archiver.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/archival/load_utils.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/reference_counts.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/ptr_aux_block.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/ptr_metadata.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/memory_stat.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/storage.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/storage/storage_allocator.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/threads/periodic_task.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/threads/task_pool.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/threads/thread_manager.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/byte_string.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/data_type.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/immutable_value.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/primitive_types.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/mutable_value.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/raw_data.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/numeric.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/type_properties.cc
        ${CMAKE_SOURCE_DIR}/libconfluo/src/types/type_manager.cc)

target_link_libraries(confluo confluoutils ${CMAKE_THREAD_LIBS_INIT} ${lz4_STATIC_LIB})
add_dependencies(confluo lz4)

find_package(Boost 1.45.0 COMPONENTS program_options)

if (BUILD_TESTS)
  # Build test
  add_executable(ctest
          test/test_main.cc
          test/filter_test.h
          test/types/data_types_test.h
          test/types/mutable_value_test.h
          test/types/immutable_value_test.h
          test/types/ip_address.h
          test/types/size_type.h
          test/types/type_manager_test.h
          test/types/byte_string_test.h
          test/atomic_multilog_metadata_test.h
          test/atomic_multilog_test.h
          test/test_utils.h
          test/aggregate/aggregate_test.h
          test/parser/aggregate_parser_test.h
          test/parser/expression_compiler_test.h
          test/parser/schema_parser_test.h
          test/parser/expression_parser_test.h
          test/parser/trigger_parser_test.h
          test/storage/ptr_test.h
          test/storage/storage_allocator_test.h
          test/storage/memory_stat_test.h
          test/archival/monolog_linear_load_test.h
          test/archival/index_load_test.h
          test/archival/filter_load_test.h
          test/archival/monolog_linear_archival_test.h
          test/archival/index_archival_test.h
          test/archival/filter_archival_test.h
          test/container/cursor/batched_cursor_test.h
          test/container/bitmap/bitmap_test.h
          test/container/bitmap/bitmap_array_test.h
          test/container/sketch/count_sketch_test.h
          test/container/sketch/sketch_test_utils.h
          test/container/sketch/universal_sketch_test.h
          test/container/sketch/priority_queue_test.h
          test/container/bitmap/delta_encoded_array_test.h
          test/container/stream_test.h
          test/container/string_map_test.h
          test/container/radix_tree_test.h
          test/container/flatten_test.h
          test/container/monolog/monolog_test.h
          test/schema/record_batch_test.h
          test/schema/column_test.h
          test/schema/schema_test.h
          test/schema/index_state_test.h
          test/threads/task_test.h
          test/threads/periodic_task_test.h
          test/threads/thread_manager_test.h
          test/compression/lz4_encode_test.h
          test/compression/delta_encode_test.h
          test/aggregated_reflog_test.h
          test/confluo_store_test.h)
  target_link_libraries(ctest confluo gtest gtest_main)
  add_dependencies(ctest googletest)

  file(GLOB_RECURSE test_sources test/*.cc)
  target_link_libraries(ctest ${TEST_LINK_LIBS} ${CMAKE_THREAD_LIBS_INIT})
  add_dependencies(ctest googletest)

  # Register test
  enable_testing()
  add_test(ConfluoTest ctest)
endif ()

# install
install(TARGETS confluo
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib)
install(DIRECTORY confluo/
        DESTINATION include
        FILES_MATCHING PATTERN "*")
