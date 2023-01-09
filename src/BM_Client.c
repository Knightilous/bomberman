#ifdef _WIN32
#include <WinSock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

int new_coords_input();
int input_check(const char *raw);

int new_coords_input()
{
    const char *str[4096];
    scanf("%s", str);
    return input_check(str);
}

int input_check(const char *raw)
{
    char *endptr;
    int end = strtol(raw, NULL, 10);
    if (endptr == raw || errno == ERANGE)
    {
        printf("Error: Invalid Input");
        return -1;
    }
    return end;
}

int main(int argc, char **argv)
{
#ifdef _WIN32
    // this part is only required on Windows: it initializes the Winsock2 dll
    WSADATA wsa_data ;
    if (WSAStartup (0x0202 , &wsa_data ))
    {
    printf ("unable to initialize winsock2 \n");
    return -1;
 }
#endif
    int pos_x;
    int pos_y;
    int s = socket (AF_INET , SOCK_DGRAM , IPPROTO_UDP );
    if (s < 0)
    {
    printf ("unable to initialize the UDP socket \n");
    return -1;
    }
    printf ("socket %d created \n", s);
    struct sockaddr_in sin;
    inet_pton (AF_INET , "127.0.0.1" , &sin.sin_addr ); // this will create a big endian 32 bit address
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9999); // converts 9999 to big endian
    while (1) //program loop
    {
        puts("\nNEW COORDINATES");
        printf("insert X coordinates: ");
        pos_x = new_coords_input();
        if (pos_x < 0)
        {
            puts("Invalid X position");
            return 1;
        }
        printf("insert Y coordinates: ");
        pos_y = new_coords_input();
        if (pos_y < 0)
        {
            puts("Invalid Y position");
            return 1;
        }
        int max_len = 8;
        char *buffer = malloc(4096);
        snprintf(buffer, max_len, "%d%s%d", pos_x, ",", pos_y);
        printf("sending buffer: %s\n", buffer);
        int sent_bytes = sendto (s, buffer , max_len, 0, (struct sockaddr *)&sin, sizeof (sin));
        printf ("sent %d bytes via UDP \n", sent_bytes );
    }
    return 0;
}