#include <stdbool.h>

typedef struct red_black_node RB_Node;
typedef struct red_black_tree RB_Tree;

/*Criar Red and Black tree*/
RB_Tree* create_RB_tree();

/*Funções da BST*/
bool insert_BS_tree(RB_Tree *tree, int entryKey);
void debug_print_prefix_BS_tree(RB_Tree *tree);

/*Funções da Red and Black Tree*/
bool kill_RB_tree(RB_Tree *tree);
void insert_RB_tree(RB_Tree *tree, int entryKey);
void search_RB_tree(RB_Tree *tree, int wantedKey);
void delete_RB_node(RB_Tree *tree, int entryKey);
int black_height(RB_Tree *tree);