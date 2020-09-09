#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#include <dlfcn.h>

    long search_realtime = 0;
    long search_realtime_nano = 0;
    clock_t search_stime = 0;
    clock_t search_utime = 0;

    long load_realtime = 0;
    long load_realtime_nano = 0;
    clock_t load_stime = 0;
    clock_t load_utime = 0;

    long remove_realtime = 0;
    long remove_realtime_nano = 0;
    clock_t remove_stime = 0;
    clock_t remove_utime = 0;

void print_result(FILE* file)
{
    if(search_realtime_nano<0)
    {
        search_realtime_nano+=1000000000;
        search_realtime--;
    }
    if(load_realtime_nano<0)
    {
        load_realtime_nano+=1000000000;
        load_realtime--;
    }    
    if(remove_realtime_nano<0)
    {
        remove_realtime_nano+=1000000000;
        remove_realtime--;
    }  

    long clocks_per_second = sysconf(_SC_CLK_TCK);
    fprintf(file,"Szukanie\n");
    fprintf(file,"Czas rzeczywisty: %lu sekund  %ld nanosekund \n", search_realtime, search_realtime_nano);
    fprintf(file,"Czas w trybie systemowym: %f \n", (float)(search_stime)/clocks_per_second);
    fprintf(file,"Czas w trybie uzytkownika: %f \n", (float)(search_utime)/clocks_per_second);

    fprintf(file,"\nŁadowanie do pamięci\n");
    fprintf(file,"Czas rzeczywisty: %lu sekund  %ld nanosekund \n", load_realtime, load_realtime_nano);
    fprintf(file,"Czas w trybie systemowym: %f \n", (float)(load_stime)/clocks_per_second);
    fprintf(file,"Czas w trybie uzytkownika: %f \n", (float)(load_utime)/clocks_per_second);

    fprintf(file,"\nUsuwanie\n");
    fprintf(file,"Czas rzeczywisty: %lu sekund  %ld nanosekund \n", remove_realtime, remove_realtime_nano);
    fprintf(file,"Czas w trybie systemowym: %f \n", (float)(remove_stime)/clocks_per_second);
    fprintf(file,"Czas w trybie uzytkownika: %f \n", (float)(remove_utime)/clocks_per_second);
}

int main(int args_count, char *args[])
{
    void *handle = dlopen("./libzad1.so", RTLD_LAZY);
    if(!handle){ fprintf( stderr, "brak biblioteki"); exit(1); }

    void (*create_table)();
    void (*search_directory)();
    void (*load_temp_file)();
    void (*remove_index)();
    void (*set_config)();
    void (*free_table)();

    create_table = (void (*)()) dlsym(handle, "create_table");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    search_directory = (void (*)()) dlsym(handle, "search_directory");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    load_temp_file = (void (*)()) dlsym(handle, "load_temp_file");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    remove_index = (void (*)())dlsym(handle, "remove_index");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    set_config = (void (*)()) dlsym(handle, "set_config");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    free_table = (void (*)())dlsym(handle, "free_table");
    if(dlerror() != NULL){ fprintf( stderr, "brak funckcji"); exit(1); }

    int i=1;
    struct tms start_time;
    struct tms end_time;
    struct timespec start_real_time, end_real_time;

    while(i < args_count)
    {
        clock_gettime(CLOCK_REALTIME, &start_real_time);
        times(&start_time);
        char *arg = args[i];
        i++;
        if( strcmp(arg, "create_table") == 0 )
        {
            if(i >= args_count) 
            {
                fprintf( stderr, "zla liczba argumentow");
                exit(1);
            }
            char *size_string = args[i];
            i++;

            int size = atoi(size_string);
            if(size <= 0)
            {
                fprintf( stderr, "size can't be <= 0");
                exit(1);
            }
            (*create_table)((size_t)size);
        }
        else if( strcmp(arg, "search_directory") == 0 )
        {
            if(i >= args_count) 
            {
                fprintf( stderr, "zla liczba argumentow");
                exit(1);
            }
            char *dir = args[i];
            i++;
            if(i >= args_count) 
            {
                fprintf( stderr, "zla liczba argumentow");
                exit(1);
            }
            char *file_name = args[i];
            i++;
            if(i >= args_count) 
            {
                fprintf( stderr, "zla liczba argumentow");
                exit(1);
            }
            char *temp_file_name = args[i];
            i++;

            (*set_config)(dir, file_name,temp_file_name);
            (*search_directory)();
            clock_gettime(CLOCK_REALTIME, &end_real_time);
            times(&end_time);
           
            search_realtime += end_real_time.tv_sec - start_real_time.tv_sec;
            search_realtime_nano += end_real_time.tv_nsec - start_real_time.tv_nsec;
            search_stime += (end_time.tms_cstime - start_time.tms_cstime);
            search_utime += (end_time.tms_cutime- start_time.tms_cutime);
        }
        else if( strcmp(arg, "load_file") == 0 )
        {
            (*load_temp_file)();

            clock_gettime(CLOCK_REALTIME, &end_real_time);
            times(&end_time);

            load_realtime += end_real_time.tv_sec - start_real_time.tv_sec;
            load_realtime_nano += end_real_time.tv_nsec - start_real_time.tv_nsec;
            load_stime += (end_time.tms_cstime - start_time.tms_cstime);
            load_utime += (end_time.tms_cutime - start_time.tms_cutime);
        }
        else if( strcmp(arg, "remove_block") == 0 )
        {
            if(i >= args_count) 
            {
                fprintf( stderr, "zla liczba argumentow");
                exit(1);
            }
            char *index_string = args[i];
            i++;

            int index = atoi(index_string);
            if(index < 0)
            {
                fprintf( stderr, "index can't be < 0");
                exit(1);
            }
            (*remove_index)(index);

            clock_gettime(CLOCK_REALTIME, &end_real_time);
            times(&end_time);

            remove_realtime += end_real_time.tv_sec - start_real_time.tv_sec;
            remove_realtime_nano += end_real_time.tv_nsec - start_real_time.tv_nsec;
            remove_stime += (end_time.tms_cstime - start_time.tms_cstime);
            remove_utime += (end_time.tms_cutime- start_time.tms_cutime);
        }
        else
        {
            fprintf( stderr, "zły argument");
            exit(1);
        }
        
    }

    (*free_table)();
    dlclose(handle);

    print_result(stdout);

    // FILE *f = fopen("wyniki.txt", "w");
    // if (f == NULL)
    // {
    //     printf("Error opening file.\n");
    // }
    // else
    // {
    //     print_result(f);
    // }

    return 0;
}