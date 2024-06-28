#include "main.h"

int *server_is_present = NULL;
int num_of_storage_servers = 5;
struct TrieNode *root;
struct storage_node *storage_server = NULL;

struct storage_node *st_array[1000];

#define LRULIMIT 5
#define ARRAYSIZE 100007
struct ll *head = NULL;
#define BUFFER_SIZE 1024

int size_cache = 0;

struct storage_node *cache[ARRAYSIZE];

struct ll
{
    int index;
    struct ll *next;
};

void pop_back(struct ll *head)
{
    struct ll *temp = head;
    if (!temp)
        return; // If head is NULL, do nothing
    while (temp->next && temp->next->next)
    {
        temp = temp->next;
    }
    if (temp->next)
    {
        struct ll *temp1 = temp->next;
        temp->next = NULL;
        cache[temp1->index] = NULL;
        printf("Removed %d indexed cache\n", temp1->index);
        free(temp1);
    }
}

void push_front(struct ll *head, int index)
{
    struct ll *temp = (struct ll *)malloc(sizeof(struct ll));
    if (!temp)
    {
        fprintf(stderr, "Memory allocation failed for ll node\n");
        return;
    }
    temp->index = index;
    temp->next = head->next;
    head->next = temp;
    printf("Added %d indexed cache\n", index);
}

void pick_element(struct ll *head, int i)
{
    struct ll *node = head;
    while (node->next)
    {
        if (node->next->index == i)
        {
            printf("Updated\n");
            struct ll *tmp = node->next;
            node->next = tmp->next;
            tmp->next = head->next;
            head->next = tmp;
            break;
        }
        node = node->next;
    }
}

int det_index(char *path)
{
    int len = strlen(path);
    int num = 31;
    int ret = 0;
    for (int i = 0; i < len; i++)
    {
        ret = (ret + path[i] * num) % ARRAYSIZE;
        num = (num * 31) % ARRAYSIZE;
    }
    return ret;
}

void init_cache()
{
    for (int i = 0; i < ARRAYSIZE; i++)
    {
        cache[i] = NULL;
    }
}

void insert_cache(char *path, struct storage_node *st)
{
    int index = det_index(path);
    struct storage_node *st1 = (struct storage_node *)malloc(sizeof(struct storage_node));
    if (!st1)
    {
        fprintf(stderr, "Memory allocation failed for storage_node\n");
        return;
    }
    *st1 = *st; // Copy the entire structure
    printf("duvbwdb\n");
    cache[index] = st1;
    size_cache++;
    if (size_cache > LRULIMIT)
    {
        push_front(head, index);
        pop_back(head);
    }
    else
    {
        printf("Added %d indexed cache\n", index);
        push_front(head, index);
    }
}

void delete_cache(char *path)
{
    int index = det_index(path);
    if (cache[index])
    {
        free(cache[index]);
        cache[index] = NULL;
        size_cache--;
        struct ll *node = head;
        while (node->next)
        {
            if (node->next->index == index)
            {
                printf("Deleted %d\n", index);
                printf("%s\n", path);
                struct ll *tmp = node->next;
                node->next = tmp->next;
                free(tmp);
                break;
            }
            node = node->next;
        }
    }
}

struct storage_node *search_cache(char *path)
{
    int index = det_index(path);
    printf("Searching in cache\n");
    printf("%d\n", index);
    if (!cache[index])
    {
        return NULL;
    }
    else
    {
        printf("Found in cache\n");
        pick_element(head, index);
        return cache[index];
    }
}

struct TrieNode *getNode(char current_char)
{
    struct TrieNode *pNode = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (!pNode)
    {
        fprintf(stderr, "Memory allocation failed for TrieNode\n");
        return NULL;
    }
    pNode->isEndOfPath = 0;
    pNode->valid = 0;
    pNode->storage_node = NULL;
    pNode->current_char = current_char;
    pNode->isDirectory = 0;
    for (int i = 0; i < 128; i++)
    {
        pNode->children[i] = NULL;
    }
    return pNode;
}

void insert_path(const char *path, struct storage_node *storage_node)
{
    struct TrieNode *current = root;
    while (*path)
    {
        if (!current->children[*path])
        {
            current->children[*path] = getNode(*path);
            if (!current->children[*path])
            {
                fprintf(stderr, "Failed to insert path due to memory allocation error\n");
                return;
            }
        }
        current = current->children[*path];
        path++;
    }
    current->isEndOfPath = 1;
    current->storage_node = storage_node;
    current->valid = 1;
}

void print_trie(struct TrieNode *root)
{
    if (root->isEndOfPath)
    {
        printf("\n");
    }
    for (int i = 0; i < 128; i++)
    {
        if (root->children[i])
        {
            printf("%c", i);
            print_trie(root->children[i]);
        }
    }
}

struct TrieNode *search_path(struct TrieNode *root, char *path, int path_len)
{
    if (root->isEndOfPath && (path_len + 1) == strlen(path))
    {
        return root;
    }
    if ((path_len + 1) == strlen(path))
    {
        if (root->storage_node)
        {
            root->storage_node->is_read = 1;
            return root;
        }
        return NULL;
    }
    if (root->children[path[path_len + 1]])
    {
        return search_path(root->children[path[path_len + 1]], path, path_len + 1);
    }
    else
    {
        return NULL;
    }
}

struct TrieNode *search(char *path, int path_len)
{
    for (int i = 0; i < 128; i++)
    {
        if (root->children[i] != NULL)
        {
            if (root->children[i]->current_char == path[0])
            {
                return search_path(root->children[i], path, 0);
            }
        }
    }
    return NULL;
}

int delete_node(struct TrieNode *node, char *path, int current)
{
    if (current + 1 == strlen(path))
    {
        node->isEndOfPath = 0;
        printf("Deleted\n");
        return 1;
    }
    if (node->children[path[current + 1]] != NULL)
    {
        delete_node(node->children[path[current + 1]], path, current + 1);
    }
    return 1;
}

void delete_recursive(struct TrieNode *node)
{
    if (node->isEndOfPath)
    {
        node->isEndOfPath = 0;
        printf("Deleted\n");
    }
    for (int i = 0; i < 128; i++)
    {
        if (node->children[i])
        {
            delete_recursive(node->children[i]);
            free(node->children[i]);
            node->children[i] = NULL;
        }
    }
}

int delete_directory(struct TrieNode *node)
{
    delete_recursive(node);
    return 1;
}

int delete(char *path)
{
    struct TrieNode *node = search(path, 0);
    if (node)
    {
        if (node->isDirectory)
        {
            return delete_directory(node);
        }
        return delete_node(root->children[path[0]], path, 0);
    }
    return 0;
}

void client_connection(int client_sock)
{
    char buffer[1024];
    while (1)
    {
        bzero(buffer, 1024);
        if (recv(client_sock, buffer, sizeof(buffer), 0) <= 0)
        {
            break;
        }
        printf("Client: %s\n", buffer);
        bzero(buffer, 1024);
        printf("Server: ");
        fgets(buffer, 1024, stdin);
        send(client_sock, buffer, strlen(buffer), 0);
    }
}

void replicant_function(char **path_storage, int count_path, struct storage_node *temp, struct storage_node *temp_copy)
{
    int src_port = temp->storage_port_for_NM;
    char *src_ip = temp->storage_ip;
    int dest_port = temp_copy->storage_port_for_NM;
    char *dest_ip = temp_copy->storage_ip;
    int folder_id = temp->storage_id;
    printf("%d\n", folder_id);

    char folder_path_str[1024] = "storage_server_";
    char folder_id_str[1024];
    sprintf(folder_id_str, "%d", folder_id);
    strcat(folder_path_str, folder_id_str);
    strcat(folder_path_str, "/");
    printf("%s\n", folder_path_str);

    give_command_to_nm(dest_ip, dest_port, "CREATE", folder_path_str);

    for (int i = 0; i < count_path; i++)
    {
        char src_path[1024];
        char dest_path[1024] = "./";
        strcpy(src_path, path_storage[i]);
        strcat(dest_path, folder_path_str);

        printf("------ src_path : %s\n", src_path);
        if (src_path[strlen(src_path) - 1] == '/')
        {
            copy_folder_parallely(src_ip, dest_ip, src_port, dest_port, src_path, dest_path, folder_id, temp->storage_port_for_NM, temp_copy->storage_port_for_NM);
        }
        else
        {
            strcat(dest_path, src_path + 2);
            for (int j = strlen(dest_path); j >= 0; j--)
            {
                if (dest_path[j] == '/')
                {
                    dest_path[j + 1] = '\0';
                    break;
                }
            }
            printf("------ dest_path ----- : %s\n", dest_path);
            copy_parallely(src_ip, dest_ip, src_port, dest_port, src_path, dest_path, folder_id, temp->storage_port_for_NM, temp_copy->storage_port_for_NM);
        }
    }
    printf("Replication done\n");
}

void handle_storage_connection(int client_socket)
{
    char buffer[4096];
    char **path_storage = (char **)malloc(sizeof(char *) * 1024);
    if (!path_storage)
    {
        fprintf(stderr, "Memory allocation failed for path_storage\n");
        return;
    }

    for (int i = 0; i < 1024; i++)
    {
        path_storage[i] = (char *)malloc(sizeof(char) * 1024);
        if (!path_storage[i])
        {
            fprintf(stderr, "Memory allocation failed for path_storage[%d]\n", i);
            for (int j = 0; j < i; j++)
            {
                free(path_storage[j]);
            }
            free(path_storage);
            return;
        }
    }

    int count_path = 0;

    struct storage_node *temp = (struct storage_node *)malloc(sizeof(struct storage_node));
    if (!temp)
    {
        fprintf(stderr, "Memory allocation failed for storage_node\n");
        for (int i = 0; i < 1024; i++)
        {
            free(path_storage[i]);
        }
        free(path_storage);
        return;
    }

    ssize_t bytes_received;
    int i = 0;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0)
    {
        buffer[bytes_received - 1] = '\0';
        printf("%s\n", buffer);
        switch (i)
        {
        case 0:
            strcpy(temp->storage_ip, buffer);
            break;
        case 1:
            temp->storage_port_for_NM = atoi(buffer);
            break;
        case 2:
            temp->storage_port_for_client = atoi(buffer);
            break;
        case 3:
            temp->storage_id = atoi(buffer);
            server_is_present[temp->storage_id] = 1;
            break;
        default:
            if (strstr(buffer, "DONE") != NULL)
            {
                break;
            }
            insert_path(buffer, temp);
            strcpy(path_storage[count_path], buffer);
            count_path++;
            continue;
        }
        i++;
    }

    st_array[temp->storage_id] = temp;

    if (temp->storage_id > 2)
    {
        temp->replicate1 = st_array[temp->storage_id - 1];
        temp->replicate2 = st_array[temp->storage_id - 2];
    }
    else
    {
        temp->replicate1 = NULL;
        temp->replicate2 = NULL;
    }

    if (temp->storage_id > 2)
    {
        printf("Replication started\n");
        replicant_function(path_storage, count_path, temp, temp->replicate1);
        replicant_function(path_storage, count_path, temp, temp->replicate2);
    }

    while (1)
    {
        int nn = recv(client_socket, buffer, sizeof(buffer), 0);
        if (nn <= 0)
        {
            break;
        }
        buffer[nn] = '\0';
        if (strstr(buffer, "CLOSE") != NULL)
        {
            break;
        }
    }

    if (temp->storage_id > 2)
    {
        replicant_function(path_storage, count_path, temp, temp->replicate1);
        replicant_function(path_storage, count_path, temp, temp->replicate2);
    }

    send(client_socket, "CLOSE", strlen("CLOSE"), 0);

    while (count_path--)
    {
        delete (path_storage[count_path]);
        delete_cache(path_storage[count_path]);
    }

    for (int i = 0; i < 1024; i++)
    {
        free(path_storage[i]);
    }
    free(path_storage);

    free(temp);

    if (bytes_received == 0)
    {
        printf("Client disconnected\n");
    }
    else if (bytes_received == -1)
    {
        perror("Error receiving data from client");
    }
    close(client_socket);
}

void *handle_storage_connection_thread(void *arg)
{
    int client_socket = *((int *)arg);
    free(arg); // Free the dynamically allocated memory for client_socket
    handle_storage_connection(client_socket);
    close(client_socket);
    return NULL;
}

void *listen_nm_thread(void *args)
{
    int port = 5566;
    char *ip = "127.0.0.1";
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t storage_thread_id;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");

    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = (port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while (1)
    {
        printf("ok\n");
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0)
        {
            perror("Error accepting connection");
            continue;
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        int *client_socket = malloc(sizeof(int));
        *client_socket = client_sock;
        pthread_create(&storage_thread_id, NULL, handle_storage_connection_thread, client_socket);
        pthread_detach(storage_thread_id); // Detach the thread to avoid memory leaks
    }
    return NULL;
}

void give_command_to_nm(char *ip, int port, char *function, char *search_path)
{
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = (port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("[-]Connect error");
        close(sock);
        return;
    }
    printf("[+]Connected to the server.\n");

    while (recv(sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    printf("b\n");

    send(sock, function, strlen(function), 0);
    printf("bo\n");
    usleep(1000);
    send(sock, search_path, strlen(search_path), 0);
    printf("boo\n");
    usleep(1000);

    while (recv(sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "STOP") != NULL)
        {
            break;
        }
    }
    printf("bob\n");

    close(sock);
}

void copy_parallely(char *src_ip, char *dest_ip, int src_port, int dest_port, char *src_path, char *dest_path, int src_server_number, int src_port_for_nm, int dst_port_for_nm)
{
    int src_sock, dest_sock;
    struct sockaddr_in src_addr, dest_addr;
    char buffer[BUFFER_SIZE];

    src_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (src_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&src_addr, '\0', sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = (src_port);
    src_addr.sin_addr.s_addr = inet_addr(src_ip);

    if (connect(src_sock, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
        perror("[-]Connect error");
        close(src_sock);
        return;
    }
    printf("[+]Connected to the source server %d for Copying.\n", src_server_number + 1);

    while (recv(src_sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    send(src_sock, "copy_from_you", strlen("copy_from_you"), 0);
    usleep(1000);
    send(src_sock, src_path, strlen(src_path), 0);

    dest_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (dest_sock < 0)
    {
        perror("[-]Socket error");
        close(src_sock);
        exit(1);
    }
    memset(&dest_addr, '\0', sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = (dest_port);
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    if (connect(dest_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        perror("[-]Connect error");
        close(src_sock);
        close(dest_sock);
        return;
    }
    printf("[+]Connected to the destination server for Pasting.\n");

    while (recv(dest_sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    send(dest_sock, "paste_to_you", strlen("paste_to_you"), 0);
    usleep(1000);

    char *copy_file = strrchr(src_path, '/');
    if (copy_file)
    {
        copy_file++;
    }
    else
    {
        copy_file = src_path;
    }

    char final_dest_path[BUFFER_SIZE];
    snprintf(final_dest_path, BUFFER_SIZE, "%s%s", dest_path, copy_file);
    send(dest_sock, final_dest_path, strlen(final_dest_path), 0);

    while (recv(src_sock, buffer, sizeof(buffer), 0) > 0)
    {
        usleep(1000);
        if (strcmp(buffer + strlen(buffer) - 4, "STOP") == 0)
        {
            buffer[strlen(buffer) - 4] = '\0';
            send(dest_sock, buffer, strlen(buffer), 0);
            send(dest_sock, "STOP", strlen("STOP"), 0);
            printf("Done\n");
            break;
        }
        send(dest_sock, buffer, strlen(buffer), 0);
    }

    close(src_sock);
    close(dest_sock);
}

void copy_to_same(char *src_ip, char *dest_ip, int src_port, int dest_port, char *src_path, char *dest_path, int src_server_number, int src_port_for_nm, int dest_port_for_nm)
{
    int sock;
    struct sockaddr_in addr;
    char buffer[BUFFER_SIZE];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&addr, '\0', sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = (src_port);
    addr.sin_addr.s_addr = inet_addr(src_ip);

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("[-]Connect error");
        close(sock);
        return;
    }
    printf("[+]Connected to the server %d for Copying.\n", src_server_number + 1);

    while (recv(sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    send(sock, "copy_self", strlen("copy_self"), 0);
    usleep(1000);

    char command[BUFFER_SIZE];
    snprintf(command, BUFFER_SIZE, "%s %s", src_path, dest_path);
    send(sock, command, strlen(command), 0);

    close(sock);
}

void copy_folder_parallely(char *src_ip, char *dest_ip, int src_port, int dest_port, char *src_path, char *dest_path, int src_server_number, int src_port_for_nm, int dest_port_for_nm)
{
    int src_sock, dest_sock;
    struct sockaddr_in src_addr, dest_addr;
    char buffer[BUFFER_SIZE];
    int port = rand() % (9999 - 1000 + 1) + 1000;
    char port_str[BUFFER_SIZE];

    src_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (src_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    memset(&src_addr, '\0', sizeof(src_addr));
    src_addr.sin_family = AF_INET;
    src_addr.sin_port = (src_port);
    src_addr.sin_addr.s_addr = inet_addr(src_ip);

    if (connect(src_sock, (struct sockaddr *)&src_addr, sizeof(src_addr)) < 0)
    {
        perror("[-]Connect error");
        close(src_sock);
        return;
    }
    printf("[+]Connected to the source server %d for Copying.\n", src_server_number + 1);

    while (recv(src_sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    send(src_sock, "copy_folder_from_you", strlen("copy_folder_from_you"), 0);
    usleep(1000);
    send(src_sock, src_path, strlen(src_path), 0);

    snprintf(port_str, BUFFER_SIZE, "%d", port);
    send(src_sock, port_str, strlen(port_str), 0);
    usleep(1000);

    dest_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (dest_sock < 0)
    {
        perror("[-]Socket error");
        close(src_sock);
        exit(1);
    }
    memset(&dest_addr, '\0', sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = (dest_port);
    dest_addr.sin_addr.s_addr = inet_addr(dest_ip);

    if (connect(dest_sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) < 0)
    {
        perror("[-]Connect error");
        close(src_sock);
        close(dest_sock);
        return;
    }
    printf("[+]Connected to the destination server for Pasting.\n");

    while (recv(dest_sock, buffer, sizeof(buffer), 0) > 0)
    {
        if (strstr(buffer, "Accepted") != NULL)
        {
            break;
        }
    }

    send(dest_sock, "paste_folder_to_you", strlen("paste_folder_to_you"), 0);
    usleep(1000);

    char *copy_file = strrchr(src_path, '/');
    if (copy_file)
    {
        copy_file++;
    }
    else
    {
        copy_file = src_path;
    }

    char final_dest_path[BUFFER_SIZE];
    snprintf(final_dest_path, BUFFER_SIZE, "%s%s", dest_path, copy_file);
    printf("-----dest_path : %s\n", final_dest_path);
    send(dest_sock, final_dest_path, strlen(final_dest_path), 0);

    usleep(1000);
    send(dest_sock, port_str, strlen(port_str), 0);
    usleep(1000);

    while (recv(src_sock, buffer, sizeof(buffer), 0) > 0)
    {
        usleep(1000);
        if (strcmp(buffer + strlen(buffer) - 4, "STOP") == 0)
        {
            buffer[strlen(buffer) - 4] = '\0';
            send(dest_sock, buffer, strlen(buffer), 0);
            send(dest_sock, "STOP", strlen("STOP"), 0);
            printf("Done\n");
            break;
        }
        send(dest_sock, buffer, strlen(buffer), 0);
    }

    close(src_sock);
    close(dest_sock);
}
// pthread_mutex_t lock1;

void execute_client_request(char *command, char *search_path, int sock)
{
    int a = atoi(command);
    printf("%d\n", a);
    printf("%s\n", search_path);

    if (a == 1)
    {
        usleep(1000);
        char function_exec[1024];
        int n1 = recv(sock, function_exec, sizeof(function_exec), 0);
        function_exec[n1] = '\0';
        printf("recv function -- %s\n", function_exec);

        int write_flag = 0;
        if (strstr(function_exec, "WRITE") != NULL)
        {
            write_flag = 1;
        }

        struct storage_node *srch_cache = search_cache(search_path);
        if (srch_cache != NULL)
        {
            send(sock, "Found", strlen("Found"), 0);
            usleep(1000);
            send(sock, srch_cache->storage_ip, strlen(srch_cache->storage_ip), 0);
            usleep(1000);
            char port_st_str[1024];
            snprintf(port_st_str, sizeof(port_st_str), "%d", srch_cache->storage_port_for_client);
            send(sock, port_st_str, strlen(port_st_str), 0);
            usleep(1000);
            send(sock, "SELF", strlen("SELF"), 0);
            usleep(1000);
        }
        else
        {
            struct TrieNode *node = search(search_path, 0);
            if (node)
            {
                if (node->isEndOfPath == 0)
                {
                    if (node->storage_node->replicate1 != NULL && !write_flag)
                    {
                        send(sock, "Found", strlen("Found"), 0);
                        usleep(1000);
                        send(sock, node->storage_node->replicate1->storage_ip, strlen(node->storage_node->replicate1->storage_ip), 0);
                        usleep(1000);
                        char port_st_str[1024];
                        snprintf(port_st_str, sizeof(port_st_str), "%d", node->storage_node->replicate1->storage_port_for_client);
                        send(sock, port_st_str, strlen(port_st_str), 0);
                        usleep(1000);
                        char path_ss[1024];
                        snprintf(path_ss, sizeof(path_ss), "storage_server_%d", node->storage_node->storage_id);
                        send(sock, path_ss, strlen(path_ss), 0);
                        usleep(1000);
                    }
                    else if (node->storage_node->replicate2 != NULL && !write_flag)
                    {
                        send(sock, "Found", strlen("Found"), 0);
                        usleep(1000);
                        send(sock, node->storage_node->replicate2->storage_ip, strlen(node->storage_node->replicate2->storage_ip), 0);
                        usleep(1000);
                        char port_st_str[1024];
                        snprintf(port_st_str, sizeof(port_st_str), "%d", node->storage_node->replicate2->storage_port_for_client);
                        send(sock, port_st_str, strlen(port_st_str), 0);
                        usleep(1000);
                        char path_ss[1024];
                        snprintf(path_ss, sizeof(path_ss), "storage_server_%d", node->storage_node->storage_id);
                        send(sock, path_ss, strlen(path_ss), 0);
                        usleep(1000);
                    }
                    else
                    {
                        send(sock, "STOP", strlen("STOP"), 0);
                        usleep(1000);
                        printf("No permission to write\n");
                    }
                }
                else
                {
                    send(sock, "Found", strlen("Found"), 0);
                    usleep(1000);
                    send(sock, node->storage_node->storage_ip, strlen(node->storage_node->storage_ip), 0);
                    usleep(1000);
                    char port_st_str[1024];
                    snprintf(port_st_str, sizeof(port_st_str), "%d", node->storage_node->storage_port_for_client);
                    send(sock, port_st_str, strlen(port_st_str), 0);
                    usleep(1000);
                    send(sock, "SELF", strlen("SELF"), 0);
                    usleep(1000);
                    insert_cache(search_path, node->storage_node);
                }
            }
            else
            {
                send(sock, "STOP", strlen("STOP"), 0);
                usleep(1000);
                printf("No such file or directory\n");
            }
        }
    }
    else if (a == 2)
    {
        usleep(1000);
        char function_exec[1024];
        int n1 = recv(sock, function_exec, sizeof(function_exec), 0);
        function_exec[n1] = '\0';
        printf("recv function -- %s\n", function_exec);

        if (strstr(function_exec, "DELETE") != NULL)
        {
            struct storage_node *srch_cache = search_cache(search_path);
            if (srch_cache != NULL)
            {
                send(sock, "Deleted", strlen("Deleted"), 0);
                usleep(1000);
                delete (search_path);
                delete_cache(search_path);
                give_command_to_nm(srch_cache->storage_ip, srch_cache->storage_port_for_NM, function_exec, search_path);
            }
            else
            {
                struct TrieNode *node = search(search_path, 0);
                if (node && node->isEndOfPath != 0)
                {
                    send(sock, "Deleted", strlen("Deleted"), 0);
                    delete (search_path);
                    give_command_to_nm(node->storage_node->storage_ip, node->storage_node->storage_port_for_NM, function_exec, search_path);
                }
                else
                {
                    send(sock, "No such file or directory", strlen("No such file or directory"), 0);
                    usleep(1000);
                    printf("No such file or directory\n");
                }
            }
        }
        else if (strstr(function_exec, "CREATE") != NULL)
        {
            printf("CREATE\n");
            printf("----%s----\n", search_path);
            char pp[1024];
            if (search_path[strlen(search_path) - 1] != '/')
            {
                strcpy(pp, search_path);
                for (int kk = strlen(pp); kk >= 0; kk--)
                {
                    if (pp[kk] == '/')
                    {
                        pp[kk + 1] = '\0';
                        break;
                    }
                }
            }
            else
            {
                int fff = 0;
                strcpy(pp, search_path);
                for (int kk = strlen(pp); kk >= 0; kk--)
                {
                    if (pp[kk] == '/')
                    {
                        if (fff == 0)
                        {
                            fff = 1;
                            continue;
                        }
                        pp[kk + 1] = '\0';
                        break;
                    }
                }
            }
            printf("----%s----\n", pp);
            struct storage_node *srch_cache = search_cache(pp);
            if (srch_cache != NULL)
            {
                insert_path(search_path, srch_cache);
                send(sock, "Path created", strlen("Path created"), 0);
                usleep(1000);
                give_command_to_nm(srch_cache->storage_ip, srch_cache->storage_port_for_NM, function_exec, search_path);
            }
            else
            {
                struct TrieNode *node = search(pp, 0);
                if (node && node->isEndOfPath != 0)
                {
                    struct storage_node *temp = (struct storage_node *)malloc(sizeof(struct storage_node));
                    temp->storage_id = node->storage_node->storage_id;
                    strcpy(temp->storage_ip, node->storage_node->storage_ip);
                    temp->storage_port_for_NM = node->storage_node->storage_port_for_NM;
                    temp->storage_port_for_client = node->storage_node->storage_port_for_client;
                    insert_cache(search_path, temp);
                    insert_path(search_path, temp);
                    send(sock, "Path created", strlen("Path created"), 0);
                    usleep(1000);
                    give_command_to_nm(temp->storage_ip, temp->storage_port_for_NM, function_exec, search_path);
                }
                else
                {
                    send(sock, "Path does not already exist", strlen("Path does not already exist"), 0);
                    usleep(1000);
                }
            }
        }
    }
    else if (a == 3)
    {
        send(sock, "Accepted", strlen("Accepted"), 0);
        char src_path[1024], dest_path[1024];
        char *token = strtok(search_path, "|");
        strcpy(src_path, token);
        token = strtok(NULL, "|");
        strcpy(dest_path, token);

        printf("%s | %s\n", src_path, dest_path);

        struct TrieNode *node = search(src_path, 0);
        printf("hibkb\n");
        if (node == NULL)
        {
            printf("No such file or directory for --%s--\n", src_path);
            return;
        }
        else if (node->isEndOfPath != 0)
        {
            char *src_ip = node->storage_node->storage_ip;
            int src_port = node->storage_node->storage_port_for_NM;

            struct TrieNode *dest_node = search(dest_path, 0);
            printf("hibkb\n");
            if (dest_node == NULL)
            {
                printf("No such file or directory for --%s--\n", dest_path);
                return;
            }
            else if (dest_node->isEndOfPath != 0)
            {
                char *dest_ip = dest_node->storage_node->storage_ip;
                int dest_port = dest_node->storage_node->storage_port_for_NM;

                struct sockaddr_in serv_addr;
                int sock_c = socket(AF_INET, SOCK_STREAM, 0);
                if (sock_c < 0)
                {
                    printf("Socket creation error\n");
                    return;
                }

                memset(&serv_addr, '0', sizeof(serv_addr));
                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = (dest_port);

                if (inet_pton(AF_INET, dest_ip, &serv_addr.sin_addr) <= 0)
                {
                    printf("Invalid address/ Address not supported\n");
                    return;
                }

                if (connect(sock_c, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
                {
                    printf("Connection Failed\n");
                    return;
                }

                send(sock_c, "COPY", strlen("COPY"), 0);
                usleep(1000);
                send(sock_c, src_ip, strlen(src_ip), 0);
                usleep(1000);
                char port_str[1024];
                snprintf(port_str, sizeof(port_str), "%d", src_port);
                send(sock_c, port_str, strlen(port_str), 0);
                usleep(1000);
                send(sock_c, src_path, strlen(src_path), 0);
                usleep(1000);
                send(sock_c, dest_path, strlen(dest_path), 0);
                usleep(1000);
                // close(sock_c);
            }
        }
    }
    else if (a == 4)
    {
        send(sock, "Exiting", strlen("Exiting"), 0);
        close(sock);
    }
}

void handle_client_connection(int client_socket)
{
    while (1)
    {
        // pthread_mutex_lock(&lock);
        char buffer[4096];
        char search_path[4096];
        char command[4096];
        char check_path[4096];
        ssize_t bytes_received;
        int flag = 0;
        int i = 0;
        while (1)
        {
            usleep(1000);
            char buffer[4096];
            bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
            if (bytes_received == 0)
            {
                continue;
            }
            buffer[bytes_received] = '\0';
            if (i == 0)
            {
                strcpy(command, buffer);
                printf("Received from client: %s\n", buffer);
                i++;
                if (atoi(command) == 4)
                {
                    break;
                }
                continue;
            }
            printf("Received from client: %s\n", buffer);
            strcpy(search_path, buffer);
            // strcpy(check_path, buffer);
            // struct TrieNode *node = search(check_path, 0);
            // if (node)
            // {
            //     send(client_socket, "Found", strlen("Found"), 0);
            //     usleep(1000);
            //     printf("Found\n");
            // }
            // else if (atoi(command) != 3)
            // {
            //     send(client_socket, "Not_Found", strlen("Not_Found"), 0);
            //     usleep(1000);
            //     printf("Not Found\n");
            //     flag = 1;
            // }
            if (i == 1)
            {
                break;
            }
        }
        if (atoi(command) == 4)
        {
            break;
        }
        if (flag == 1)
        {
            continue;
        }
        execute_client_request(command, search_path, client_socket);
        printf("NEW CLIENT REQUEST\n");
    }
    printf("Client disconnected\n");
    close(client_socket);
}

void *handle_client_connection_thread(void *arg)
{
    int client_socket = *((int *)arg);
    handle_client_connection(client_socket);
    close(client_socket);
    return NULL;
}

void *listen_client(void *vargp)
{
    int port = 5565;
    char *ip = "127.0.0.1";
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    socklen_t addr_size;
    char buffer[1024];
    int n;
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0)
    {
        perror("[-]Socket error");
        exit(1);
    }
    printf("[+]TCP server socket created.\n");
    memset(&server_addr, '\0', sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0)
    {
        perror("[-]Bind error");
        exit(1);
    }
    printf("[+]Bind to the port number: %d\n", port);
    listen(server_sock, 5);
    printf("Listening...\n");
    addr_size = sizeof(client_addr);
    int client_socket;
    int k = 0;
    while (1)
    {
        if ((client_socket = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len)) == -1)
        {
            perror("Error accepting connection");
            continue;
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        // Handle the connection in a new function
        pthread_t client_thread_id;
        pthread_create(&client_thread_id, NULL, handle_client_connection_thread, &client_socket);
    }
    return NULL;
}

int main()
{
    init_cache();
    server_is_present = (int *)malloc(sizeof(int) * 100);
    for (int i = 0; i < 100; i++)
    {
        server_is_present[i] = 0;
    }
    head = (struct ll *)malloc(sizeof(struct ll));
    root = getNode('*');
    //  pthread_mutex_init(&lock1, NULL);
    pthread_t listen_nm_thread_id;
    pthread_create(&listen_nm_thread_id, NULL, listen_nm_thread, NULL);

    pthread_t listen_client_thread_id;
    pthread_create(&listen_client_thread_id, NULL, listen_client, NULL);

    pthread_join(listen_nm_thread_id, NULL);
    // give_command_to_nm();
    pthread_join(listen_client_thread_id, NULL);
    return 0;
}