// Ariel Towne
// ar217413

#include <stdio.h>
#include <stdlib.h>
#include "KindredSpirits.h"

// From Szumlanski's "LinkedLists.c"
typedef struct Node
{
	int data;
	struct Node *next;
} Node;

// From Szumlanski's "LinkedLists.c"
typedef struct LinkedList
{
	Node *head;
	Node *tail;
} LinkedList;

// From Szumlanski's "Queue.c"
typedef struct Queue
{
	LinkedList *list;
	int size;
} Queue;


// From Szumlanski's "bst.c"
node *create_binaryNode(int data)
{
    node *n = calloc(1, sizeof(node));
    n->data = data;
    return n;
}

// From Szumlanski's "LinkedLists.c"
Node *create_Listnode(int data)
{
	Node *n = malloc(sizeof(Node));
	n->data = data;
	n->next = NULL;
	return n;
}

// Based on Szumlanski's "Queue.c"
Queue *create_queue(void)
{
	Queue *q = malloc(sizeof(Queue));
	q->list = calloc(1, sizeof(LinkedList));
	q->size = 0;
	return q;
}

// From Szumlanski's "LinkedLists.c".
void tail_insert(LinkedList *list, int data)
{
	if (list == NULL)
		return;

    // If there was no tail, there was no list.
	if (list->tail == NULL)
	{
		list->head = list->tail = create_Listnode(data);
		return;
	}

	list->tail->next = create_Listnode(data);
	list->tail = list->tail->next;
}

// From Szumlanski's "Queue.c".
void enqueue(Queue *q, int data)
{
	if (q == NULL || q->list == NULL)
		return;

	q->size++;
	tail_insert(q->list, data);
}

// From Szumlanski's "LinkedLists.c".
int head_delete(LinkedList *list)
{
	int retval;
	Node *temp;

	// NULL checks in recursive_notKindredSpirits()
	// before calling dequeue(), which calls head_delete().

	retval = list->head->data;

	temp = list->head->next;

	free(list->head);

	list->head = temp;

	if (list->head == NULL)
		list->tail = NULL;

	return retval;
}

// From Szumlanski's "Queue.c".
int dequeue(Queue *q)
{
    // NULL checks in recursive_notKindredSpirits()
	// before calling dequeue().

	q->size--;
	return head_delete(q->list);
}

// From Szumlanski's "LinkedLists.c"
Node *recursive_destroyer(Node *head)
{
	if (head == NULL)
		return NULL;

	recursive_destroyer(head->next);
	free(head);

	return NULL;
}

// From Szumlanski's "LinkedLists.c"
LinkedList *destroy_list(LinkedList *list)
{
	if (list == NULL)
		return NULL;

	recursive_destroyer(list->head);
	free(list);

	return NULL;
}

Queue *destroy_queue(Queue *q)
{
    if (q == NULL)
		return NULL;

    // Deallocates queue and all of its parts.
    destroy_list(q->list);
    free(q);

    return NULL;
}

int recursive_notReflection(node *a, node *b)
{
    // Two empty trees are reflections of each other.
    if (a == NULL && b == NULL)
        return 0;

    // Checks for structural differences.
    if (a == NULL || b == NULL)
        return 1;

    // Checks for value differences.
    if (a->data != b->data)
        return 1;

    return (recursive_notReflection(a->left, b->right) ||
            recursive_notReflection(a->right, b->left));
}

int isReflection(node *a, node *b)
{
    return !(recursive_notReflection(a, b));
}

node *makeReflection(node *root)
{
    node *copy;

    if (root == NULL)
        return NULL;

    copy = create_binaryNode(root->data);

    // Builds copy's structure opposite of root's.
    copy->left = makeReflection(root->right);
    copy->right = makeReflection(root->left);

    return copy;
}

int recursive_notKindredSpirits(node *root, Queue *queue)
{
    if (root == NULL)
        return 0;

    // If all values from queue were already removed,
    // then the traversals don't match.
    if (queue == NULL || queue->size == 0 || queue->list == NULL ||
        queue->list->head == NULL || queue->list->tail == NULL)
        return 1;

    // Compares root's current preorder value to
    // the corresponding postorder value in the queue.
    if (root->data != dequeue(queue))
        return 1;

    return(recursive_notKindredSpirits(root->left, queue) ||
           recursive_notKindredSpirits(root->right, queue));
}

void record_postorder(node *root, Queue *queue)
{
    if (root == NULL)
        return;

    record_postorder(root->left, queue);
    record_postorder(root->right, queue);

    // Enters postorder value into queue.
    enqueue(queue, root->data);
}

int preComparedToPost(node *a, node *b)
{
    Queue *postorderQueue;

	postorderQueue = create_queue();

	// Puts postorder of a into queue.
    record_postorder(a, postorderQueue);

    // Success only if the entirety of both traversals matched,
    // without any leftover values from the postorder of a.
    if (!(recursive_notKindredSpirits(b, postorderQueue)) == 1
        && (postorderQueue == NULL || postorderQueue->size == 0))
    {
        destroy_queue(postorderQueue);
        return 1;
    }

    destroy_queue(postorderQueue);
    return 0;
}

int kindredSpirits(node *a, node *b)
{
    return (preComparedToPost(a, b) || preComparedToPost(b, a));
}

double difficultyRating(void)
{
    return 3.10;
}

double hoursSpent(void)
{
    return 40.0;
}
