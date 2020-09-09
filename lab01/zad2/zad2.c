#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <unistd.h>
#include <time.h>
#include "../zad1/zad1.h"

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

    struct tms start_time;
    struct tms end_time;
    struct timespec start_real_time, end_real_time;

    int i=1;
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
            create_table((size_t)size);
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

            set_config(dir, file_name,temp_file_name);
            search_directory();
            times(&end_time);

            clock_gettime(CLOCK_REALTIME, &end_real_time);
           
            search_realtime += end_real_time.tv_sec - start_real_time.tv_sec;
            search_realtime_nano += end_real_time.tv_nsec - start_real_time.tv_nsec;
            search_stime += (end_time.tms_cstime - start_time.tms_cstime);
            search_utime += (end_time.tms_cutime- start_time.tms_cutime);
        }
        else if( strcmp(arg, "load_file") == 0 )
        {
            load_temp_file();

            times(&end_time);
            clock_gettime(CLOCK_REALTIME, &end_real_time);

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
            remove_index(index);

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

    free_table();

    print_result(stdout);

    return 0;
}