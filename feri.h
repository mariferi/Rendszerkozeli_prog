void printVersioninfo()
{
    system("clear");
    printf("\n//////////////// Version info ////////////////\n");
    printf("BMP Decoder & Sender\n");
    printf("Version:2.03 LTS 'Chameleon'\n");
    printf("Finished:2021.04.26\n");
    printf("Senior Programmer/Project Lead: Mári Ferenc\n");
    printf("//////////////// Version info ////////////////\n\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void printHelp()
{
    system("clear");
    printf("\tBMP Decoder & Sender\n");
    printf("\t\tHelp\n");
    printf("------------------------------------------\n");
    printf("Args:    \tEffect:\n\n");
    printf("--help   \tdisplay running options\n");
    printf("--version\tdisplay version info\n");
    printf("XYZ.bmp \tf.name,to open,decode,send\n");
    printf("no args  \tinput will be given later\n");
    printf("compile errors? Use the '-fopenmp'flag\n\n");
    printf("------------------------------------------\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char random_char()
{
    int lower = 97, upper = 122; //Ascii:97(a)->122(z)
    int number = (rand() % (upper - lower + 1)) + lower;
    return (char)number;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool random_bool()
{
    int lower = 0, upper = 1;
    int number = (rand() % (upper - lower + 1)) + lower;
    if (number == 0)
        return false;
    else
        return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WhatToDo(int sig)
{
    if (sig == SIGINT)
    {
        pid_t pid;
        pid = fork();
        if (pid == 0) //child
        {
            fprintf(stderr, "Ctrl+C button combination will NOT stop the program!\n");
            kill(getpid(), SIGKILL); //önmagát leállítja
        }
    }
    else if (sig == SIGALRM)
    {
        fprintf(stderr, "This program has been running for too long!\n");
        exit(1); // program álljon le egy hibakóddal!
    }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int BrowseForOpen()
{

    DIR *dir;
    struct dirent *entry;
    struct stat inode;
    char tmp[256];
    chdir(getenv("HOME"));
    printf("Please choose a correct .bmp file to decode!\n");
    system("pwd");
    puts("");
    do
    {
        dir = opendir(".");
        while ((entry = readdir(dir)) != NULL)
        {
            //printf("%s\t", (*e).d_name);
            stat((*entry).d_name, &inode);
            switch (inode.st_mode & S_IFMT)
            {
            case S_IFDIR: //ha mappa
                printf("%s%s%s\t", BLU, (*entry).d_name, WHT);
                break;
            case S_IFREG: //ha file
                printf("%s%s%s\t", RED, (*entry).d_name, WHT);
                break;
            }
        }
        printf("\nWhich '.bmp' ?\n");
        scanf("%s", tmp);
        if (stat(tmp, &inode) >= 0)
        {
            if (inode.st_mode & S_IFDIR) //ha mappa
            {
                chdir(tmp);
                system("clear");
                system("pwd");
                puts("");
                closedir(dir);
            }
        }
        else
        {
            system("clear");
            printf("The Typed object doesn't exist!\n");
        }
    } while ((inode.st_mode & S_IFREG) == 0); //amíg nem file
    system("clear");
    strcpy(filename, tmp);
    printf("Chosen file :%s Opening.......\n", filename);
    closedir(dir);
    if (strstr(filename, ".bmp") == NULL) //nem .bmp file
    {
        printf("Warning! %s is NOT a .bmp file!\n", filename);
    }
    int f = open(filename, O_RDONLY); //ha file,open,//file bináris megyitás,csak olvasás,f->file descriptor,filename->filename
    if (f >= 0)                       //sikeres
    {
        printf("Opening %s : Successful\n", filename);
    }
    else
    {
        printf("Opening %s : Unsuccessful\n", filename);
        exit(1);
    }
    return f;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *Unwrap(char *Pbuff, int NumCh)
{
    char *decoded = (char *)malloc((sizeof(char) * NumCh));
    //int NumProc = omp_get_num_procs();
    //int NumThreads = omp_get_num_threads();
    //printf("Number of Processors: %d and Threads:%d\n", NumProc, NumThreads);
    if (decoded == NULL)
    {
        fprintf(stderr, "Unsuccessful memory allocation!\n");
        exit(1); // program álljon le egy hibakóddal!
    }
    /// decoding    ///
    printf("Multi Threaded Decoding.....\n");
    int chrnum = NumCh * 3;
#pragma omp parallel for ordered schedule(guided)
    for (int i = 0; i < chrnum; i += 3) //3masával
    {
        //Éselés
        char akt1 = Pbuff[i] & 3;     //aktuális char1  //11000000;3=00000011
        char akt2 = Pbuff[i + 1] & 7; //aktuális char2  //00111000;7=00000111
        char akt3 = Pbuff[i + 2] & 7; //aktuális char3  //00000011;7=00000111

        //shift,akt3 jó
        decoded[i / 3] = akt1 << 6 | akt2 << 3 | akt3;
        //printf("%c", decoded[i/3]);
    }
    free(Pbuff);
    return decoded; //mem cím
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *TestArray(int *NumCh)
{
    int chrnum = 9;                                               //1 pixel adat 3 char végére
    char *pixel_tomb = (char *)malloc(sizeof(char) * chrnum + 1); //3db 24bites pixel(72) (char=8bit*3*3=72)
    //8 random bit(random char)
    char r1 = random_char(), r2 = random_char(), r3 = random_char();

    char a = 97; //01 100 001
    char b = 98; //01 100 010
    char c = 99; //01 100 011

    //éselés
    char a1 = a & 192; //192=11000000
    char a2 = a & 56;  //56=00111000
    char a3 = a & 7;   //7=00000111

    char b1 = b & 192; //192=11000000
    char b2 = b & 56;  //56=00111000
    char b3 = b & 7;   //7=00000111

    char c1 = c & 192; //192=11000000
    char c2 = c & 56;  //56=00111000
    char c3 = c & 7;   //7=00000111

    //shiftelés
    a1 = a1 >> 6; //00000011
    a2 = a2 >> 3; //00000111
    //a3 jó      //00000111
    b1 = b1 >> 6; //00000011
    b2 = b2 >> 3; //00000111
    //b3 jó      //00000111
    c1 = c1 >> 6; //00000011
    c2 = c2 >> 3; //00000111
    //c3 jó      //00000111

    //helycsinálás
    r1 = r1 << 3; //xxxxx000
    r2 = r2 << 3; //xxxxx000
    r3 = r3 << 3; //xxxxx000

    //vagyolás(mind3)
    a1 = r1 | a1;
    a2 = r2 | a2;
    a3 = r3 | a3;

    b1 = r1 | b1;
    b2 = r2 | b2;
    b3 = r3 | b3;

    c1 = r1 | c1;
    c2 = r2 | c2;
    c3 = r3 | c3;

    //a pixel
    pixel_tomb[0] = a1;
    pixel_tomb[1] = a2;
    pixel_tomb[2] = a3;
    //b
    pixel_tomb[3] = b1;
    pixel_tomb[4] = b2;
    pixel_tomb[5] = b3;
    //c
    pixel_tomb[6] = c1;
    pixel_tomb[7] = c2;
    pixel_tomb[8] = c3;

    /*
    for (int i = 0; i < sizeof(pixel_tomb) + 1; i++)
    {
        printf("%d: %d\n", i, pixel_tomb[i]);
    }
    */

    //végeredmény
    // a: 1.px = xxxxxx01 xxxxx100 xxxxx001
    // b: 2.px = xxxxxx01 xxxxx100 xxxxx010
    // c: 3.px = xxxxxx01 xxxxx100 xxxxx011
    //R:8 G:8 B:8 = 24bit = 1db pixel

    *NumCh = chrnum / 3; //int *NumCh változó memóriaterületére helyezze el a kódolt karekterek számát (3)
    return pixel_tomb;   //térjen vissza a lefoglalt memóriaterület címével!
    //free(pixel_tomb);//??
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *ReadPixels(int f, int *NumCh)
{
    /////// Mem Foglalás    /////////////////
    int *full_size = (int *)malloc(sizeof(int));
    int *header = (int *)malloc(sizeof(int));
    /////////////////// Adatok keresése //////////(-1 mert 0 ról indul)
    lseek(f, 3, SEEK_SET);              //3.-6. bájtjában szerepel a fájl teljes mérete bájtokban
    read(f, full_size, 4);              ///A kép teljes mérete
    lseek(f, 6, SEEK_SET);              //köv 4 bájta kódolt karakterek száma
    read(f, NumCh, 4);                  //Kódolt karakterek
    lseek(f, 10, SEEK_SET);             //Ezután 4 bájton , hány bájtos a fejrész
    read(f, header, 4);                 //Fejrész méret
    int msg_len = *full_size - *header; //Fő üzenet hossz,Teljes-a fejrész info-k
    /////////////// Fő Mem foglalás    ///////////////
    char *decoded = (char *)malloc(msg_len * sizeof(char));
    if (decoded == NULL)
    {
        fprintf(stderr, "Unsuccessful memory allocation!\n");
        exit(1);
    }
    //////////////  Beolvasás   //////////////////
    lseek(f, *header, SEEK_SET); //elejére
    read(f, decoded, msg_len);   //beolvasás
    //////////////// Debug  ////////////////////
    printf("NumChars: %d; FullSize: %d; Header: %d; Msg_len: %d\n", *NumCh, *full_size, *header, msg_len);
    ///////////// Close & return  ////////////
    close(f);
    return decoded; //memcím decodolt tömb
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Post(char *NeptunID, char *message, int NumCh)
{
    ////////////////// változók   /////////////
    int mysocket, connection, send_error, receive_error;
    unsigned int server_size;
    char on = 1;
    char buffer[BUFFER] = "";
    struct sockaddr_in server;
    ////////// server  //////////////
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons(PORT);
    server_size = sizeof server;
    /////////////// Post konkatenáció   //////////////
    snprintf(buffer, sizeof(buffer),
             "POST /~vargai/post.php HTTP/1.1\r\nHost: irh.inf.unideb.hu\r\nContent-Length: %d\r\nContent-Type: application/x-www-form-urlencoded\r\n\r\nNeptunID=%s&PostedText=%s",
             BUFFER, NeptunID, message);
    ////////////////    socket    ////////////////
    mysocket = socket(AF_INET, SOCK_STREAM, 0); //Ipv4,TCP
    setsockopt(mysocket, SOL_SOCKET, SO_KEEPALIVE, (char *)&on, sizeof(on));

    connection = connect(mysocket, (struct sockaddr *)&server, server_size);
    if (connection < 0)
    {
        fprintf(stderr, "Connection Error\n");
        return 1;
    }
    /////////////// Küldés  /////////////////////
    send_error = send(mysocket, buffer, strlen(buffer) + 1, 0);
    printf("Message:%s", message);
    printf("Sending..........\n");
    if (send_error <= 0)
    {
        fprintf(stderr, "Send Error\n");
        return 2;
    }
    /////////////// Válasz fogadása  /////////////
    receive_error = recv(mysocket, buffer, BUFFER, 0);
    if (receive_error < 0)
    {
        fprintf(stderr, "Receive Error\n");
        return 3;
    }
    /////////   socket lezárás  ////////////
    close(mysocket);
    /////////////////////// Megkapta?   ////////////////
    if (strstr(buffer, "The message has been received.") != NULL) //válaszban benne van e a szöveg?
        return 0;                                                 //igen
    else
        return 4; //nem
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////