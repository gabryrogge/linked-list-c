#include <stdio.h>
#include <stdlib.h>

/* ─── Node structure ─────────────────────────────────────── */
typedef struct Node {
    int data;
    struct Node *next;
    struct Node *prev;
} Node;

/* ─── List structure ─────────────────────────────────────── */
typedef struct {
    Node *head;
    Node *tail;
    int   size;
} LinkedList;

/* ─── Create a new empty list ───────────────────────────── */
LinkedList *list_create(void) {
    LinkedList *list = malloc(sizeof(LinkedList));
    if (!list) { perror("malloc"); exit(EXIT_FAILURE); }
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

/* ─── Create a new node ─────────────────────────────────── */
static Node *node_create(int data) {
    Node *n = malloc(sizeof(Node));
    if (!n) { perror("malloc"); exit(EXIT_FAILURE); }
    n->data = data;
    n->next = NULL;
    n->prev = NULL;
    return n;
}

/* ─── Insert at head ────────────────────────────────────── */
void list_push_front(LinkedList *list, int data) {
    Node *n = node_create(data);
    if (list->head == NULL) {
        list->head = list->tail = n;
    } else {
        n->next = list->head;
        list->head->prev = n;
        list->head = n;
    }
    list->size++;
}

/* ─── Insert at tail ────────────────────────────────────── */
void list_push_back(LinkedList *list, int data) {
    Node *n = node_create(data);
    if (list->tail == NULL) {
        list->head = list->tail = n;
    } else {
        n->prev = list->tail;
        list->tail->next = n;
        list->tail = n;
    }
    list->size++;
}

/* ─── Insert at position (0-indexed) ───────────────────── */
void list_insert_at(LinkedList *list, int data, int pos) {
    if (pos <= 0) { list_push_front(list, data); return; }
    if (pos >= list->size) { list_push_back(list, data); return; }
    Node *curr = list->head;
    for (int i = 0; i < pos - 1; i++) curr = curr->next;
    Node *n = node_create(data);
    n->next = curr->next;
    n->prev = curr;
    if (curr->next) curr->next->prev = n;
    curr->next = n;
    list->size++;
}

/* ─── Delete first node with given value ───────────────── */
int list_delete(LinkedList *list, int data) {
    Node *curr = list->head;
    while (curr) {
        if (curr->data == data) {
            if (curr->prev) curr->prev->next = curr->next;
            else            list->head = curr->next;
            if (curr->next) curr->next->prev = curr->prev;
            else            list->tail = curr->prev;
            free(curr);
            list->size--;
            return 1;
        }
        curr = curr->next;
    }
    return 0; /* not found */
}

/* ─── Search ────────────────────────────────────────────── */
Node *list_search(LinkedList *list, int data) {
    Node *curr = list->head;
    while (curr) {
        if (curr->data == data) return curr;
        curr = curr->next;
    }
    return NULL;
}

/* ─── Print forward ─────────────────────────────────────── */
void list_print(LinkedList *list) {
    printf("[HEAD] ");
    Node *curr = list->head;
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->next;
    }
    printf("[TAIL]  (size=%d)\n", list->size);
}

/* ─── Print backward ────────────────────────────────────── */
void list_print_reverse(LinkedList *list) {
    printf("[TAIL] ");
    Node *curr = list->tail;
    while (curr) {
        printf("%d ", curr->data);
        curr = curr->prev;
    }
    printf("[HEAD]\n");
}

/* ─── Free all memory ───────────────────────────────────── */
void list_destroy(LinkedList *list) {
    Node *curr = list->head;
    while (curr) {
        Node *tmp = curr->next;
        free(curr);
        curr = tmp;
    }
    free(list);
}

/* ─── Main: test suite ──────────────────────────────────── */
int main(void) {
    LinkedList *list = list_create();

    printf("=== Push back: 10, 20, 30 ===\n");
    list_push_back(list, 10);
    list_push_back(list, 20);
    list_push_back(list, 30);
    list_print(list);

    printf("=== Push front: 5 ===\n");
    list_push_front(list, 5);
    list_print(list);

    printf("=== Insert 15 at position 2 ===\n");
    list_insert_at(list, 15, 2);
    list_print(list);

    printf("=== Print reverse ===\n");
    list_print_reverse(list);

    printf("=== Search 20: %s ===\n",
           list_search(list, 20) ? "found" : "not found");
    printf("=== Search 99: %s ===\n",
           list_search(list, 99) ? "found" : "not found");

    printf("=== Delete 15 ===\n");
    list_delete(list, 15);
    list_print(list);

    printf("=== Delete head (5) ===\n");
    list_delete(list, 5);
    list_print(list);

    printf("=== Delete tail (30) ===\n");
    list_delete(list, 30);
    list_print(list);

    list_destroy(list);
    printf("=== Memory freed. Done. ===\n");
    return 0;
}
