//===================================================================================//
//                          Visitor Management System                                //
//===================================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define INITIAL_CAPACITY 5
#define MAX_NAME_LEN 50
#define MAX_PHONE_LEN 15
#define FILENAME "visitors.dat"

//====================Data Structure====================
typedef struct
{
  char name[MAX_NAME_LEN];
  char phone[MAX_PHONE_LEN];
  char visit_time[20];
} Visitor;

typedef struct
{
  Visitor *visitors;
  int count;
  int capacity;
} VisitorList;

//====================Prototypes========================
void initVisitorList(VisitorList *list);
void freeVisitorList(VisitorList *list);
int addVisitor(VisitorList *list);
void displayAllVisitors(VisitorList *list);
void saveToFile(VisitorList *list);
int loadFromFile(VisitorList *list);
void displayMenu();
int getValidChoice();
void clearInputBuffer();
int isValidPhone(const char *phone);
void getCurrentTime(char *timeStr);

//====================Main Functions====================
int main()
{
  VisitorList visitors;
  int choice;

  initVisitorList(&visitors);
  loadFromFile(&visitors);

  printf("\n=== VISITOR MANAGEMENT SYSTEM ===\n");
  printf("==================================\n");

  while (1)
  {
    displayMenu();
    choice = getValidChoice();

    switch (choice)
    {
    case 1:
      if (addVisitor(&visitors))
      {
        printf("\n Visitor added successfully!\n");
        saveToFile(&visitors);
      }
      break;

    case 2:
      displayAllVisitors(&visitors);
      break;

    case 3:
      saveToFile(&visitors);
      printf("\n Data saved successfully!\n");
      break;

    case 4:
      printf("\n Thank you for using Visitor Management System!\n");
      freeVisitorList(&visitors);
      return 0;

    default:
      printf("\n Invalid choice! Please try again.\n");
    }

    // printf("\nPress Enter to continue...");
    // clearInputBuffer();
  }

  return 0;
}

//====================Implementation====================
void initVisitorList(VisitorList *list)
{
  list->capacity = INITIAL_CAPACITY;
  list->count = 0;
  list->visitors = (Visitor *)malloc(list->capacity * sizeof(Visitor));
  if (list->visitors == NULL)
  {
    printf("❌ Memory allocation failed!\n");
    exit(1);
  }
}

void freeVisitorList(VisitorList *list)
{
  if (list->visitors)
  {
    free(list->visitors);
    list->visitors = NULL;
  }
}

void resizeVisitorList(VisitorList *list)
{
  list->capacity *= 2;
  Visitor *temp = (Visitor *)realloc(list->visitors, list->capacity * sizeof(Visitor));
  if (temp == NULL)
  {
    printf("Memory reallocation failed. Continuing with current capacity.\n");
    return;
  }
  list->visitors = temp;
}

int addVisitor(VisitorList *list)
{
  if (list->count >= list->capacity)
  {
    resizeVisitorList(list);
  }

  Visitor *newVisitor = &list->visitors[list->count];

  printf("\n--- Add New Visitor ---\n");
  printf("Enter visitor name: ");

  fgets(newVisitor->name, MAX_NAME_LEN, stdin);
  newVisitor->name[strcspn(newVisitor->name, "\n")] = 0;

  printf("Enter phone number: ");
  fgets(newVisitor->phone, MAX_PHONE_LEN, stdin);
  newVisitor->phone[strcspn(newVisitor->phone, "\n")] = 0;

  if (!isValidPhone(newVisitor->phone))
  {
    printf(" Invalid phone number format!\n");
    return 0;
  }

  getCurrentTime(newVisitor->visit_time);

  list->count++;
  return 1;
}

void displayAllVisitors(VisitorList *list)
{
  if (list->count == 0)
  {
    printf("\n No visitors recorded yet.\n");
    return;
  }

  printf("\n=== VISITOR REGISTER (Total: %d) ===\n", list->count);
  printf("%-4s | %-20s | %-15s | %-19s\n", "ID", "NAME", "PHONE", "VISIT TIME");
  printf("----------------------------------------------------------\n");

  for (int i = 0; i < list->count; i++)
  {
    printf("%-4d | %-20s | %-15s | %-19s\n",
           i + 1,
           list->visitors[i].name,
           list->visitors[i].phone,
           list->visitors[i].visit_time);
  }
}

void saveToFile(VisitorList *list)
{
  FILE *file = fopen(FILENAME, "wb");
  if (!file)
  {
    printf(" Failed to save data!\n");
    return;
  }

  fwrite(&list->count, sizeof(int), 1, file);
  fwrite(list->visitors, sizeof(Visitor), list->count, file);
  fclose(file);
}

int loadFromFile(VisitorList *list)
{
  FILE *file = fopen(FILENAME, "rb");
  if (!file)
  {
    printf(" No existing data found. Starting fresh.\n");
    return 0;
  }

  fread(&list->count, sizeof(int), 1, file);
  if (list->count > 0)
  {
    while (list->count > list->capacity)
    {
      resizeVisitorList(list);
    }
    fread(list->visitors, sizeof(Visitor), list->count, file);
  }
  fclose(file);
  printf(" Loaded %d visitors from file.\n", list->count);
  return 1;
}

void displayMenu()
{
  printf("\n=========== MENU ===========\n");
  printf("1. Add New Visitor\n");
  printf("2. View All Visitors\n");
  printf("3. Save Data \n");
  printf("4. Exit\n");
  printf("============================\n");
  printf("Enter your choice (1-4): ");
}

int getValidChoice()
{
  int choice;
  while (scanf("%d", &choice) != 1 || choice < 1 || choice > 4)
  {
    printf("Please enter a number between 1-4: ");
    clearInputBuffer();
  }
  clearInputBuffer();
  return choice;
}

void clearInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n')
    ;
}

int isValidPhone(const char *phone)
{
  int len = strlen(phone);
  if (len < 10 || len > 15)
    return 0;

  for (int i = 0; i < len; i++)
  {
    if (!isdigit(phone[i]) && phone[i] != '+' && phone[i] != '-' && phone[i] != ' ')
    {
      return 0;
    }
  }
  return 1;
}

void getCurrentTime(char *timeStr)
{
  time_t now = time(NULL);
  struct tm *local = localtime(&now);
  strftime(timeStr, 20, "%Y-%m-%d %H:%M:%S", local);
}

// Output-- VISITOR MANAGEMENT SYSTEM

/*
No existing data found. Starting fresh.

=== VISITOR MANAGEMENT SYSTEM ===
==================================

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data to file
4. Exit
============================
Enter your choice (1-4): 1

--- Add New Visitor ---
Enter visitor name: Kureshu pujari
Enter phone number: 7893285837

 Visitor added successfully!

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data to file
4. Exit
============================
Enter your choice (1-4): 1

--- Add New Visitor ---
Enter visitor name: Kalyni Kumari
Enter phone number: 7943258384

 Visitor added successfully!

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data to file
4. Exit
============================
Enter your choice (1-4): 1

--- Add New Visitor ---
Enter visitor name: Pravanjan Khutia
Enter phone number: 7943253290

 Visitor added successfully!

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data to file
4. Exit
============================
Enter your choice (1-4): 2

=== VISITOR REGISTER (Total: 3) ===
ID   | NAME                 | PHONE           | VISIT TIME
----------------------------------------------------------
1    | Kureshu pujari       | 7893285837      | 2026-01-13 22:42:36
2    | Kalyni Kumari        | 7943258384      | 2026-01-13 22:43:08
3    | Pravanjan Khutia     | 7943253290      | 2026-01-13 22:43:38

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data
4. Exit
============================
Enter your choice (1-4): 3

 Data saved successfully!

=========== MENU ===========
1. Add New Visitor
2. View All Visitors
3. Save Data to file
4. Exit
============================
Enter your choice (1-4): 4

 Thank you for using Visitor Management System!
*/