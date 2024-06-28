#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdbool.h>
#define MAX_STORAGE_SERVERS 20


extern int* server_is_present;
extern int num_of_storage_servers;
struct storage_node
{
    int storage_id;
    char storage_ip[16];
    int storage_port_for_NM;
    int storage_port_for_client;
    struct storage_node *replicate1;
    struct storage_node *replicate2;
    int is_read;
};

struct TrieNode
{
    struct TrieNode *children[128];
    char current_char;
    int isEndOfPath;
    int valid;
    int isDirectory;
    struct storage_node *storage_node;
};

extern struct TrieNode *trie_root;
extern struct storage_node *storage_server;

// struct TrieNode *getNode(char current_char);
// void insert_path(const char *path, struct storage_node *storage_node);
// void print_trie(struct TrieNode *root);
// struct TrieNode *search(char *path, int path_len);
// struct TrieNode *search_path(struct TrieNode *root, char *path, int path_len);
// int delete_node(struct TrieNode *node, char *path, int current);
// int delete(char *path);
// void client_connection(int client_sock);
// void handle_storage_connection(int client_socket);
// void *listen_nm_thread(void *args);



