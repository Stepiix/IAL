/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
  if(tree != NULL){
    (*tree) = NULL;
  }
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  bool search = false;
  bst_node_t *rootPtr = tree;
  while(search == false && rootPtr != NULL){
    if(rootPtr->key > key){
      rootPtr = rootPtr->left;
    } 
    else if(rootPtr->key < key){
      rootPtr = rootPtr->right;
    } 
    else if(rootPtr->key == key){
      *value = rootPtr->value;
      search = true;
    }
  }
  return search;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
  if(tree == NULL){
    return;
  }
  bst_node_t *rootPtr = *tree;
  bst_node_t *prevrootPtr = NULL;
  while(rootPtr != NULL){
    prevrootPtr = rootPtr;
    if(rootPtr->key == key){
      rootPtr->value = value;
      return;
    }
    else if(rootPtr->key > key){
      rootPtr = rootPtr->left;
    }
    else if(rootPtr->key < key){
      rootPtr = rootPtr->right; 
    }
  }
  bst_node_t *newPtr = malloc(sizeof(bst_node_t));
  if(newPtr == NULL){
    return;
  }
  newPtr->key = key;
  newPtr->value = value;
  newPtr->left = NULL;
  newPtr->right = NULL;

  if(prevrootPtr == NULL){
    (*tree) = newPtr;
  }
  else if(prevrootPtr->key > key){
    prevrootPtr->left = newPtr;
  } 
  else if(prevrootPtr->key < key){
    prevrootPtr->right = newPtr;
  }

}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if(tree == NULL){
    return;
  }
  bst_node_t *save = NULL;
  while((*tree)->right != NULL){
    tree = &(*tree)->right;
  }
  target->key = (*tree)->key;
  target->value = (*tree)->value;
  save = (*tree)->left;
  free(*tree);
  if(save != NULL){
    *tree = save;
    return;
  } 
    *tree = NULL;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */

void bst_delete(bst_node_t **tree, char key) {
  if(tree == NULL){
    return;
  }
  
  bool search = false;
  bst_node_t *searchrootPtr = *tree;

  while(search == false && searchrootPtr != NULL){
    if(searchrootPtr->key > key){
      searchrootPtr = searchrootPtr->left;
    } 
    else if(searchrootPtr->key < key){
      searchrootPtr = searchrootPtr->right;
    } 
    else if(searchrootPtr->key == key){
      search = true;
    }
  }

  bst_node_t *rootPtr = *tree;
  bst_node_t *prevrootPtr = NULL;
  if(search == false){
    return;
  }

  while(rootPtr->key != key){
    prevrootPtr = rootPtr;
    if(rootPtr->key < key){
      rootPtr = rootPtr->right;
    }
    else{
      rootPtr = rootPtr->left;
    }
  }

  if(rootPtr->left != NULL && rootPtr->right == NULL){
    if(prevrootPtr == NULL){
      *tree = rootPtr->left;
    }
    else if(prevrootPtr->left == rootPtr){
      prevrootPtr->left = rootPtr->left;
    } 
    else if(prevrootPtr->right == rootPtr){
      prevrootPtr->right = rootPtr->left;
    }
  }

  else if(rootPtr->left == NULL && rootPtr->right != NULL){
    if(prevrootPtr == NULL){
      *tree = rootPtr->right;
    }
    else if(prevrootPtr->left == rootPtr){
      prevrootPtr->left = rootPtr->right;
    }
    else if(prevrootPtr->right == rootPtr){
      prevrootPtr->right = rootPtr->right;
    }
  }

  else if(rootPtr->left != NULL && rootPtr->right != NULL){
    bst_replace_by_rightmost(rootPtr, &(rootPtr->left)); 
    return;
  }

  else if(rootPtr->left == NULL && rootPtr->right == NULL){
    if(prevrootPtr == NULL){
      *tree = NULL;
    }
    else if(prevrootPtr->left == rootPtr){
      prevrootPtr->left = NULL;
    }
    else if(prevrootPtr->right == rootPtr){
      prevrootPtr->right = NULL;
    }
  }
  free(rootPtr);
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
  if(tree == NULL){
    return;
  }
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  stack_bst_init(stack);
  bst_node_t *rootPtr = *tree;
  do{
    if(rootPtr == NULL){
      if((stack_bst_empty(stack)) == false){
        rootPtr = stack_bst_pop(stack);
      }
    } else {
      if(rootPtr->right != NULL){
        stack_bst_push(stack, rootPtr->right);
      }
      bst_node_t *next = rootPtr;
      rootPtr = rootPtr->left;
      free(next);
    }

  } while(rootPtr != NULL || (stack_bst_empty(stack)) == false);
  *tree = NULL;
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    bst_print_node(tree);
    tree = tree->left;
  }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL){
    return;
  }
  stack_bst_init(stack);
  bst_leftmost_preorder(tree, stack);
  while((stack_bst_empty(stack)) == false){
    bst_node_t *tmp = stack_bst_pop(stack);
    bst_leftmost_preorder(tmp->right, stack);
  }
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit,tree);
    tree = tree->left;
  }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL){
    return;
  }
  stack_bst_init(stack);
  bst_leftmost_inorder(tree,stack);
  while((stack_bst_empty(stack)) == false){
    tree = stack_bst_pop(stack);
    bst_print_node(tree);
    bst_leftmost_inorder(tree->right,stack);
  }
  free(stack);
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
  while(tree != NULL){
    stack_bst_push(to_visit, tree);
    stack_bool_push(first_visit,true);
    tree = tree->left;
  }
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  bool fLeft;
  stack_bst_t *stack = malloc(sizeof(stack_bst_t));
  if(stack == NULL){
    return;
  }
  stack_bst_init(stack);
  stack_bool_t *boolStack = malloc(sizeof(stack_bool_t));
  if(boolStack == NULL){
    return;
  }
  stack_bool_init(boolStack);
  bst_leftmost_postorder(tree,stack,boolStack);

  while((stack_bst_empty(stack)) == false){
    tree = stack_bst_top(stack);
    fLeft = stack_bool_pop(boolStack);
    if(fLeft){
      stack_bool_push(boolStack,false);
      bst_leftmost_postorder(tree->right,stack,boolStack);
    } else {
      stack_bst_pop(stack);
      bst_print_node(tree);
    }
  }
  free(stack);
  free(boolStack);
}
