/**
 * @file queue.c
 * @author Canaan Matias (ctmatias)
 * 
 * Represents the queue of values using a linked list. 
 * Provides definitions for the Node type used to represent individual values 
 * and the Queue type, used to represent the whole linked list.
*/

#include "queue.h"

#include <stdlib.h>
#include <stdio.h>

Queue *makeQueue()
{
  // Allocate space for the queue
  Queue *queue = (Queue *) malloc(sizeof(Queue));

  // For empty queue, head is null and tail points to head
  queue->head = NULL;
  queue->tail = &(queue->head);

  return queue;
}

void enqueue( Queue *q, Node *n )
{
  // Special case for when queue is empty
  if (!(q->head)) {
    q->head = n;
    q->tail = &(n->next);
    return;
  }

  // Make the pointer tail is pointing to point to n
  *(q->tail) = n;
  // Make tail point to n's next
  q->tail = &(n->next);
}

Node *dequeue( Queue *q )
{
  // Return null if queue is empty
  if (!(q->head)) {
    return NULL;
  }

  // Get the first node
  Node *front = q->head;
  // Set head to point to next node
  q->head = front->next;

  // Check if queue is now empty
  if (!(q->head)) {
    q->tail = &(q->head);
  }

  return front;
}

bool promote( Queue *q, Node const *example )
{
  // Special case for empty queue
  if (!(q->head)) {
    return false;
  }

  Node *prev = NULL;
  Node *current = q->head;

  // Traverse nodes until you reach end of queue
  // or you find a matching node
  while (current && !(current->equals(example, current))) {
    prev = current;
    current = current->next;
  }

  // Reached end of queue
  if (!current) {
    return false;
  }

  // You want to promote the first item in the queue
  if (current == q->head) {
    return true;
  }

  // Special case for promoting the last element
  if (!(current->next)) {
    q->tail = &(prev->next);
  }

  // Make the node before current point to node after current
  prev->next = current->next;
  // Make current point to front of queue
  current->next = q->head;
  // Make current the front of the queue
  q->head = current;

  // free(prev);
  // free(current);

  return true;
}

void freeQueue( Queue *q )
{
  // Ensures we don't lose the next 
  // reference after freeing head
  while (q->head) {
    Node *next = q->head->next;  
    free(q->head);
    q->head = next;
  }

  // free(q->head);
  // free(q->tail);
  free(q);
}
