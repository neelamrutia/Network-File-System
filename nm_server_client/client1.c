#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char *clientIP = "127.0.0.1";
int clientPort = 10110;
char *namingServerIP = "127.0.0.1";
int namingServerPort = 5565;
#define pathSIZE 300

void connection_to_storage_server_for_read_write(char *ip, int port, char *path, char *function)
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = port;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("[+] Connected to the server.\n");
    printf("%d\n", port);
    printf("%s\n", ip);
    printf("%s\n", path);
    char buffer1[1024];
    while (1)
    {
        int n = recv(sock, buffer1, sizeof(buffer1) - 1, 0);
        if (n <= 0)
        {
            continue;
        }
        buffer1[n] = '\0';
        printf("%s\n", buffer1);
        if (strstr(buffer1, "Accepted") != NULL)
        {
            break;
        }
    }
    send(sock, path, strlen(path), 0);
    usleep(1000);
    memset(buffer, '\0', sizeof(buffer));
    strcpy(buffer, function);
    send(sock, buffer, strlen(buffer), 0);
    memset(buffer, '\0', sizeof(buffer));
    usleep(1000);
    printf("-----------FILE CONTENTS-----------\n");
    printf("\n--------x---------------x----------------x----\n");

    if (strstr(function, "READ") != NULL)
    {
        while (1)
        {
            int n = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (n <= 0)
            {
                usleep(100);
                continue;
            }
            buffer[n] = '\0';
            // Check if the received message contains "STOP"
            char *stop_pos = strstr(buffer, "STOP");
            if (stop_pos != NULL)
            {
                // Print up to the start of "STOP"
                *stop_pos = '\0';
                printf("%s", buffer);
                printf("\n--------x---------------x----------------x----\n");
                printf("----DONE----\n");
                break;
            }
            printf("%s", buffer);
            memset(buffer, '\0', sizeof(buffer));
        }
        close(sock);
    }

    else if (strstr(function, "WRITE") != NULL)
    {
        printf("Enter choice: 0 for write, 1 for append\n");
        char choice[10];
        memset(choice, '\0', sizeof(choice));
        fgets(choice, 10, stdin);
        choice[strlen(choice) - 1] = '\0';
        send(sock, choice, sizeof(choice), 0);
        int a = atoi(choice);
        int k = 0;
        printf("WAIT for other to complete WRITE\n");
        while (1)
        {
            int n = recv(sock, buffer, sizeof(buffer), 0);
            if (k == 0)
            {
                k++;
            }
            if (n == 0)
            {
                continue;
            }
            buffer[n] = '\0';
            if (strstr(buffer, "Accepted") != NULL)
            {
                printf("----DONE----\n");
                break;
            }
            // printf("%s\n", buffer);
            memset(buffer, '\0', sizeof(buffer));
        }
        if (a == 0)
        {
            printf("Enter string to write: \n");
            while (1)
            {
                char sTR[1024];
                fgets(sTR, 1024, stdin);
                sTR[strlen(sTR) - 1] = '\0';
                send(sock, sTR, sizeof(sTR), 0);
                if (strstr(sTR, "STOP") != NULL)
                {
                    break;
                }
            }
            close(sock);
        }
        else if (a == 1)
        {

            printf("Enter string to append: \n");
            while (1)
            {
                char sTR[1024];
                fgets(sTR, 1024, stdin);
                sTR[strlen(sTR) - 1] = '\0';
                send(sock, sTR, sizeof(sTR), 0);
                if (strstr(sTR, "STOP") != NULL)
                {
                    break;
                }
            }
            close(sock);
        }
    }
    else if (strstr(function, "DETAILS") != NULL)
    {
        char strin1[1024];
        int n1 = recv(sock, strin1, sizeof(strin1), 0);
        strin1[n1] = '\0';
        printf("SIZE --- %s\n", strin1);
        while (1)
        {
            char strin2[1024];
            int n2 = recv(sock, strin2, sizeof(strin2), 0);
            if (n2 == 0)
            {
                continue;
            }
            strin2[n2] = '\0';
            printf("PERMISSION --- %s\n", strin2);
            break;
        }
        close(sock);
    }
}

void handle_read_write(int sock, char *path, char *function)
{
    char buffer[1024];
    char ip_for_stor[1024];
    int port_for_stor;
    int c = 4;
    while (c--)
    {
        int n = recv(sock, buffer, sizeof(buffer), 0);
        buffer[n] = '\0';
        printf("jum barabar %s\n", buffer);
        if (c == 2)
        {
            strcpy(ip_for_stor, buffer);
            memset(buffer, '\0', sizeof(buffer));
        }
        else if (c == 1)
        {
            port_for_stor = atoi(buffer);
            memset(buffer, '\0', sizeof(buffer));
        }
        else if (c == 3)
        {
            if (strstr(buffer, "STOP") != NULL)
            {
                printf("File not found\n");
                return;
            }
        }
        else if (c == 0)
        {
            if (strstr(buffer, "SELF") != NULL)
            {
                printf("SELF if\n");
                connection_to_storage_server_for_read_write(clientIP, port_for_stor, path, function);
            }
            else
            {
                printf("buffer : %s\n", buffer);
                char path_dup[1024];
                strcpy(path_dup, "./");
                strcat(path_dup, buffer);
                // printf("path_dup : %s\n", path_dup);
                strcat(path_dup, "/");
                strcat(path_dup, path);
                printf("path_dup : %s\n", path_dup);
                strcpy(path, path_dup);
                connection_to_storage_server_for_read_write(ip_for_stor, port_for_stor, path, function);
            }
        }
        memset(buffer, '\0', sizeof(buffer));
    }
    // connection_to_storage_server_for_read_write(ip_for_stor, port_for_stor, path, function);
}

void handle_create_delete(int sock, char *path, char *function)
{
    usleep(1000);
    char function1[1024];
    memset(function1, '\0', sizeof(function1));
    strcpy(function1, function);
    send(sock, function1, strlen(function1), 0);
    usleep(1000);
    while (1)
    {
        char buffer[1024];
        int n = recv(sock, buffer, sizeof(buffer), 0);
        if (n == 0)
        {
            continue;
        }
        buffer[n] = '\0';
        printf("%s\n", buffer);
        break;
        memset(buffer, '\0', sizeof(buffer));
    }
}

void handle_copy(int sock, char *path)
{
    char buffer[1024];
    memset(buffer, '\0', sizeof(buffer));
    int n = recv(sock, buffer, sizeof(buffer), 0);
    buffer[n] = '\0';
    // printf("%s\n", buffer);
    if (strstr(buffer, "Accepted") != NULL)
    {
        printf("Accepted\n");
    }
    else
    {
        printf("Not Accepted\n");
    }
    memset(buffer, '\0', sizeof(buffer));
}

int main()
{
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;
    char buffer[1024];
    int n;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = namingServerPort;
    addr.sin_addr.s_addr = inet_addr(namingServerIP);
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    printf("[+] Connected to the server.\n");
    printf("The Client must provide the path to the file/directory wherever applicable.\n");
    printf("------------ CLIENT INTERFACE ------------\n");
    while (1)
    {
        printf("Our Server Provied The Following Functionalities :\n");
        printf("press 1 for Reading, Writing, and Retrieving Information about Files\n");
        printf("press 2 for Creating and Deleting Files and Folders\n");
        printf("press 3 for Copying Files/Directories Between Storage Servers\n");
        char *choice = (char *)malloc(10 * sizeof(char));
        memset(choice, '\0', 10);
        printf("Enter your choice : ");
        fgets(choice, 10, stdin);
        send(sock, choice, strlen(choice), 0);
        if (atoi(choice) == 1)
        {
            char buffer[1024];
            char *function = (char *)malloc(100 * sizeof(char));
            char *path = (char *)malloc(pathSIZE * sizeof(char));
            memset(path, '\0', pathSIZE);
            memset(function, '\0', 100);
            printf("Enter the function with path of the file you want to access : ");
            fgets(buffer, 1024, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            char *token = strtok(buffer, " ");
            strcpy(function, token);
            function[strlen(function)] = '\0';
            token = strtok(NULL, " ");
            strcpy(path, token);
            send(sock, path, strlen(path), 0);
            usleep(1000);
            send(sock, function, strlen(function), 0);
            usleep(1000);
            memset(buffer, '\0', sizeof(buffer));
            char response[1024];
            int flag = 0;
            // while (1)
            // {
            //     int nn = recv(sock, response, sizeof(response), 0);
            //     if (nn == 0)
            //     {
            //         continue;
            //     }
            //     response[nn] = '\0';
            //     printf("responce ---- %s\n", response);
            //     if (strcmp(response, "Not_Found") == 0)
            //     {
            //         flag = 1;
            //         printf("File not found\n");
            //         break;
            //     }
            //     break;
            // }
            if (flag == 0)
            {
                handle_read_write(sock, path, function);
            }
        }
        else if (atoi(choice) == 2)
        {
            char buffer[1024];
            char *function = (char *)malloc(100 * sizeof(char));
            char *path = (char *)malloc(pathSIZE * sizeof(char));
            memset(path, '\0', pathSIZE);
            memset(function, '\0', 100);
            printf("Enter the function with path of the file you want to access : ");
            fgets(buffer, 1024, stdin);
            buffer[strlen(buffer) - 1] = '\0';
            char *token = strtok(buffer, " ");
            strcpy(function, token);
            function[strlen(function)] = '\0';
            token = strtok(NULL, " ");
            strcpy(path, token);
            send(sock, path, strlen(path), 0);
            char response[1024];
            int flag = 0;
            // while (1)
            // {
            //     int nn = recv(sock, response, sizeof(response), 0);
            //     if (nn == 0)
            //     {
            //         continue;
            //     }
            //     response[nn] = '\0';

            //     if (strcmp(response, "Not_Found") == 0)
            //     {
            //         flag = 1;
            //         printf("File not found\n");
            //         break;
            //     }
            //     break;
            // }
            if (flag == 0)
            {
                handle_create_delete(sock, path, function);
            }
        }
        else if (atoi(choice) == 3)
        {
            char *function = (char *)malloc(100 * sizeof(char));
            char *src_path = (char *)malloc(pathSIZE * sizeof(char));
            char *dst_path = (char *)malloc(pathSIZE * sizeof(char));
            memset(src_path, '\0', pathSIZE);
            memset(dst_path, '\0', pathSIZE);
            memset(function, '\0', 100);
            printf("Enter the path of the file/directory you want to copy from : ");
            fgets(src_path, pathSIZE, stdin);
            printf("Enter the path of the directory you want to copy to : ");
            fgets(dst_path, pathSIZE, stdin);
            src_path[strlen(src_path) - 1] = '\0';
            dst_path[strlen(dst_path) - 1] = '\0';
            char *path = (char *)malloc(5 * pathSIZE * sizeof(char));
            memset(path, '\0', 5 * pathSIZE);
            strcat(path, src_path);
            strcat(path, "|");
            strcat(path, dst_path);
            printf("path : %s\n", path);
            send(sock, path, strlen(path), 0);
            memset(buffer, '\0', sizeof(buffer));
            handle_copy(sock, path);
        }
        else if (atoi(choice) == 4)
        {
            break;
        }
    }
    printf("Disconnected from the server.\n");
    close(sock);
    return 0;
}