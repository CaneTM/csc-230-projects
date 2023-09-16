/**
 * @file catalog.h
 * @author Canaan Matias (ctmatias)
 * 
 * Provides function prototypes, macrodefinitions, and types for the catalog component
*/

#include <stdbool.h>

/** Min number of required fields for a course description */
#define REQUIRED_FIELDS 4

/** The factor of how much an array needs to be resized */
#define RESIZE_FACTOR 2

/** Number of letters denoting the course department */
#define DEPT_LEN 3

/** Number of digits representing the course number */
#define COURSE_NUM_LEN 3

/** Number of characters representing the meeting days */
#define DAYS_LEN 2

/** Max number of characters representing the meeting time */
#define TIME_LEN 5

/** Max number of characters for the course name */
#define COURSE_NAME_LEN 30

/** Index position of the course name in a course file */
#define COURSE_NAME_POS 4

/** Initial capacity of a catalog's list of courses */
#define INIT_CAPACITY 5

/** Indicates if you're reading a department */
#define DEPT_FIELD 0

/** Indicates if you're reading a course number */
#define COURSE_NUM_FIELD 1

/** Indicates if you're reading the meeting days */
#define DAYS_FIELD 2

/** Indicates if you're reading the time */
#define TIME_FIELD 3

/**
 * Represents a course with a department, number,
 * meeting days, meeting time, and name
*/
typedef struct Course {
  char dept[DEPT_LEN + 1];
  char number[COURSE_NUM_LEN + 1];
  char days[DAYS_LEN + 1];
  char time[TIME_LEN + 1];
  char name[COURSE_NAME_LEN + 1];
} Course;

/**
 * Represents a list of courses the user can take
*/
typedef struct Catalog {
  Course **list;
  int count;
  int capacity;
} Catalog;

/**
 * Allocates storage for the Catalog, initializes its fields, and returns a pointer to the new Catalog
 * 
 * @return a pointer to a Catalog
*/
Catalog *makeCatalog();

/**
 * Frees the memory used to store the given Catalog, including freeing space for all the Courses, 
 * freeing the resizable array of pointers and freeing space for the Catalog struct itself.
 * 
 * @param catalog the Catalog to free
*/
void freeCatalog( Catalog *catalog );

/**
 * Reads all the courses from the given file. 
 * Makes an instance of the Course struct for each read course and 
 * stores a pointer to that Course in the resizable array in catalog.
 * 
 * @param filename the file to read the courses from
 * @param catalog the Catalog whose list is used to store the courses in
*/
void readCourses( char const *filename, Catalog *catalog );

/**
 * Sorts the courses in the given catalog.
 * Uses qsort() together with the function pointer parameter to order the courses.
 * 
 * @param catalog the catalog whose courses need to be sorted
 * @param compare a pointer to a comparison function which takes two Course pointers and is used for sorting
*/
void sortCourses( Catalog *catalog, int (* compare) (void const *va, void const *vb ));

/**
 * Prints all or some of the courses
 * 
 * @param catalog the Catalog containing the courses to print
 * @param test a pointer to a function used to pass in the criteria for what courses to print
 * @param str1 a string representing a criterion for printing a course
 * @param str2 a string representing a criterion for printing a course
*/
void listCourses( Catalog *catalog, bool (* test)( Course const *course, char const *str1, char const *str2 ), char const *str1, char const *str2 );
