#pragma once


extern void set_config(char *directory, char *file, char *temp_file);
extern void search_directory();
extern void create_table(size_t size);
extern int add_memory_block(char *block);
extern int load_temp_file();
extern void remove_index(int index);
extern void free_table();