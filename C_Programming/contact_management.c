/*
 * Project : Contact Management System
 * Purpose : Store, search, update and manage contacts
 * Author  : Suresh Pujari
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NAME_LEN 30
#define MAX_PHONE_LEN 15
#define INITIAL_CAPACITY 10
#define DATA_FILE "contacts.dat"

typedef enum
{
  false,
  true
} bool;

/* ===================== Structures ============================== */

typedef struct
{
  char name[MAX_NAME_LEN];
  char phone[MAX_PHONE_LEN];
} Contact;

typedef struct
{
  Contact *data;
  int count;
  int capacity;
} ContactList;

/* ===================== Function Prototypes ===================== */

void initList(ContactList *);
void loadFromFile(ContactList *);
void saveToFile(const ContactList *);
void addContact(ContactList *);
void displayContacts(const ContactList *);
void searchContact(const ContactList *);
void updateContact(ContactList *);
void deleteContact(ContactList *);
bool validatePhone(const char *);
void resizeList(ContactList *);
void clearInputBuffer(void);
void showMenu(void);

/* ===================== Main ===================== */

int main(void)
{
  ContactList list;
  int choice;

  initList(&list);
  loadFromFile(&list);

  while (1)
  {
    showMenu();
    if (scanf("%d", &choice) != 1)
    {
      clearInputBuffer();
      continue;
    }
    clearInputBuffer();

    switch (choice)
    {
    case 1:
      addContact(&list);
      break;
    case 2:
      displayContacts(&list);
      break;
    case 3:
      searchContact(&list);
      break;
    case 4:
      updateContact(&list);
      break;
    case 5:
      deleteContact(&list);
      break;
    case 6:
      saveToFile(&list);
      printf("Exiting... Data saved.\n");
      free(list.data);
      return 0;
    default:
      printf("Invalid option!\n");
    }
  }
}

/* ===================== Core Functions ===================== */

void initList(ContactList *list)
{
  list->count = 0;
  list->capacity = INITIAL_CAPACITY;
  list->data = (Contact *)malloc(sizeof(Contact) * list->capacity);
  if (!list->data)
  {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
}

void resizeList(ContactList *list)
{
  list->capacity *= 2;
  Contact *temp = (Contact *)realloc(list->data, sizeof(Contact) * list->capacity);
  if (!temp)
  {
    printf("Memory resize failed\n");
    return;
  }
  list->data = temp;
}

void addContact(ContactList *list)
{
  if (list->count == list->capacity)
    resizeList(list);

  Contact *c = &list->data[list->count];

  printf("Enter Name : ");
  fgets(c->name, MAX_NAME_LEN, stdin);
  c->name[strcspn(c->name, "\n")] = '\0';

  do
  {
    printf("Enter Phone : ");
    fgets(c->phone, MAX_PHONE_LEN, stdin);
    c->phone[strcspn(c->phone, "\n")] = '\0';
  } while (!validatePhone(c->phone));

  list->count++;
  printf("Contact added successfully!\n");
}

void displayContacts(const ContactList *list)
{
  if (list->count == 0)
  {
    printf("No contacts available.\n");
    return;
  }

  printf("\n------ Contact List ------\n");
  for (int i = 0; i < list->count; i++)
  {
    printf("%d. %s | %s\n",
           i + 1, list->data[i].name, list->data[i].phone);
  }
}

void searchContact(const ContactList *list)
{
  char key[MAX_NAME_LEN];
  printf("Search Name : ");
  fgets(key, MAX_NAME_LEN, stdin);
  key[strcspn(key, "\n")] = '\0';

  for (int i = 0; i < list->count; i++)
  {
    if (strcasecmp(list->data[i].name, key) == 0)
    {
      printf("Found: %s - %s\n",
             list->data[i].name, list->data[i].phone);
      return;
    }
  }
  printf("Contact not found.\n");
}

void updateContact(ContactList *list)
{
  int index = -1;
  char key[MAX_NAME_LEN];
  printf("Enter Name : ");
  fgets(key, MAX_NAME_LEN, stdin);
  key[strcspn(key, "\n")] = '\0';
  for (int i = 0; i < list->count; i++)
  {
    if (strcasecmp(list->data[i].name, key) == 0)
    {
      index = i;
      break;
    }
  }

  if (index == -1)
    return;

  char name[MAX_NAME_LEN];
  char phoneNumber[MAX_PHONE_LEN];
  int i, flag = 0;

  printf("\nEnter New Name (if you want to change it otherwise skip)         :: ");
  fgets(name, MAX_NAME_LEN, stdin);
  if (!((i = strcspn(name, "\n")) < 1))
  {
    name[i] = 0;
    strcpy(list->data[index].name, name);
    flag = 1;
  }
  printf("Enter New Phone Number (if you want to change it otherwise skip) ::");
  fgets(phoneNumber, MAX_PHONE_LEN, stdin);
  if (!((i = strcspn(phoneNumber, "\n")) < 1))
  {
    phoneNumber[i] = 0;
    strcpy(list->data[index].phone, phoneNumber);
    flag = 1;
  }

  if (flag)
  {
    printf("Changes are saved successfully\n");
  }
  else
  {
    printf("No changes made\n");
  }
}

void deleteContact(ContactList *list)
{
  int index;
  printf("\nEnter contact number to delete: ");
  scanf("%d", &index);
  clearInputBuffer();

  if (index < 1 || index > list->count)
  {
    printf("Invalid index\n");
    return;
  }

  for (int i = index - 1; i < list->count - 1; i++)
    list->data[i] = list->data[i + 1];

  list->count--;
  printf("Contact deleted successfully\n");
}

/* ===================== File Handling ===================== */

void saveToFile(const ContactList *list)
{
  FILE *fp = fopen(DATA_FILE, "wb");
  if (!fp)
    return;

  fwrite(&list->count, sizeof(int), 1, fp);
  fwrite(list->data, sizeof(Contact), list->count, fp);
  fclose(fp);
}

void loadFromFile(ContactList *list)
{
  FILE *fp = fopen(DATA_FILE, "rb");
  if (!fp)
    return;

  fread(&list->count, sizeof(int), 1, fp);
  fread(list->data, sizeof(Contact), list->count, fp);
  fclose(fp);
}

/* ===================== Utilities ===================== */

bool validatePhone(const char *phone)
{
  for (int i = 0; phone[i]; i++)
    if (!isdigit(phone[i]))
      return false;
  return true;
}

void clearInputBuffer(void)
{
  while (getchar() != '\n')
    ;
}

void showMenu(void)
{
  printf("\n==== Contact Management ====\n");
  printf("1. Add Contact\n");
  printf("2. View Contacts\n");
  printf("3. Search Contact\n");
  printf("4. Update Contact\n");
  printf("5. Delete Contact\n");
  printf("6. Exit\n\n");
  printf("Choose option: ");
}

// OUTPUT-
/*
==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 1

Enter Name : Nirlova panda
Enter Phone : 7832019492
Contact added successfully!

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 2

------ Contact List ---------
1. Nirlova panda | 7832019492

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 1

Enter Name : Suresh pujari
Enter Phone : 4839289381
Contact added successfully!

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 2

------ Contact List ------
1. Nirlova panda | 7832019492
2. Suresh pujari | 4839289381

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 3

Search Name : NIRLOVA PANDA
Found: Nirlova panda - 7832019492

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 4

Enter Name : SURESH PUJARI
Enter New Name (if you want to change it otherwise skip)         ::
Enter New Phone Number (if you want to change it otherwise skip) ::6793284930
Changes are saved successfully

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 2

------ Contact List ------
1. Nirlova panda | 7832019492
2. Suresh pujari | 6793284930

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 1

Enter Name : Jasbant sing
Enter Phone : 8943564839
Contact added successfully!

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit
Choose option: 2

------ Contact List ------
1. Nirlova panda | 7832019492
2. Suresh pujari | 6793284930
3. Jasbant sing | 8943564839

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 2

------ Contact List ------
1. Nirlova panda | 7832019492
2. Suresh pujari | 6793284930
3. Jasbant sing | 8943564839

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 5

Enter contact number to delete: 3
Contact deleted successfully

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit

Choose option: 2

------ Contact List ------
1. Nirlova panda | 7832019492
2. Suresh pujari | 6793284930

==== Contact Management ====
1. Add Contact
2. View Contacts
3. Search Contact
4. Update Contact
5. Delete Contact
6. Exit
Choose option: 6
Exiting... Data saved.

*/