/**
    @file queue.h
    @author Canaan Matias (ctmatias)
    Header file for queue, a generic implementation of a linked list.
*/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

/** Node containing an arbitrary value. */
typedef struct NodeStruct {
  /** Pointer to a function that prints the value in this node.
      @param n Pointer to the node containing the value to print. */
  void (*print)(struct NodeStruct const *n);

  /** Compare the value in the two given nodes, returning true
      if it's considered equivalent.
      @param a Pointer to the left-hand node to compare (the one
      containing this compare function)
      @param b Poitner to the right-hand node to compare.
      @return pointer to a new queue. */
  bool (*equals)(struct NodeStruct const *a,
                 struct NodeStruct const *b);

  /** Pointer to the next node on the queue. */
  struct NodeStruct *next;

  /** Contents of the node.  Whenever we allocate a node, we reserve some
      extra space at the end for holding the contents.  This field evaluates
      to a pointer to the start of that extra space. */
  char data[];
} Node;

/** Representation of a queue of generic values. */
typedef struct {
  /** Pointer to the first node in the queue. */
  Node *head;

  /** Address of the null pointer at the end of this queue, either
      pointing to the head pointer or pointing to the next pointer
      inside the last node. */
  Node **tail;
} Queue;

/**
 * Makes an empty, dynamically allocated queue, initializing its field(s) and returning a pointer to it.
 * 
 * @return a pointer to a new, empty queue
*/
Queue *makeQueue();

/**
 * Adds the given node to the back of the given queue.
 * 
 * @param q the queue to add a Node to
 * @param n the node to add to the given queue
*/
void enqueue( Queue *q, Node *n );

/**
 * Removes and returns the node at the front of the given queue
 * 
 * @param q the queue to remove the first node from
 * @return the node at the front of the queue, NULL if empty
*/
Node *dequeue( Queue *q );

/**
 * Finds the first node in the queue with a value that matches example and moves it to the front of the queue.
 * 
 * @param q the queue to find the given example in
 * @param example the Node which should be promoted 
 * @return true if a Node was successfully promoted, false otherwise
*/
bool promote( Queue *q, Node const *example );

/**
 * Frees all the memory used to store the given queue, including the memory for each of the nodes.
 * 
 * @param q the queue to free
*/
void freeQueue( Queue *q );

#endif
