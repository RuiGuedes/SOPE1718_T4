#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bufSize 1024

#define SUCESS 0
#define INSUCESS -1
#define EXISTS 1
#define DONT_EXISTS 2
#define FILE_OPEN_ERROR -2

#define DEFAULT   "\033[0m"
#define BLACK     "\033[30m"      /* Black */
#define RED       "\033[31m"      /* Red */
#define GREEN     "\033[32m"      /* Green */
#define YELLOW    "\033[33m"      /* Yellow */
#define BLUE      "\033[34m"      /* Blue */
#define MAGENTA   "\033[35m"      /* Magenta */
#define CYAN      "\033[36m"      /* Cyan */
#define WHITE     "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

const char * fileDirectory;

////////////////////////////
// Functions declarations //
////////////////////////////

int searchFile(const char * directory);
int searchFileWord();
int checkPatternExistenceOnFile();
int checkPatternPresenceOnTextLine(char * textLine);
int analyzeAndPrintWord(char * word);