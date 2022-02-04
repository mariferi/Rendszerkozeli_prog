#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <omp.h>
#include <sys/wait.h>
#include <signal.h>
#include <pwd.h>
#include <limits.h>
#include <stdbool.h>
#include "colors.h"
////////////////////////////////////////////////
#define BUFFER 1024        //socket buffer
#define PORT 80            //portszám
#define IP "193.6.135.162" //cél ip

char filename[30];
bool filename_ok = false;
int picture; //beolvasott file
#include "feri.h"

int main(int argc, char *argv[]) //-fopenmp
{
    char *arg = argv[1], *verzio = "--version", *help = "--help", *myNeptunID = "ZC44G0";
    int NumCh;
    clock_t Unwrap_time;
    srand(time(NULL));
    system("clear"); //Terminál tisztítás
    /////////////////////////   Args    ////////////////////////////////////////////////////
    if (argc == 1) //Nincs argumentum
    {
        fprintf(stdout, "No filename in args!\n");
        filename_ok = false; //filenev még nincs
    }
    else if (argc >= 2) //1 vagy több argumentum
    {
        if (!strcmp(arg, verzio)) //--version
        {
            printVersioninfo();
            exit(0);
        }
        else if (!strcmp(arg, help)) //--help
        {
            printHelp();
            exit(0);
        }
        else if (strstr(arg, ".bmp") != 0) //filename(cpu.bmp)
        {
            strcpy(filename, arg);
            filename_ok = true;
            fprintf(stdout, "Filename given in args : %s Opening.......\n", filename);
            picture = open(filename, O_RDONLY);
            Unwrap_time = clock(); //timer start
        }
    }

    if (!filename_ok) //ha nincs fájlnév az argban,tallózás
    {
        picture = BrowseForOpen(); //beolvasott file
        filename_ok = true;
        Unwrap_time = clock(); //timer start
    }

    /////////////////       Átalakítás       /////////////////////////////////////////////
    signal(SIGINT, WhatToDo);                        //signalkezelő
    signal(SIGALRM, WhatToDo);                       //1mp mérő
    alarm(1);                                        //1mp re kell
    char *pixel_array = ReadPixels(picture, &NumCh); //beolvassa a képfájl teljes tartalmát egy dinamikus memterületre és ennek a címével tér vissza
    close(picture);
    char *Unwrapped = Unwrap(pixel_array, NumCh);
    //////////////  idő ////////////////
    //sleep(2);//alarm test
    printf("Time left until alarm: %dsec\n", alarm(0));
    Unwrap_time = clock() - Unwrap_time;
    double time_taken = ((double)Unwrap_time) / CLOCKS_PER_SEC; // in seconds
    printf("Unwrap time: %fs \n", time_taken);
    /////////////        küldés        ///////////////////////////////////////////////
    if ((Post(myNeptunID, Unwrapped, NumCh)) == 0)
    {
        printf("Upload : Successful!\n");
    }
    else
    {
        printf("Upload : Unsuccessful!\n");
    }
    /////////////////   Debug   ////////////////////////////////////////////////////////
    //printf("Sent Message:%s", Unwrapped);
    //printf("NumCh:%d\n", NumCh);
    //printf("filename_ok? %d;filename: %s\n", filename_ok, filename);
    //printf("\nlefutott..\n");
    free(Unwrapped);
    return EXIT_SUCCESS;
}
