/**
 * @file schedule.c
 * @author Canaan Matias (ctmatias)
 *
 * Serves as the top-level component for parsing user commands
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "catalog.h"

/** Required number of arguments for program to run */
#define REQUIRED_ARGS 2

/** Max length of one space-separated token in a user command */
#define CMD_ARG_LEN 10

/** Max number of courses for a schedule */
#define MAX_COURSES 10

/** An AM time whose hour is 8 */
#define AM_8 8

/** An AM time whose hour is 10 */
#define AM_10 10

/** An AM time whose hour is 11 */
#define AM_11 11

/** Visual cue for prompting the user for a command */
#define CMD_PRMPT "cmd> "

/** User command for listing courses */
#define LIST "list"

/** User command for adding a course */
#define ADD "add"

/** User command for dropping a course */
#define DROP "drop"

/** User command for quitting the program */
#define QUIT "quit\n"

/**
 * Represents the user's class schedule
*/
typedef struct Schedule {
  Course **list;
  int count;
  int capacity;
} Schedule;

/**
 * Determines whether the given course should be printed
 * 
 * @param course the course to print
 * @param str1 a string representing a criterion for printing a course
 * @param str2 a string representing a criterion for printing a course
 * @return true if the given course should be printed, false otherwise
*/
static bool test(Course const *course, char const *str1, char const *str2)
{
  // Print only courses with a matching dept (str1 = dept)
  if (str1 && !str2) {
    return !strcmp(course->dept, str1);
  }
  // Print only courses with a matching timeslot (str1 = days, str2 = time)
  else if (str1 && str2) {
    return !strcmp(course->days, str1) && !strcmp(course->time, str2);
  }

  // Print all courses
  return true;
}

/**
 * Returns true if the given hour represents 
 * an AM time, false if it's a PM time
 * 
 * @param hour the hour to determine the meridian of
 * @return true if the given hour represents an AM time, false if it's a PM time
*/
static bool isMorningHour(int hour)
{
  return hour == AM_8 || hour == AM_10 || hour == AM_11;
}

/**
 * Returns true if the given hour represents 
 * an AM time, false if it's a PM time
 * 
 * @param hour the hour to determine the meridian of
 * @return true if the given hour represents an AM time, false if it's a PM time
*/
static bool areTimesSameMeridian(bool time1, bool time2)
{
  return (time1 && time2) || (!time1 && !time2);
}

/**
 * Compares two courses by their ID (dept and number)
 * 
 * @param va Course pointer
 * @param vb Course pointer
 * @return -1 if va should come before vb, 1 if va should come after vb, or 0 if equal
*/
static int compareByCourseID(void const *va, void const *vb)
{
  // Convert parameters to Course pointers
  Course **course1 = (Course **) va;
  Course **course2 = (Course **) vb;

  int cmp_val = strcmp((*course1)->dept, (*course2)->dept);

  return cmp_val != 0 ? cmp_val : strcmp((*course1)->number, (*course2)->number);
}

/**
 * Compares two courses by their name, followed by their ID if the names are identical
 * 
 * @param va Course pointer
 * @param vb Course pointer
 * @return -1 if va should come before vb, 1 if va should come after vb, or 0 if equal
*/
static int compareByCourseName(void const *va, void const *vb)
{
  // Convert parameters to Course pointers
  Course **course1 = (Course **) va;
  Course **course2 = (Course **) vb;

  int cmp_val = strcmp((*course1)->name, (*course2)->name);

  // Sort by course ID if the names are the same
  return cmp_val != 0 ? cmp_val : compareByCourseID(va, vb);
}

/**
 * Compares two courses by their meeting days, followed 
 * by their start time if the meetings days are the same
 * 
 * @param va Course pointer
 * @param vb Course pointer
 * @return -1 if va should come before vb, 1 if va should come after vb, or 0 if equal
*/
static int compareByCourseTimeslot(void const *va, void const *vb)
{
  // Convert parameters to Course pointers
  Course **course1 = (Course **) va;
  Course **course2 = (Course **) vb;

  // First, compare the days
  int cmp_val = strcmp((*course1)->days, (*course2)->days);

  // Next, compare the times if the days are the same
  if (!cmp_val) {
    // Get the hour parts of the time string
    // Will want to make a copy so it doesn't
    // modify the original value
    char hour1_cpy[strlen((*course1)->time)];
    strcpy(hour1_cpy, (*course1)->time);
    char *hour1 = strtok(hour1_cpy, ":");

    char hour2_cpy[strlen((*course2)->time)];
    strcpy(hour2_cpy, (*course2)->time);
    char *hour2 = strtok(hour2_cpy, ":");

    // Convert each string to int
    int course1_hour = atoi(hour1);
    int course2_hour = atoi(hour2);

    // Determine the meridian of each time
    bool is_course1_am = isMorningHour(course1_hour);
    bool is_course2_am = isMorningHour(course2_hour);

    int hour_diff = course1_hour - course2_hour;

    if (areTimesSameMeridian(is_course1_am, is_course2_am)) {
      return hour_diff;
    }
    else {
      return is_course1_am ? -1 : 1;
    }
  }

  return cmp_val;
}

/**
 * Prints the user's course schedule
 * 
 * @param schedule the schedule to print
*/
static void printSchedule(Schedule *schedule)
{
  // Sort schedule here
  qsort(schedule->list, schedule->count, sizeof(Course *), compareByCourseTimeslot);

  // Print the header
  printf("%-7s ", "Course");
  printf("%-30s ", "Name");
  printf("%-8s\n", "Timeslot");

  for (int i = 0; i < schedule->count; i++) {
    printf("%s %s ", (schedule->list)[i]->dept, (schedule->list)[i]->number);
    printf("%-30s ", (schedule->list)[i]->name);
    printf("%s ", (schedule->list)[i]->days);
    printf("%5s\n", (schedule->list)[i]->time);
  }

  printf("\n");
}

/**
 * Handles all variations of the list command (list names, list courses,
 * list department, list timeslot, and list schedule)
 * 
 * @param catalog the catalog containing the courses 
 * @param arg2 the second argument in the list command
 * @param arg3 the third argument in the list command
 * @param arg4 the fourth argument in the list command
*/
static void performList(Catalog *catalog, Schedule *schedule, char arg2[], char arg3[], char arg4[])
{
  // Perform names subcommand
  // Print all courses, but sorted by name
  if (!strcmp("names", arg2)) {
    sortCourses(catalog, &compareByCourseName);
    listCourses(catalog, &test, NULL, NULL);
    return;
  }

  // Perform schedule subcommand
  else if (!strcmp("schedule", arg2)) {
    printSchedule(schedule);
    return;
  }

  // By default, sort by course ID
  sortCourses(catalog, &compareByCourseID);

  // Perform courses subcommand
  // Print all courses, but sorted by ID
  if (!strcmp("courses", arg2)) {
    listCourses(catalog, &test, NULL, NULL);
  }

  // Perform department subcommand
  // Print courses with matching department
  else if (!strcmp("department", arg2)) {
    // Invalid if dept is not 3 uppercase letters
    if (strlen(arg3) != DEPT_LEN || !isupper(arg3[0]) || !isupper(arg3[1]) || !isupper(arg3[2])) {
      printf("Invalid command\n\n");
      return;
    }
    listCourses(catalog, &test, arg3, NULL);
  }

  // Perform timeslot subcommand
  // Print courses with matching timeslot
  else if (!strcmp("timeslot", arg2)) {
    // Invalid if days is not MW or TH
    if (strcmp("MW", arg3) && strcmp("TH", arg3)) {
      printf("Invalid command\n\n");
      return;
    }
    // Invalid if time is not one of the following times
    if (strcmp("8:30", arg4) && strcmp("10:00", arg4) && strcmp("11:30", arg4) && strcmp("1:00", arg4) && strcmp("2:30", arg4) && strcmp("4:00", arg4)) {
      printf("Invalid command\n\n");
      return;
    }
    listCourses(catalog, &test, arg3, arg4);
  }

  // Invalid command detected
  else {
    printf("Invalid command\n\n");
  }
}

/**
 * Handles the add command (add DEPT NUM).
 * Adds a course to the user's schedule.
 * 
 * @param catalog the catalog containing the courses 
 * @param schedule the schedule to add the course to
 * @param arg2 the course dept
 * @param arg3 the course number
*/
static void performAdd(Catalog *catalog, Schedule *schedule, char arg2[], char arg3[])
{
  // Invalid if schedule already has 10 courses
  if (schedule->count >= MAX_COURSES) {
    printf("Invalid command\n");
    return;
  }

  // Resize schedule list if list capacity is met
  if (schedule->count >= schedule->capacity) {
    schedule->capacity *= RESIZE_FACTOR;
    schedule->list = (Course **) realloc(schedule->list, sizeof(Course *) * schedule->capacity);
  }

  // Make a pointer for the course to add
  Course *course = NULL;

  // Search for the course the user requested
  for (int i = 0; i < catalog->count; i++) {
    if (!strcmp(arg2, (catalog->list)[i]->dept) && !strcmp(arg3, (catalog->list)[i]->number)) {
      course = (catalog->list)[i];
      break;
    }
  }

  // If the requested course was not
  // found, the command was invalid
  if (!course) {
    printf("Invalid command\n");
    return;
  }

  // Determine whether any course in the schedule has
  // the same timeslot as course to add
  for (int i = 0; i < schedule->count; i++) {
    if (!compareByCourseTimeslot(&((schedule->list)[i]), &course)) {
      printf("Invalid command\n");
      return;
    }
  }

  // Add the course to the schedule
  (schedule->list)[schedule->count] = course;
  schedule->count += 1;
}

/**
 * Handles the drop command (drop DEPT NUM).
 * Removes a course from the user's schedule.
 * 
 * @param schedule the schedule to remove the course from
 * @param arg2 the course dept
 * @param arg3 the course number
*/
static void performDrop(Catalog *catalog, Schedule *schedule, char arg2[], char arg3[])
{
  int removed_idx = -1;

  // Search for the course to remove
  for (int i = 0; i < schedule->count; i++) {
    if (!strcmp(arg2, (schedule->list)[i]->dept) && !strcmp(arg3, (schedule->list)[i]->number)) {
      removed_idx = i;
      break;
    }
  }

  // Command is invalid if it didn't find a course to remove
  if (removed_idx == -1) {
    printf("Invalid command\n");
    return;
  }

  // Left-shift the courses in the schedule (starting from removed_idx)
  for (int i = removed_idx; i < schedule->count - 1; i++) {
    (schedule->list)[i] = (schedule->list)[i + 1];
  }

  schedule->count -= 1;
}

/**
 * Parses the given user command
 * 
 * @param command the user command to process
*/
static void processUserCommand(Catalog *catalog, Schedule *schedule, char *command)
{
  // Up to 4 args could be provided in a user command
  char arg1[CMD_ARG_LEN + 1] = {'\0'};
  char arg2[CMD_ARG_LEN + 1] = {'\0'};
  char arg3[CMD_ARG_LEN + 1] = {'\0'};
  char arg4[CMD_ARG_LEN + 1] = {'\0'};

  sscanf(command, "%s %s %s %s", arg1, arg2, arg3, arg4);

  // Echo user command
  if (arg1[0]) {
    printf("%s", arg1);
  }
  if (arg2[0]) {
    printf(" %s", arg2);
  }
  if (arg3[0]) {
    printf(" %s", arg3);
  }
  if (arg4[0]) {
    printf(" %s", arg4);
  }
  printf("\n");

  if (!strcmp(LIST, arg1)) {  // Parse a list command
    performList(catalog, schedule, arg2, arg3, arg4);
  }
  else if (!strcmp(ADD, arg1)) {  // Parse an add command
    performAdd(catalog, schedule, arg2, arg3);
    printf("\n");
  }
  else if (!strcmp(DROP, arg1)) { // Parse a drop command
    performDrop(catalog, schedule, arg2, arg3);
    printf("\n");
  }
  else {
    printf("Invalid command\n");
  }
}

/**
 * Reads user commands from stdin
*/
static void readUserCommands(Catalog *catalog, Schedule *schedule)
{
  char command[LINE_LEN + 1];
  // Used to determine if you reach EOF
  char *command_cpy = fgets(command, LINE_LEN + 1, stdin);

  // Keep running if you have not encountered EOF or quit
  while (command_cpy && strcmp(QUIT, command)) {
    printf(CMD_PRMPT);
    processUserCommand(catalog, schedule, command);
    command_cpy = fgets(command, LINE_LEN + 1, stdin);
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

  free(schedule->list);
}

/**
 * Initializes the given catalog and its fields
 * 
 * @param catalog the catalog to initialize
 * @param argc number of cmd line args
 * @param argv list of cmd line args
*/
static void loadCatalog(Catalog *catalog, int argc, char const *argv[])
{
  // Terminate if no course file is provided
  if (argc < REQUIRED_ARGS) {
    fprintf(stderr, "usage: schedule <course-file>*\n");
    exit(EXIT_FAILURE);
  }

  // Read each course file
  for (int i = 1; i < argc; i++) {
    readCourses(argv[i], catalog);
  }
}

/**
 * Entry point of program
 * 
 * @param argc number of cmd line args
 * @param argv list of cmd line args
 * @return exit status code
*/
int main(int argc, char const *argv[])
{
  // Create and initialize the catalog
  Catalog *catalog = makeCatalog();

  loadCatalog(catalog, argc, argv);

  // Create a course schedule and use it for a Schedule object
  Course **course_schedule = (Course **) malloc(sizeof(Course *) * INIT_CAPACITY);
  Schedule schedule = { course_schedule, 0, INIT_CAPACITY };

  readUserCommands(catalog, &schedule);

  freeCatalog(catalog);

  return EXIT_SUCCESS;
}
