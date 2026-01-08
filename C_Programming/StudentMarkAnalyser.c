//============================================================//
//           Student Mark Analyzer                            //
//============================================================//

#include <stdio.h>
#include <string.h>

#define SUBJECT_COUNT 6
#define TOTAL_MARKS 600
#define MAX_NAME_LEN 30
#define MAX_CLASS_LEN 10

// Function Declarations
void inputStudentDetails(char name[], char className[]);
void inputMarks(int marks[], const char *subjects[], int subjectCount);
int calculateTotal(int marks[]);
int findHighest(int marks[], const char *subjects[], int subjectCount, char strongSubject[]);
int findLowest(int marks[], const char *subjects[], int subjectCount, char weakSunject[]);
float calculatePercentage(int securedMarks);
char calculateGrade(float percentage);
void displayResult(char name[], char className[], int securedMarks,
                   float percentage, char grade, int highest, int lowest, char strongSubject[], char weakSubject[]);

int main()
{
  char studentName[MAX_NAME_LEN];
  char className[MAX_CLASS_LEN];
  int marks[SUBJECT_COUNT];
  char strongSubject[15], weakSubject[15];
  int securedMarks, highest, lowest;
  float percentage;
  char grade;

  const char *subjects[SUBJECT_COUNT] = {
      "Odia", "English", "Sanskrit",
      "Mathematics", "General Science", "Social Science"};

  inputStudentDetails(studentName, className);
  inputMarks(marks, subjects, SUBJECT_COUNT);

  securedMarks = calculateTotal(marks);
  highest = findHighest(marks, subjects, SUBJECT_COUNT, strongSubject);
  lowest = findLowest(marks, subjects, SUBJECT_COUNT, weakSubject);
  percentage = calculatePercentage(securedMarks);
  grade = calculateGrade(percentage);

  displayResult(studentName, className, securedMarks,
                percentage, grade, highest, lowest, strongSubject, weakSubject);

  return 0;
}

//====================== Function Definitions ======================//

void inputStudentDetails(char name[], char className[])
{
  printf("Enter Student Name        : ");
  scanf("%s", name);

  printf("Enter Class Studied       : ");
  scanf("%s", className);
}

void inputMarks(int marks[], const char *subjects[], int subjectCount)
{
  printf("\nEnter marks for each subject (0 - 100)\n");

  for (int i = 0; i < subjectCount; i++)
  {
    do
    {
      printf("%s : ", subjects[i]);
      scanf("%d", &marks[i]);

      if (marks[i] < 0 || marks[i] > 100)
        printf("Invalid marks! Enter between 0 and 100.\n");

    } while (marks[i] < 0 || marks[i] > 100);
  }
}

int calculateTotal(int marks[])
{
  int total = 0;
  for (int i = 0; i < SUBJECT_COUNT; i++)
    total += marks[i];
  return total;
}

int findHighest(int marks[], const char *subjects[], int subjectCount, char strongSubject[])
{
  int max = marks[0];
  int idx = 0;
  for (int i = 1; i < subjectCount; i++)
  {
    if (marks[i] > max)
    {
      max = marks[i];
      idx = i;
    }
  }
  strcpy(strongSubject, subjects[idx]);
  return max;
}

int findLowest(int marks[], const char *subjects[], int subjectCount, char weakSubject[])
{
  int min = marks[0];
  int idx = 0;
  for (int i = 1; i < subjectCount; i++)
  {
    if (marks[i] < min)
    {
      min = marks[i];
      idx = i;
    }
  }
  strcpy(weakSubject, subjects[idx]);
  return min;
}

float calculatePercentage(int securedMarks)
{
  return ((float)securedMarks / TOTAL_MARKS) * 100;
}

char calculateGrade(float percentage)
{
  if (percentage >= 90)
    return 'A';
  else if (percentage >= 80)
    return 'B';
  else if (percentage >= 70)
    return 'C';
  else if (percentage >= 60)
    return 'D';
  else
    return 'F';
}

void displayResult(char name[], char className[], int securedMarks,
                   float percentage, char grade, int highest, int lowest, char strongSubject[], char weakSubject[])
{
  printf("\n=================== STUDENT REPORT =======================\n");
  printf("Student Name                : %s\n", name);
  printf("Class                       : %s\n", className);
  printf("Total Marks                 : %d\n", TOTAL_MARKS);
  printf("Secured Marks               : %d\n", securedMarks);
  printf("Percentage                  : %.2f %%\n", percentage);
  printf("Grade                       : %c\n", grade);
  printf("Weakest Subject %s scored   : %d\n", weakSubject, lowest);
  printf("Strongest Subject %s scored : %d\n", strongSubject, highest);
  printf("=============================================================\n");
}

//====================== Sample Output ======================//
/*
Enter marks for each subject (0 - 100)
Odia : 88
English : 75
Sanskrit : 94
Mathematics : 79
General Science : 91
Social Science : 76

=================== STUDENT REPORT =======================
Student Name                : Subhash_ratha
Class                       : 10th
Total Marks                 : 600
Secured Marks               : 503
Percentage                  : 83.83 %
Grade                       : B
Weakest Subject English scored   : 75
Strongest Subject Sanskrit scored : 94
=============================================================
*/
