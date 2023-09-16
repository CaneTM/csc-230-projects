/**
 * @file catalog.c
 * @author Canaan Matias (ctmatias)
 *
 * Reads and manages courses at start-up
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "catalog.h"

/**
 * Terminates the program and prints an error message
 * saying the provided course file was invalid
 * 
 * @param filename the name of the invalid file 
*/
static void invalidCourseFile(char const *filename)
{
  fprintf(stderr, "Invalid course file: %s\n", filename);
  exit(EXIT_FAILURE);
}

Catalog *makeCatalog()
{
  // Create a Catalog pointer
  Catalog *catalog = (Catalog *) malloc(sizeof(Catalog));

  // Allocate memory for the course list
  Course **course_list = (Course **) malloc(sizeof(Course *) * INIT_CAPACITY);

  // Initialize the fields
  catalog->list = course_list;
  catalog->capacity = INIT_CAPACITY;
  catalog->count = 0;

  return catalog;
}

void freeCatalog( Catalog *catalog )
{
  // Free each of the Course pointers in list
  for (int i = 0; i < catalog->count; i++) {
    free((catalog->list)[i]);
  }
  // Free the list itself
  free(catalog->list);

  // Free the catalog
  free(catalog);
}

/**
 * Initializes the fields of the given Course struct
 * 
 * @param count determines what field to initialize
 * @param course the Course to initialize
 * @param word the value of the Course's field
*/
static void initCourseFields(int count, Course *course, char *word, char const *filename)
{
  switch (count) {
    // Reading the dept
    case DEPT_FIELD:
      if (strlen(word) != DEPT_LEN || !isupper(word[0]) || !isupper(word[1]) || !isupper(word[2])) {
        invalidCourseFile(filename);
      }
      strcpy(course->dept, word);
      break;

    // Reading the course number
    case COURSE_NUM_FIELD:
      if (strlen(word) != COURSE_NUM_LEN || !isdigit(word[0]) || !isdigit(word[1]) || !isdigit(word[2])) {
        invalidCourseFile(filename);
      }
      strcpy(course->number, word);
      break;

    // Reading the meeting days
    case DAYS_FIELD:
      if (strcmp("MW", word) && strcmp("TH", word)) {
        invalidCourseFile(filename);
      }
      strcpy(course->days, word);
      break;

    // Reading the meeting time
    case TIME_FIELD:
      if (strcmp("8:30", word) && strcmp("10:00", word) && strcmp("11:30", word) && strcmp("1:00", word) && strcmp("2:30", word) && strcmp("4:00", word)) {
        invalidCourseFile(filename);
      }
      strcpy(course->time, word);
      break;
    
    // Reading the course name
    default:
      if (strlen(word) > COURSE_NAME_LEN) {
        invalidCourseFile(filename);
      }
      strcpy(course->name, word);
      break;
  }
}

/**
 * Updates the fields of the given catalog
 * 
 * @param catalog the catalog to update
 * @param course the course to add to the catalog's course list
*/
static void updateCatalog(Catalog *catalog, Course *course)
{
  // Resize course list if list capacity is met
  if (catalog->count >= catalog->capacity) {
    catalog->capacity *= RESIZE_FACTOR;
    catalog->list = (Course **) realloc(catalog->list, sizeof(Course *) * catalog->capacity);
  }

  (catalog->list)[catalog->count] = course;
  catalog->count += 1;  
}

/**
 * Removes all leading whitespace from the given string
 * 
 * @param str the string to remove leading whitespace from
 * @return a string without any leading whitespace
*/
static char *removeLeadingWhitespace(char *str)
{
  int idx = 0;

  // Iterate string until last leading space character
  while (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n') {
    idx++;
  }

  return (char *) &(str[idx]);
}

/**
 * Checks the catalog for any duplicate courses, and
 * terminates the program if so
 * 
 * @param catalog the catalog containing all the courses
*/
static void checkCatalogForDuplicates(Catalog *catalog, char const *filename)
{
  for (int i = 0; i < catalog->count - 1; i++) {
    for (int j = i + 1; j < catalog->count; j++) {
      // Two courses are duplicates if they have the same dept and number
      if (!strcmp((catalog->list)[i]->dept, (catalog->list)[j]->dept) && !strcmp((catalog->list)[i]->number, (catalog->list)[j]->number)) {
        invalidCourseFile(filename);
      }
    }
  }
}

void readCourses( char const *filename, Catalog *catalog )
{
  // This opens a course file
  FILE *fp = fopen(filename, "r");

  // Invalid file detected
  if (!fp) {
    free(fp);
    fprintf(stderr, "Can't open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  // Read a line from the file
  char *line = readLine(fp);
  bool has_line = (line != NULL);

  // Read until the end of the file
  while (has_line) {
    // Make a Course to store the course being parsed
    Course *course = (Course *) malloc(sizeof(Course));

    // Get the first word in the line
    char *word = strtok(line, (char *) " ");

    // Used to keep track of what course field you're reading
    int count = 0;

    // Keep getting words until the end of the line
    while (word) {

      initCourseFields(count, course, word, filename);

      count++;

      // If you've reached the course name, grab everthing until the end of the line
      if (count >= COURSE_NAME_POS) {
        word = removeLeadingWhitespace(strtok(NULL, "\0"));
        initCourseFields(count, course, word, filename);
        break;
      }

      word = strtok(NULL, " ");
    }

    // Check that all fields are present
    if (count < REQUIRED_FIELDS) {
      invalidCourseFile(filename);
    }

    updateCatalog(catalog, course); // Add course to catalog and update fields accordingly

    free(line);

    line = readLine(fp);
    has_line = (line != NULL);
  }

  free(line);

  checkCatalogForDuplicates(catalog, filename);

  fclose(fp);
}

void sortCourses( Catalog *catalog, int (* compare) (void const *va, void const *vb))
{
  // Pass in the list to sort, 
  // the number of items in the list, 
  // the size of each item
  // and the comparison function
  qsort(catalog->list, catalog->count, sizeof(Course *), compare);
}

/**
 * Prints an output line containing the course id, course name, and timeslot
 * 
 * @param idx the index position of the desired course
 * @param catalog the catalog containing the list of courses
*/
static void printCourse(int idx, Catalog *catalog)
{
  printf("%s %s ", (catalog->list)[idx]->dept, (catalog->list)[idx]->number);
  printf("%-30s ", (catalog->list)[idx]->name);
  printf("%s ", (catalog->list)[idx]->days);
  printf("%5s\n", (catalog->list)[idx]->time);
}

void listCourses( Catalog *catalog, bool (* test)( Course const *course, char const *str1, char const *str2 ), char const *str1, char const *str2 )
{
  // Print the header
  printf("%-7s ", "Course");
  printf("%-30s ", "Name");
  printf("%-8s\n", "Timeslot");

  for (int i = 0; i < catalog->count; i++) {
    // Print the course if it meets the given criteria
    if (test((catalog->list)[i], str1, str2)) {
      printCourse(i, catalog);
    }
  }

  printf("\n");
}
