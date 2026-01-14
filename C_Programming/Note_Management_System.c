//===============================================================================//
//                             NOTE MANAGEMENT SYSTEM                            //
//===============================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define INITIAL_CAPACITY 5
#define MAX_TITLE_LEN 50
#define MAX_CONTENT_LEN 300
#define MAX_CATEGORY_LEN 30
#define TIME_LEN 20
#define FILENAME "notes.dat"

/* ================= DATA STRUCTURES ================= */

typedef struct
{
  char title[MAX_TITLE_LEN];
  char category[MAX_CATEGORY_LEN];
  char content[MAX_CONTENT_LEN];
  char created_at[TIME_LEN];
  int priority;
} Note;

typedef struct
{
  Note *notes;
  int count;
  int capacity;
} NoteList;

/* ================= FUNCTION PROTOTYPES ================= */

void initNoteList(NoteList *list);
void freeNoteList(NoteList *list);
void resizeNoteList(NoteList *list);

void addNote(NoteList *list);
void displayAllNotes(NoteList *list);
void displayNotesByCategory(NoteList *list);

void saveToFile(NoteList *list);
void loadFromFile(NoteList *list);

void displayMenu();
int getChoice();
void clearBuffer();
void getCurrentTime(char *timeStr);

/* ================= MAIN ================= */

int main()
{
  NoteList notes;
  int choice;

  initNoteList(&notes);
  loadFromFile(&notes);

  printf("\n=== NOTE MANAGEMENT SYSTEM ===\n");

  while (1)
  {
    displayMenu();
    choice = getChoice();

    switch (choice)
    {
    case 1:
      addNote(&notes);
      saveToFile(&notes);
      break;

    case 2:
      displayAllNotes(&notes);
      break;

    case 3:
      displayNotesByCategory(&notes);
      break;

    case 4:
      saveToFile(&notes);
      freeNoteList(&notes);
      printf("\nExiting... Goodbye!\n");
      return 0;

    default:
      printf("Invalid choice!\n");
    }
  }
}

/* ================= IMPLEMENTATION ================= */

void initNoteList(NoteList *list)
{
  list->capacity = INITIAL_CAPACITY;
  list->count = 0;
  list->notes = (Note *)malloc(sizeof(Note) * list->capacity);
  if (list->notes == NULL)
  {
    printf("Memory allocation failed try again!\n");
    exit(1);
  }
}

void resizeNoteList(NoteList *list)
{
  list->capacity *= 2;
  Note *temp = (Note *)realloc(list->notes, sizeof(Note) * list->capacity);
  if (temp == NULL)
  {
    printf("Memory allocation failed for during resize!\n");
    return;
  }
  list->notes = temp;
}

void addNote(NoteList *list)
{
  if (list->count >= list->capacity)
    resizeNoteList(list);

  Note *n = &list->notes[list->count];

  printf("\nEnter title: ");
  fgets(n->title, MAX_TITLE_LEN, stdin);
  n->title[strcspn(n->title, "\n")] = 0;

  printf("Enter category: ");
  fgets(n->category, MAX_CATEGORY_LEN, stdin);
  n->category[strcspn(n->category, "\n")] = 0;

  printf("Enter content: ");
  fgets(n->content, MAX_CONTENT_LEN, stdin);
  n->content[strcspn(n->content, "\n")] = 0;

  printf("Priority (1=High, 2=Medium, 3=Low): ");
  scanf("%d", &n->priority);
  clearBuffer();

  getCurrentTime(n->created_at);

  list->count++;
  printf("\nNote added successfully!\n");
}

void displayAllNotes(NoteList *list)
{
  if (list->count == 0)
  {
    printf("\nNo notes available.\n");
    return;
  }

  printf("\n=== ALL NOTES ===\n");
  for (int i = 0; i < list->count; i++)
  {
    Note *n = &list->notes[i];
    printf("\n[%d] %s (%s)\n", i + 1, n->title, n->category);
    printf("Priority: %d\n", n->priority);
    printf("Created: %s\n", n->created_at);
    printf("Content: %s\n", n->content);
  }
}

void displayNotesByCategory(NoteList *list)
{
  char category[MAX_CATEGORY_LEN];
  int found = 0;

  printf("\nEnter category to search: ");
  fgets(category, MAX_CATEGORY_LEN, stdin);
  category[strcspn(category, "\n")] = 0;

  for (int i = 0; i < list->count; i++)
  {
    if (strcmp(list->notes[i].category, category) == 0)
    {
      Note *n = &list->notes[i];
      printf("\n%s - %s\n", n->title, n->created_at);
      printf("%s\n", n->content);
      found = 1;
    }
  }

  if (!found)
    printf("\nNo notes found in this category.\n");
}

void saveToFile(NoteList *list)
{
  FILE *fp = fopen(FILENAME, "wb");
  if (!fp)
  {
    printf("Failed to save data!\n");
    return;
  }
  fwrite(&list->count, sizeof(int), 1, fp);
  fwrite(list->notes, sizeof(Note), list->count, fp);
  fclose(fp);
}

void loadFromFile(NoteList *list)
{
  FILE *fp = fopen(FILENAME, "rb");
  if (!fp)
  {
    printf("Failed to load file!\n");
    return;
  }

  fread(&list->count, sizeof(int), 1, fp);
  while (list->count > list->capacity)
    resizeNoteList(list);

  fread(list->notes, sizeof(Note), list->count, fp);
  fclose(fp);
}

void displayMenu()
{
  printf("\n1. Add Note");
  printf("\n2. View All Notes");
  printf("\n3. View Notes by Category");
  printf("\n4. Exit");
  printf("\nEnter choice: ");
}

int getChoice()
{
  int ch;
  while (scanf("%d", &ch) != 1)
  {
    printf("Enter a valid number between 1-4!\n");
    clearBuffer();
  }
  clearBuffer();
  return ch;
}

void clearBuffer()
{
  while (getchar() != '\n')
    ;
}

void getCurrentTime(char *timeStr)
{
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(timeStr, TIME_LEN, "%Y-%m-%d %H:%M:%S", t);
}

void freeNoteList(NoteList *list)
{
  free(list->notes);
}

// Output
/*
=== NOTE MANAGEMENT SYSTEM ===

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 1

Enter title: Meeting Notes
Enter category: Work
Enter content: Discuss project milestones and deadlines.
Priority (1=High, 2=Medium, 3=Low): 1

Note added successfully!

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 1

Enter title: Grocery List
Enter category: Personal
Enter content: Milk, Bread, Eggs, Fruits.
Priority (1=High, 2=Medium, 3=Low): 3

Note added successfully!

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 1

Enter title: Book to read
Enter category: Leisure
Enter content: Finish reading "The Greate Gatsby
Priority (1=High, 2=Medium, 3=Low): 2

Note added successfully!

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 2

=== ALL NOTES ===

[1] Meeting Notes (Work)
Priority: 1
Created: 2026-01-14 09:55:01
Content: Discuss project milestones and deadlines.

[2] Grocery List (Personal)
Priority: 3
Created: 2026-01-14 09:56:06
Content: Milk, Bread, Eggs, Fruits.

[3] Book to read (Leisure)
Priority: 2
Created: 2026-01-14 09:58:05
Content: Finish reading "The Greate Gatsby

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 3

Enter category to search: Work

Meeting Notes - 2026-01-14 09:55:01
Discuss project milestones and deadlines.

1. Add Note
2. View All Notes
3. View Notes by Category
4. Exit
Enter choice: 4

Exiting... Goodbye!
*/