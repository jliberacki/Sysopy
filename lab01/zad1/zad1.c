#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "zad1.h"

char *directory_path;
char *file_name;
char *temp_file_name;

size_t blocks_count;
char **memory_blocks;

void set_config(char *directory, char *file, char *temp_file)
{
    directory_path = directory;
    file_name = file;
    temp_file_name = temp_file;
}

void search_directory()
{
    char* find_command;
    find_command = calloc(1, 50 + strlen(directory_path) + strlen(file_name) + strlen(temp_file_name));
    sprintf( find_command, "find %s -name \"%s\" > %s 2>/dev/null", directory_path, file_name, temp_file_name);

    system(find_command);

    free(find_command);
}

void create_table(size_t size)
{
    blocks_count = size;
    memory_blocks = calloc(size, sizeof(char*));
    if(!memory_blocks)
    {
        fprintf( stderr, "error allocating memory\n");
        exit(1);
    }
}

int add_memory_block(char *block)
{
    for (size_t i = 0; i < blocks_count; ++i)
    {
        if(memory_blocks[i] == 0)
        {
            memory_blocks[i] = block;
            return (int)i;
        }
    }
    fprintf( stderr, "blocks table is full\n");
    exit(1);
}

int load_temp_file()
{
    FILE *file;
    long lSize;
    char *buffer;

    file = fopen ( temp_file_name , "r" );
    if( file == NULL )
    {
        perror(temp_file_name);
        exit(1);
    }

    fseek( file , 0L , SEEK_END);
    lSize = ftell( file );
    rewind( file );

    buffer = calloc( 1, lSize+1 );
    if( !buffer )
    {
        fclose(file);
        fprintf( stderr, "error allocating memory\n");
        exit(1);
    }

    if(fread( buffer, (size_t) lSize, 1, file) != 1)
    {
        fclose(file);
        free(buffer);
        fprintf( stderr, "error allocating memory\n");
        exit(1);
    }
    
    fclose(file);

    return add_memory_block(buffer);
}

void remove_index(int index)
{
    if(memory_blocks[index] != NULL)
    {
        free(memory_blocks[index]);
        memory_blocks[index] = NULL;
    }
}

void free_table()
{
    for(int i=0; i < blocks_count; i++)
    {
        if(memory_blocks[i] != NULL)
        {
            free(memory_blocks[i]);
            memory_blocks[i] = NULL;
        }
    }
}
