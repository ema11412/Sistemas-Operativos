/* llist.c
 * Generic Linked List implementation
 */

#include <stdlib.h>
#include <stdio.h>
#include "../../include/linkedlist.h"

llist *llist_create(void *new_data)
{
    struct node *new_node;

    llist *new_list = malloc(sizeof (llist));
    *new_list = malloc(sizeof (struct node));
    
    new_node = *new_list;
    new_node->data = new_data;
    new_node->next = NULL;
    return new_list;
}

void llist_free(llist *list)
{
    struct node *curr = *list;
    struct node *next;

    while (curr != NULL) {
        next = curr->next;
        free(curr);
        curr = next;
    }

    free(list);
}

// Returns 0 on failure
int llist_add_inorder(void *data, llist *list,
                       int (*comp)(void *, void *))
{
    struct node *new_node;
    struct node *curr;
    struct node *prev = NULL;
    
    if (list == NULL || *list == NULL) {
        fprintf(stderr, "llist_add_inorder: list is null\n");
        return 0;
    }
    
    curr = *list;
    if (curr->data == NULL) {
        curr->data = data;
        return 1;
    }

    new_node = (struct node *)malloc(sizeof (struct node));
    new_node->data = data;

    // Find spot in linked list to insert new node
    while (curr != NULL && curr->data != NULL && comp(curr->data, data) < 0) {
        prev = curr;
        curr = curr->next;
    }
    new_node->next = curr;

    if (prev == NULL) 
        *list = new_node;
    else 
        prev->next = new_node;

    return 1;
}

void llist_push(llist *list, void *data)
{
    struct node *head;
    struct node *new_node;
    if (list == NULL || *list == NULL) {
        fprintf(stderr, "llist_add_inorder: list is null\n");
    }

    head = *list;
    
    // Head is empty node
    if (head->data == NULL)
        head->data = data;

    // Head is not empty, add new node to front
    else {
        new_node = malloc(sizeof (struct node));
        new_node->data = data;
        new_node->next = head;
        *list = new_node;
    }
}

void *llist_pop(llist *list)
{
    void *popped_data;
    struct node *head = *list;

    if (list == NULL || head->data == NULL)
        return NULL;
    
    popped_data = head->data;
    *list = head->next;

    free(head);

    return popped_data;
}

void llist_print(llist *list, void (*print)(void *))
{
    struct node *curr = *list;
    while (curr != NULL) {
        print(curr->data);
        printf(" ");
        curr = curr->next;
    }
    putchar('\n');
}


void *llist_getbyId(llist *list, int Id)
{
    void *popped_data;
    struct node *curr = *list;

    if (llist_getSize(list)-1 < Id){
      return NULL;
    }

    for(int i = 0; i < Id; i++){
      if (list == NULL || curr->data == NULL)
        return NULL;
      curr = curr->next;
    }
    popped_data = curr->data;

    return popped_data;
}

void llist_addLast(llist *list, void *data)
{
    struct node *curr = *list;
    struct node *new_node;
    if (list == NULL || *list == NULL) {
      *list = malloc(sizeof (struct node));
      new_node = *list;
      new_node->data = data;
      new_node->next = NULL;
    }
    
    // Head is empty node
    else if (curr->data == NULL)
        curr->data = data;
        
    // Head is not empty, add new node to tail
    else {
      while (curr->next != NULL) {
          curr = curr->next;
      }
      new_node = malloc(sizeof (struct node));
      new_node->data = data;
      new_node->next = NULL;
      curr->next = new_node;
    }
}

int llist_getSize(llist *list)
{
  int count = 0;
  struct node *curr = *list;

  if (list == NULL || *list == NULL) {
    return 0;
  }

  if(curr->data == NULL) {
    return 0;
  }

  while (curr != NULL){
    curr = curr->next;
    count++;
  }
  return count;
}

int llist_delById(llist *list, int Id)
{
    struct node *curr = *list;
    struct node *temp = *list;
    if (list == NULL || *list == NULL) {
      return 1;
    }

    if (llist_getSize(list)-1 < Id){
      return 1;
    }

    if (Id == 0){
      *list = curr->next;
      return 0;
    }
    
    for(int i = 0; i < Id-1; i++){
      if (list == NULL || curr->next == NULL)
        return 1;
      curr = curr->next;
      temp = temp->next;
    }

    temp = temp->next;
    curr->next = temp->next;

    free(temp);
    return 0;
}

int llist_addById(llist *list,void *data, int Id)
{
    struct node *curr = *list;
    struct node *new_node;
    if (list == NULL || *list == NULL) {
      return 1;
    }

    if (llist_getSize(list)-1 < Id){
      llist_addLast(list,data);
    }
    
    // Head is empty node
    if (curr->data == NULL)
        curr->data = data;
    
    if(Id == 0)
      llist_push(list,data);
        
    // Head is not empty, add new node to tail
    else {
      for(int i = 0; i < Id-1; i++){
        if (list == NULL || curr->next == NULL)
          return 1;
        curr = curr->next;
      }
      new_node = malloc(sizeof (struct node));
      new_node->data = data;
      new_node->next = curr->next;
      curr->next = new_node;
    }
    return 0;
}

llist* SJF_SortingAlgorithm(llist* readyQueue){

    int swapped, i;
    struct node* start = readyQueue;
    struct node* ptr1;
    struct node* lptr = NULL;

  
    /* Checking for empty list */
    if (llist_getSize(readyQueue) != 0 ){
        return readyQueue;
    }

    do
    {
        swapped = 0;
        ptr1 = start;
  
        while (ptr1->next != lptr)
        {
            if (ptr1->channelTime > ptr1->next->channelTime)
            { 
                swap(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);

}

void Priority_SortingAlgorithm(llist* readyQueue){

    int swapped, i;
    struct node* start = readyQueue;
    struct node* ptr1;
    struct node* lptr = NULL;

  
    /* Checking for empty list */
    if (readyQueue == NULL ){
        return;
    }

    do
    {
        swapped = 0;
        ptr1 = start;
  
        while (ptr1->next != lptr)
        {
            if (ptr1->priotity > ptr1->next->priotity)
            { 
                swap2(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    }
    while (swapped);

}

void swap(struct node* a, struct node* b)
{
    int temp = a->channelTime;
    a->channelTime = b->channelTime;
    b->channelTime = temp;
}

void swap2(struct node* a, struct node* b)
{
    int temp = a->priotity;
    a->priotity = b->priotity;
    b->priotity = temp;
}