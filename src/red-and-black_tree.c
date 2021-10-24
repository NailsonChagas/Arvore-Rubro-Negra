#define INT_MIN -2147483648

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "red-and-black_tree.h"

struct red_black_node{
    int key;
    bool red;
    RB_Node *father, *left, *right;
};

struct red_black_tree{
    RB_Node *root;
    RB_Node *NIL;//vai ser as folhas, devem ser pretas e nulas
};

RB_Tree* create_RB_tree(){//funciona
    RB_Tree *new = NULL;
    RB_Node *leaf = NULL;

    leaf = (RB_Node*)malloc(sizeof(RB_Node));
    leaf->left = leaf->right = leaf->father = NULL;
    leaf->red = false;
    leaf->key = 0;

    new = (RB_Tree*)malloc(sizeof(RB_Tree));
    new->NIL = leaf;
    new->root = new->NIL;

    return new;
}

static RB_Node* create_RB_node(int entryKey){//funciona
    RB_Node *new = NULL;

    new = (RB_Node*)malloc(sizeof(RB_Node));
    new->key = entryKey;
    new->red = true;
    new->father = new->left = new->right = NULL;

    return new;
}

static void print_RB_node(RB_Node *node){//funciona
    if(node != NULL){
        printf("-----------------------------------------------------\n");
        printf("Node memory address: %p\n", node);
        printf("\tKey: %d\n", node->key);

        if(node->red == true){
            printf("\tRed: true\n");
        }
        else{
            printf("\tRed: false\n");
        }

        printf("\tFather: %p\n", node->father);
        printf("\tLeft: %p | Right: %p\n", node->left, node->right);
    }
}

static void debug_prefix(RB_Tree *tree, RB_Node *root){//funciona
    if(root != tree->NIL){
        print_RB_node(root);
        debug_prefix(tree, root->left);
        debug_prefix(tree, root->right);
    }
}

void debug_print_prefix_BS_tree(RB_Tree *tree){//funciona
    if(tree != NULL){
        debug_prefix(tree, tree->root);
    }
}

/*Inicio funções para procura, balanceamento, inserção e remoção em uma árvore RB*/
//lembretes para depois:
//em caso de duvidas: https://www.youtube.com/playlist?list=PL9xmBV_5YoZNqDI8qfOZgzbqahCUmUEin
//ou ainda: https://www.codesdope.com/course/data-structures-red-black-trees-insertion/
//no código do professor 0: vermelho | 1: preto

static void kill_RB(RB_Tree *tree, RB_Node *root){//funciona
    if(root != tree->NIL){
        kill_RB(tree, root->left);
        kill_RB(tree, root->right);
        free(root);
    }
}

bool kill_RB_tree(RB_Tree *tree){//não funciona depois de eu utilizar a delete_RB_node
    if(tree != NULL){
        if(tree->root != tree->NIL){
            kill_RB(tree, tree->root);
            free(tree->NIL);
            free(tree);

            return true;
        }
        else{
            free(tree->NIL);
            free(tree);

            return true;
        }
    }
    return false;
}

static RB_Node* search_RB(RB_Tree *tree, RB_Node *root, int wantedKey){//FUNCIONA
    if(root != tree->NIL){
        if(root->key == wantedKey){
            return root;
        }
        else if(wantedKey < root->key){
            return search_RB(tree, root->left, wantedKey);
        }
        else{
            return search_RB(tree, root->right, wantedKey);
        }
    }
    return NULL;
}

void search_RB_tree(RB_Tree *tree, int wantedKey){//funciona
    if(tree != NULL){
        RB_Node *searched = search_RB(tree, tree->root, wantedKey);
        print_RB_node(searched);
    }
}

static void left_rotate(RB_Tree *tree, RB_Node *nodeX){//funciona
    RB_Node *y = nodeX->right;

    nodeX->right = y->left;

    if(y->left != tree->NIL){
        y->left->father = nodeX;
    }

    y->father = nodeX->father;

    if(nodeX->father == tree->NIL){//nodeX é raiz
        tree->root = y;
    }
    else if(nodeX == nodeX->father->left){//nodeX esta a esquerda
        nodeX->father->left = y;
    }
    else{//nodeX esta a direita
        nodeX->father->right = y;
    }

    y->left = nodeX;
    nodeX->father = y;
}

static void right_rotate(RB_Tree *tree, RB_Node *nodeX){//funciona
    RB_Node *y = nodeX->left;

    nodeX->left = y->right;

    if(y->right != tree->NIL){
        y->right->father = nodeX;
    }

    y->father = nodeX->father;

    if(nodeX->father == tree->NIL){
        tree->root = y;
    }
    else if(nodeX == nodeX->father->right){
        nodeX->father->right = y;
    }
    else{
        nodeX->father->left = y;
    }

    y->right = nodeX;
    nodeX->father = y;
}

static void insertion_balance(RB_Tree *tree, RB_Node *nodeZ){//funciona
    while(nodeZ->father->red == true){
        if(nodeZ->father == nodeZ->father->father->left){//pai de z a esquerda
            RB_Node *y = nodeZ->father->father->right; //tio de z

            if(y->red == true){//caso 1
                nodeZ->father->red = false;
                y->red = false;
                nodeZ->father->father->red = true;
                nodeZ = nodeZ->father->father;
            }
            else{//caso 2 e 3
                if(nodeZ == nodeZ->father->right){//caso 2
                    nodeZ = nodeZ->father;
                    left_rotate(tree, nodeZ);
                }
                //case 3
                nodeZ->father->red = false;
                nodeZ->father->father->red = true;
                right_rotate(tree, nodeZ->father->father);
            }
        }
        else{//pai de z a direita
            RB_Node *y = nodeZ->father->father->left; //tio de z

            if(y->red == true){
                nodeZ->father->red = false;
                y->red = false;
                nodeZ->father->father->red = true;
                nodeZ = nodeZ->father->father;
            }
            else{
                if(nodeZ == nodeZ->father->left){
                    nodeZ = nodeZ->father;
                    right_rotate(tree, nodeZ);
                }

                nodeZ->father->red = false;
                nodeZ->father->father->red = true;
                left_rotate(tree, nodeZ->father->father);
            }
        }
    }
    tree->root->red = false;
}

void insert_RB_tree(RB_Tree *tree, int entryKey){//funciona
    RB_Node *new = create_RB_node(entryKey);
    RB_Node *newFather = tree->NIL, *aux = tree->root;

    while(aux != tree->NIL){
        newFather = aux;

        if(new->key < aux->key){
            aux = aux->left;
        }
        else{
            aux = aux->right;
        }
    }

    new->father = newFather;

    if(newFather == tree->NIL){//nó adicionado é a raiz
        tree->root = new;
    }
    else if(new->key < newFather->key){//nó vai estar a esqueda
        newFather->left = new;
    }
    else{
        newFather->right = new;
    }

    new->right = tree->NIL;
    new->left = tree->NIL;

    insertion_balance(tree, new);
}

//lembretes para depois:
//transplant: code to transplant a subtree rooted at node v in place of the subtree rooted at node u
//Se tiver duvidas na transplant:
//https://www.codesdope.com/course/data-structures-binary-search-trees/
//https://www.codesdope.com/course/data-structures-red-black-trees-deletion/

static void transplant_RB(RB_Tree *tree, RB_Node *toBeRemoved, RB_Node *subtreeTransplanted){//funciona
    if(toBeRemoved == tree->NIL){//toBeRemoved é raiz
        tree->root = subtreeTransplanted;
    }
    else if(toBeRemoved == toBeRemoved->father->left){//se toBeRemoved estiver a esquerda
        toBeRemoved->father->left = subtreeTransplanted;
    }
    else{//toBeRemoved a direita
        toBeRemoved->father->right = subtreeTransplanted;
    }
    subtreeTransplanted->father = toBeRemoved->father;
}

static RB_Node* minimum_RB_node(RB_Tree *tree, RB_Node *nodeX){//funciona
    while(nodeX->left != tree->NIL){
        nodeX = nodeX->left;
    }
    return nodeX;
}

static void delete_balance(RB_Tree *tree, RB_Node *nodeX){//funciona
    while((nodeX != tree->root) && (nodeX->red == false)){
        if(nodeX == nodeX->father->left){//nodeX a esquerda
            RB_Node *y = nodeX->father->right; //tio de nodeX

            if(y->red == true){
                y->red = false;
                nodeX->father->red = true;
                left_rotate(tree, nodeX->father);
                y = nodeX->father->right;
            }

            if(y->left->red == false && y->right->red == false){
                y->red = true;
                nodeX = nodeX->father;
            }
            else{
                if(y->right->red == false){
                    y->left->red = false;
                    y->red = true;
                    right_rotate(tree, y);
                    y = nodeX->father->right;
                }
                y->red = nodeX->father->red;
                nodeX->father->red = false;
                y->father->red = false;
                left_rotate(tree, nodeX->father);
                nodeX = tree->root;
            }
        }
        else{//nodeX a direita
            RB_Node *y = nodeX->father->left; //tio de nodeX

            if(y->red == true){
                y->red = false;
                nodeX->father->red = true;
                right_rotate(tree, nodeX->father);
                y = nodeX->father->left;
            }

            if((y->right->red == false) && (y->left->red == false)){
                y->red = true;
                nodeX = nodeX->father;
            }
            else{
                if(y->left->red == false){
                    y->right->red = false;
                    y->red = true;
                    left_rotate(tree, y);
                    y = nodeX->father->left;
                }
                y->red = nodeX->father->red;
                nodeX->father->red = false;
                y->left->red = false;
                right_rotate(tree, nodeX->father);
                nodeX = tree->root;
            }
        }
    }
    nodeX->red = false;
}

void delete_RB_node(RB_Tree *tree, int entryKey){//funciona
    RB_Node *toBeRemoved = search_RB(tree, tree->root, entryKey);
    RB_Node *auxA = toBeRemoved, *auxB = NULL;
    bool originalColor = auxA->red;

    if(toBeRemoved->left == tree->NIL){
        auxB = toBeRemoved->right;
        transplant_RB(tree, toBeRemoved, toBeRemoved->right);
    }
    else if(toBeRemoved->right == tree->NIL){
        auxB = toBeRemoved->left;
        transplant_RB(tree, toBeRemoved, toBeRemoved->left);    
    }
    else{//toBeRemoved tem dois descendentes
        auxA = minimum_RB_node(tree, toBeRemoved->right);
        originalColor = auxA->red;
        auxB = auxA->right;

        if(auxA->father == toBeRemoved){
            auxB->father = toBeRemoved;
        }
        else{
            transplant_RB(tree, auxA, auxA->right);
            auxA->right = toBeRemoved->right;
            auxA->right->father = auxA;
        }

        transplant_RB(tree, toBeRemoved, auxA);
        auxA->left = toBeRemoved->left;
        auxA->left->father = auxA;
        auxA->red = toBeRemoved->red;
    }

    if(originalColor == false){
        delete_balance(tree, auxB);
    }

    free(toBeRemoved);
}

int black_height(RB_Tree *tree){
    int blackHeight = INT_MIN;
    
    if(tree != NULL){
        RB_Node *aux = tree->root;

        blackHeight = 0;

        while(aux != tree->NIL){
            if(aux->red == false){
                blackHeight += 1;
            }
            aux = aux->left;
        }
    }
    return blackHeight;
}