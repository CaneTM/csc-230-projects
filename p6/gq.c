/**
 * @file gq.c
 * @author Canaan Matias (ctmatias)
 * 
 * Reads and processes commands from standard input and updates the queue in response.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "input.h"
#include "queue.h"
#include "types.h"

/** Visual cue for prompting the user for a command */
#define CMD_PRMPT "cmd> "

/** User command for adding to the queue */
#define ENQUEUE "enqueue"

/** User command for removing from the queue */
#define DEQUEUE "dequeue"

/** User command for moving a node to the front of the queue */
#define PROMOTE "promote"

/** User command for getting length of queue */
#define LENGTH "length"

/** User command for quitting the program */
#define QUIT "quit\n"

/** List of all the node creation functions.  This is a chain of
    responsibility; we can keep trying these functions until we find
    one that knows how to parse the init string. */
static Node *(*nodeMakers[])( char const *init ) = {
  makeIntNode,
  makeRealNode,
  makeStringNode
};

/**
 * Prints the number of items in the given queue
 * 
 * @param queue the queue to find the length of
*/
static void performLength(Queue *queue)
{
  int length = 0;

  Node *current = queue->head;

  // Traverse node and increment length each time
  while (current) {
    length++;
    current = current->next;
  }

  printf("%d\n", length);
}

/**
 * Creates and returns a Queue node 
 * 
 * @param params the data of the node
 * @return a node for a Queue
*/
static Node *makeNode(char params[LINE_LEN])
{
  Node *n = NULL;

  int nodeMakersLen = sizeof(nodeMakers) / sizeof(nodeMakers[0]);

  for (int i = 0; i < nodeMakersLen; i++) {
    n = nodeMakers[i](params);

    // Node creation was successful
    if (n) {
      break;
    }
  }

  return n;
}

/**
 * Performs the enqueue command
 * 
 * @param queue the queue to add data to
 * @param params the data to enqueue
*/
static void performEnqueue(Queue *queue, char params[LINE_LEN])
{
  Node *n = makeNode(params);
  int param_len = strlen(params);

  // Node creation was unsuccessful
  // or no params were given
  if (!n || !param_len) {
    printf("Invalid command\n");
    return;
  }

  enqueue(queue, n);
}

/**
 * Performs the dequeue command
 * 
 * @param queue the queue to dequeue from
*/
static void performDequeue(Queue *queue)
{
  Node *removed_node = dequeue(queue);

  if (!removed_node) {
    printf("Invalid command\n");
    return;
  }

  if (removed_node->data[0] == '\0') {
    for (int i = 1; removed_node->data[i] != '\"'; i++) {
      printf("%c", removed_node->data[i]);
    }
    printf("\n");
  } 
  else {
    removed_node->print(removed_node);
  }

  // Analyze removed node's string if it has digit characters
  // (it's an invalid pointer for some reason)
  bool has_digit = false;

  for (int i = 1; removed_node->data[i]; i++) {
    if (isdigit(removed_node->data[i])) {
      has_digit = true;
    }
  }

  if (has_digit) {
    return;
  }

  if (!removed_node) {
    free(removed_node);
  }
}

/**
 * Performs the promote command
 * 
 * @param queue the queue to manipulate
 * @param params the data to search for and promote
*/
static void performPromote(Queue *queue, char params[LINE_LEN])
{
  int param_len = strlen(params);

  // No params were given
  if (!param_len) {
    printf("Invalid command\n");
    return;
  }

  Node *example = makeNode(params);

  // An example node could not be created
  if (!example) {
    printf("Invalid command\n");
    return;
  }

  bool is_promoted = promote(queue, example);

  // Found nothing to promote
  if (!is_promoted) {
    printf("Invalid command\n");
    return;
  }

  free(example);
}

/**
 * Parses the given user command
 * 
 * @param queue the queue to manipulate
 * @param cmd the user command to process
*/
static void processUserCommand(Queue *queue, char *cmd)
{
  char action[LINE_LEN] = "\0";
  char params[LINE_LEN] = "\0";
  char values[LINE_LEN] = "\0";
  char args[LINE_LEN] = "\0";

  int before_pos = 0;
  int after_pos = 0;
  int space_pos = 0;

  // Only read the action
  sscanf(cmd, "%[\t\v\r]%n", action, &after_pos);
  // Read remaining params
  sscanf(cmd + after_pos, " %n%[^\n] ", &space_pos, params);

  // Print action with any extra whitespace
  for (int i = 0; i < before_pos; i++) {
    printf(" ");
  }
  printf("%s", action);

  // Print params with any extra whitespace
  if (params[0]) {
    for (int i = 0; i < space_pos; i++) {
      printf(" ");
    }
    printf("%s", params);
  }
  printf("\n");

  // Parse the user command
  sscanf(params, "%s%n%*s", action, &after_pos);
  // sscanf(params + after_pos, "%s", values);

  for (int i = after_pos; params[i]; i++) {
    values[i - after_pos] = params[i];
  }

  // Remove leading whitespace
  int idx = 0;
  int j = 0;
  int k = 0;

  while (values[idx] == ' ' || values[idx] == '\t' || values[idx] == '\n') {
    idx++;
  }

  for (j = idx; values[j] != '\0'; j++) {
    args[k] = values[j];
    k++;
  }

  args[k] = '\0';

  if (!strcmp(ENQUEUE, action)) {   // Parse enqueue command
    performEnqueue(queue, args);
  }
  else if (!strcmp(DEQUEUE, action)) {  // Parse dequeue command
    performDequeue(queue);
  }
  else if (!strcmp(PROMOTE, action)) {  // Parse promote command
    performPromote(queue, args);
  }
  else if (!strcmp(LENGTH, action)) {   // Parse length command
    performLength(queue);
  }
  else {
    printf("Invalid command\n");
  }
  printf("\n");
}

/**
 * Reads user commands from stdin
 * 
 * @param queue the queue to manipulate
*/
static void readUserCommands(Queue *queue)
{
  char command[LINE_LEN + 1];
  // Make a copy to determine if you reach EOF
  // FILE *fp = fopen("input-09.txt", "r");
  // char *command_cpy = fgets(command, LINE_LEN + 1, fp);
  char *command_cpy = fgets(command, LINE_LEN + 1, stdin);
  int x = 0;

  // Keep running if you have not encountered EOF or quit
  while (command_cpy && strcmp(QUIT, command)) {
    printf(CMD_PRMPT);
    processUserCommand(queue, command);
    // command_cpy = fgets(command, LINE_LEN + 1, fp);
    command_cpy = fgets(command, LINE_LEN + 1, stdin);
    x++;
  }

  // A quit command was encountered
  if (!strcmp(QUIT, command)) {
    printf(CMD_PRMPT);
    printf("quit\n");
  }

  // EOF was encountered
  else if (!command_cpy) {
    printf(CMD_PRMPT);
  }
}

/**
 * Entry point of program
 * 
 * @param argc number of cmd line args
 * @param argv array of cmd line args
 * @return exit status code
*/
int main(int argc, char const *argv[])
{
  // Create the queue
  Queue *queue = makeQueue();

  // Read in commands from the user
  readUserCommands(queue);

  // When done, free the queue
  freeQueue(queue);

  return EXIT_SUCCESS;
}
