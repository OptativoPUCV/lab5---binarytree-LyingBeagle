#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    //new->lower_than = lower_than;

    TreeMap* map = (TreeMap*)malloc(sizeof(TreeMap));

    if(map == NULL) return NULL;

    map->root = NULL;
    map->current = NULL;
    map->lower_than = lower_than;
    
    return map;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  
    TreeNode* current = tree->root;
    TreeNode* padre = NULL;

    while(current != NULL){

        padre = current;
      
        if(is_equal(tree, current->pair->key, key)){
          
            tree->current = current;
            return;
          
        }else if(tree->lower_than(current->pair->key, key)){
          
            current = current->right;
          
        }else{
          
            current = current->left; 
        }
    }

    TreeNode* nuevoNodo = createTreeNode(key, value);
    nuevoNodo->parent = padre;

    if(padre == NULL){
        tree->root = nuevoNodo;
    }
    else if(tree->lower_than(padre->pair->key, key)){
        padre->right = nuevoNodo;
    }
    else{
        padre->left = nuevoNodo;
    }


    tree->current = nuevoNodo;
}

TreeNode * minimum(TreeNode * x){

    TreeNode* current = x;

    if(current->left == NULL) return x;

    while(current->left != NULL){

      current = current->left;
      
    }

    return current;
}


void removeNode(TreeMap * tree, TreeNode* node) {

    if(node->left == NULL && node->right == NULL){
        if(node->parent == NULL){
          tree->root = NULL;
        }
        else if(node == node->parent->left){
            node->parent->left = NULL;
        }
        else{
            node->parent->right = NULL;
        }
        free(node);
    }
    else if(node->left == NULL){
      TreeNode* hijo = node->right;
      node->parent->left = hijo;

      hijo->parent = node->parent;
      free(node);
    }
    else if(node->right == NULL){
      TreeNode* hijo = node->left;
      node->parent->right = hijo;

      hijo->parent = node->parent;
      free(node);
    }
    else{
        TreeNode* NodoMinumum = minimum(node->right);
        node->pair->key = NodoMinumum->pair->key;
        node->pair->value = NodoMinumum->pair->value;
        node->parent = NodoMinumum->parent;
        removeNode(tree, NodoMinumum);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {

    TreeNode* current = tree->root;  

    while(current != NULL){

      if(is_equal(tree, current->pair->key, key)){
          tree->current = current;
          return current->pair;
      }

      if(tree->lower_than(current->pair->key, key)){
          current = current->right;
      }
      else{
          current = current->left;
      }
    }
  
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {

    TreeNode* current = tree->root;
    TreeNode* ub_node = NULL;

    while(current != NULL){

      if(tree->lower_than(current->pair->key, key)){
          current = current->right;
      }
      else if(is_equal(tree, current->pair->key, key)){
          return current->pair;
      }
      else{
          ub_node = current;
          current = current->left;
      }      
    }

    if(ub_node != NULL){
        tree->current = ub_node;
        return ub_node->pair;
    }

    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {

    TreeNode* NodoMinimo = minimum(tree->root);

    tree->current = NodoMinimo;

    return NodoMinimo->pair;
}

Pair * nextTreeMap(TreeMap * tree) {

    if(tree->current == NULL) return NULL;

    TreeNode* current = tree->current;
    TreeNode* sucesor = NULL;

    if(current->right != NULL){
        sucesor = minimum(current->right);
    }
    else{
        TreeNode* antecesor = current->parent;

        while(antecesor != NULL && current == antecesor->right){
            current = antecesor;
            antecesor = antecesor->parent;
        }
        sucesor = antecesor;
    }

    tree->current = sucesor;
  
    return (sucesor != NULL) ? sucesor->pair : NULL;
}
