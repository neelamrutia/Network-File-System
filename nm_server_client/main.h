#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "strings.h"
#include "sys/types.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "netdb.h"
#include "arpa/inet.h"
#include "headers.h"
#define max_size 4096



extern int socket_naming_server;
extern int socket_storage_server[max_size];
extern int socket_client;

void *listen_client(void *vargp);
void *handle_client_connection_thread(void *arg);
void handle_client_connection(int client_socket);
void execute_client_request(char *command, char *search_path, int sock);
void copy_folder_parallely(char *src_ip, char *dest_ip, int src_port, int dest_port, char *src_path, char *dest_path, int src_server_number, int src_port_for_nm, int dest_port_for_nm);
void copy_parallely(char *src_ip, char *dest_ip, int src_port, int dest_port, char *src_path, char *dest_path, int src_server_number, int src_port_for_nm, int dst_port_for_nm);
void give_command_to_nm(char *ip, int port, char *function, char *search_path);
void *listen_nm_thread(void *args);
void *handle_storage_connection_thread(void *arg);
void handle_storage_connection(int client_socket);
void replicant_function(char **path_storage, int count_path, struct storage_node *temp, struct storage_node *temp_copy);
void client_connection(int client_sock);
int det_index(char *path);
void init_cache();
void insert_cache(char *path, struct storage_node *st);
struct storage_node *search_cache(char *path);
struct TrieNode *getNode(char current_char);
void insert_path(const char *path, struct storage_node *storage_node);
void print_trie(struct TrieNode *root);
struct TrieNode *search_path(struct TrieNode *root, char *path, int path_len);
struct TrieNode *search(char *path, int path_len);
int delete_node(struct TrieNode *node, char *path, int current);
void delete_recursive(struct TrieNode *node);
int delete_directory(struct TrieNode *node);