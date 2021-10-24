#include <stdio.h>
#include <stdbool.h>
#include "red-and-black_tree.h"

int main(void){
    RB_Tree *arvore = NULL;
    int vetorTeste[11], i;
    arvore = create_RB_tree();

    vetorTeste[0] = 9;
    vetorTeste[1] = 4;
    vetorTeste[2] = 30;
    vetorTeste[3] = 27;
    vetorTeste[4] = 1;
    vetorTeste[5] = 17;
    vetorTeste[6] = 3;
    vetorTeste[7] = 2;
    vetorTeste[8] = 19;
    vetorTeste[9] = 5;
    vetorTeste[10] = 40;

    for(i=0; i<11; i++){
        insert_RB_tree(arvore, vetorTeste[i]);
    }

    /*debug_print_prefix_BS_tree(arvore);

    printf("\n\nDeletando o 9:\n");
    delete_RB_node(arvore, 1);

    debug_print_prefix_BS_tree(arvore);*/

    printf("Altura negra: %d\n", black_height(arvore));

    if(kill_RB_tree(arvore) == true){
        printf("\nCerto\n");
    }
    else{
        printf("\nErro\n");
    }

    return 0;
}