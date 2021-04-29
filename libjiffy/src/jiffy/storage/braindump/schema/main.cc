#include <stdio.h>
#include <iostream>

#include "schema.h"
#include "record.h"

typedef struct dummy_data_ {
  ulong timestamp;
  int amount;
  //std::string name;
  int amount2;
  bool exists;
} dummy_data;

void main() {
  void* global_log;
  std::vector<std::string> user_data1 = {"1", "123", "true", "a", "543"};
  std::vector<std::string> user_data2 = {"2", "123", "false", "b", "543"};
  std::vector<std::string> user_data3 = {"3", "123", "true", "c", "543"};
  schema_builder builder;

  builder.add_column(data_type("int", NULL), "amount");
  builder.add_column(data_type("int", NULL), "amount1");
  builder.add_column(data_type("bool", NULL), "exists");
  builder.add_column(data_type("char", NULL), "character");
  builder.add_column(data_type("int", NULL), "amount3");
  schema_t schema(builder.get_columns());
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
  // get record ID, and use offset to get to correct blob 
  // do data_to_record vector on blob and output it to the user
  std::vector<std::string> output_data = schema.data_to_record_vector(global_log + 2 * offset);

  for (auto i = output_data.begin(); i != output_data.end(); ++i)
    std::cout << *i << ' ';
  std::cout << std::endl;


  //std::cout << (data->name).size() << std::endl;
  //std::cout << data->name << std::endl;

  // go through schema classs
  /*
  std::vector<std::string> record = schema.data_to_record_vector((void*)data);
  std::string output;
  schema.record_vector_to_data(output, record);
  std::cout << output << std::endl;
  */

  // go through record class
  /*
  record_t record = schema.apply_unsafe(0, (void*)data);
  for (size_t i = 0; i < schema.size(); i++) {
    field_t curr_field = record.at(i);
    // keep data in record format when working with it
    std::cout << i << ": " << curr_field.to_string() << std::endl;
  }
  */
  

  // SELECT:
  // use filter and std::unique_ptr<record_cursor> atomic_multilog::execute_filter()
}