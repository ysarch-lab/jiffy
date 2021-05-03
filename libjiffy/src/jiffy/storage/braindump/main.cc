#include <stdio.h>
#include <iostream>

#include "schema.h"
#include "record.h"

int main() {
  void* global_log;
  // generate dummy values
  std::vector<std::string> user_data1 = {"1", "123", "true", "a", "543"};
  std::vector<std::string> user_data2 = {"2", "321", "false", "b", "543"};
  std::vector<std::string> user_data3 = {"3", "456", "true", "c", "543"};
  confluo::schema_builder builder;

  // add columns to schema builder
  builder.add_column(confluo::data_type("int", NULL), "amount");
  builder.add_column(confluo::data_type("int", NULL), "amount1");
  builder.add_column(confluo::data_type("bool", NULL), "exists");
  builder.add_column(confluo::data_type("char", NULL), "character");
  builder.add_column(confluo::data_type("int", NULL), "amount3");
  confluo::schema_t schema(builder.get_columns());
  // allocated space for global_log to hold 3 records
  global_log = malloc(schema.record_size() * 3);

  // write:
  // take vector<string> from user and make a blob out of it
  void* blob1 = schema.record_vector_to_data(user_data1);
  void* blob2 = schema.record_vector_to_data(user_data2);
  void* blob3 = schema.record_vector_to_data(user_data3);
  size_t offset = schema.record_size();
  memcpy(global_log, blob1, schema.record_size());
  memcpy(global_log + offset, blob2, schema.record_size());
  memcpy(global_log + 2 * offset, blob3, schema.record_size());

  // read: 
  // use offset to get to 3rd blob 
  void* targeted_record = global_log + 2 * offset;
  // convert blob to vector<string>
  std::vector<std::string> output_data = schema.data_to_record_vector(targeted_record);

  // pass through columns of record
  for (auto i = output_data.begin(); i != output_data.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;

  // projection:
  size_t end_of_log = (size_t) global_log + 3*offset;
  for (size_t curr_record = (size_t) global_log; curr_record < end_of_log; curr_record += offset) {
    // apply schema to pointer within global log to get current record
    confluo::record_t record = schema.apply_unsafe(0, (void*) curr_record);
    // output 2nd element of given record 
    confluo::field_t curr_field = record.at(2);
    std::cout << curr_field.to_string() << std::endl;
  }
  std::cout << std::endl;

  // SELECT:
  // use filter and std::unique_ptr<record_cursor> atomic_multilog::execute_filter()
}