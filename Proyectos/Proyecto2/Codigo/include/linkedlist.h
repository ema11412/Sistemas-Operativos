/* llist.h
 * Generic Linked List
 */

struct node {
    void *data;
    int channelTime;
    int priotity;
    struct node *next;
};

typedef struct node * llist;

/* llist_create: Create a linked list */
llist *llist_create(void *data);

/* llist_free: Free a linked list */
void llist_free(llist *list);

/* llist_add_inorder: Add to sorted linked list */
int llist_add_inorder(void *data, llist *list, 
                       int (*comp)(void *, void *));

/* llist_push: Add to head of list */
void llist_push(llist *list, void *data);

/* llist_pop: remove and return head of linked list */
void *llist_pop(llist *list);

/* llist_print: print linked list */
void llist_print(llist *list, void (*print)(void *data));

/* llist_getbyId: get item of linked list by Id */
void *llist_getbyId(llist *list, int Id);

/* llist_addLast: add item to tale of linked list */
void llist_addLast(llist *list, void *data);

/* llist_delById: delete item of linked list serched by Id */
int llist_delById(llist *list, int Id);

/* llist_getSize: get size of linked list */
int llist_getSize(llist *list);

/* llist_addById: add item to linked list in a specific pos */
int llist_addById(llist *list,void *data, int Id);

llist* SJF_SortingAlgorithm(llist* readyQueue);
void swap(struct node* a, struct node* b);
void Priority_SortingAlgorithm(llist* readyQueue);
void swap2(struct node* a, struct node* b);