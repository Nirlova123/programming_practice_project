//=================================================================//
//                  Corporate Advanced Calculator                  //
//=================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_HISTORY 100
#define MAX_LINE 256
#define ENTRY_LEN 200

// -------------------- Global Calculator State -------------------- //
static double last_answer = 0.0;
static double memory_value = 0.0;
static int memory_set = 0;

static char history[MAX_HISTORY][ENTRY_LEN];
static int history_count = 0;

// -------------------- Utility Functions -------------------- //

int read_line(char *buffer, size_t size)
{
  if (!fgets(buffer, (int)size, stdin))
    return 0;

  buffer[strcspn(buffer, "\n")] = '\0'; // remove newline
  return 1;
}

int parse_double(const char *input, double *out)
{
  if (strcmp(input, "ans") == 0)
  {
    *out = last_answer;
    return 1;
  }
  if (strcmp(input, "mem") == 0)
  {
    if (!memory_set)
    {
      printf("⚠ Memory is empty.\n");
      return 0;
    }
    *out = memory_value;
    return 1;
  }

  char *end;
  double val = strtod(input, &end);
  if (end == input)
    return 0;

  *out = val;
  return 1;
}

void add_history(const char *entry)
{
  if (history_count < MAX_HISTORY)
    strncpy(history[history_count++], entry, ENTRY_LEN - 1);
}

void show_history()
{
  if (history_count == 0)
  {
    printf("📜 No calculation history available.\n");
    return;
  }

  printf("\n---------- Calculation History ----------\n");
  for (int i = 0; i < history_count; i++)
    printf("%2d) %s\n", i + 1, history[i]);
  printf("-----------------------------------------\n");
}

void save_history(const char *filename)
{
  FILE *fp = fopen(filename, "w");
  if (!fp)
  {
    printf("❌ Unable to save history.\n");
    return;
  }

  for (int i = 0; i < history_count; i++)
    fprintf(fp, "%s\n", history[i]);

  fclose(fp);
  printf("✅ History saved to '%s'\n", filename);
}

// -------------------- Math Operations -------------------- //

long long factorial(long long n)
{
  if (n < 0)
    return -1;

  long long result = 1;
  for (long long i = 1; i <= n; i++)
  {
    result *= i;
    if (result < 0)
      return -1;
  }
  return result;
}

// -------------------- Menu Display -------------------- //

void show_menu()
{
  printf("\n========= ADVANCED CALCULATOR =========\n");
  printf("1.  Addition (a + b)\n");
  printf("2.  Subtraction (a - b)\n");
  printf("3.  Multiplication (a * b)\n");
  printf("4.  Division (a / b)\n");
  printf("5.  Modulus (int %% int)\n");
  printf("6.  Power (a ^ b)\n");
  printf("7.  Factorial (n!)\n");
  printf("8.  Square Root\n");
  printf("9.  Percentage\n");
  printf("10. Memory (Store / Recall / Clear)\n");
  printf("11. Show History\n");
  printf("12. Save History to File\n");
  printf("13. Clear History\n");
  printf("14. Exit\n");
  printf("=======================================\n");
  printf("Enter choice: ");
}

// -------------------- Main Application -------------------- //

int main()
{
  char input[MAX_LINE];
  int choice;

  while (1)
  {
    show_menu();
    if (!read_line(input, sizeof(input)))
      continue;

    if (sscanf(input, "%d", &choice) != 1)
    {
      printf("❌ Invalid choice.\n");
      continue;
    }

    double a, b, result;
    char log_entry[ENTRY_LEN];

    switch (choice)
    {
    case 1: // Addition
      printf("Enter a (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;

      printf("Enter b (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b))
        break;

      result = a + b;
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f + %.6f = %.6f", a, b, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 2: // Subtraction
      printf("Enter a (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;

      printf("Enter b (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b))
        break;

      result = a - b;
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f - %.6f = %.6f", a, b, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 3: // Multiplication
      printf("Enter a (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;
      printf("Enter b (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b))
        break;
      result = a * b;
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f * %.6f = %.6f", a, b, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 4: // Division
      printf("Enter numerator: ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;

      printf("Enter denominator: ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b) || b == 0)
      {
        printf("❌ Division by zero.\n");
        break;
      }

      result = a / b;
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f / %.6f = %.6f", a, b, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 5: // Modulus
      printf("Enter integer a: ");
      read_line(input, sizeof(input));
      int int_a = atoi(input);
      printf("Enter integer b: ");
      read_line(input, sizeof(input));
      int int_b = atoi(input);
      if (int_b == 0)
      {
        printf("❌ Division by zero.\n");
        break;
      }
      int int_result = int_a % int_b;
      printf("Result = %d\n", int_result);
      snprintf(log_entry, ENTRY_LEN, "%d %% %d = %d", int_a, int_b, int_result);
      add_history(log_entry);
      last_answer = (double)int_result;
      break;
    case 6: // Power
      printf("Enter base a (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;
      printf("Enter exponent b (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b))
        break;
      result = pow(a, b);
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f ^ %.6f = %.6f", a, b, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 7: // Factorial
      printf("Enter integer n: ");
      read_line(input, sizeof(input));

      long long n = atoll(input);
      long long f = factorial(n);

      if (f < 0)
      {
        printf("❌ Invalid or overflow factorial.\n");
        break;
      }

      printf("Result = %lld\n", f);
      snprintf(log_entry, ENTRY_LEN, "%lld! = %lld", n, f);
      add_history(log_entry);
      last_answer = (double)f;
      break;
    case 8: // Square Root
      printf("Enter number (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a) || a < 0)
      {
        printf("❌ Invalid input for square root.\n");
        break;
      }
      result = sqrt(a);
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "sqrt(%.6f) = %.6f", a, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 9: // Percentage
      printf("Enter total value (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &a))
        break;
      printf("Enter percentage (or ans/mem): ");
      read_line(input, sizeof(input));
      if (!parse_double(input, &b))
        break;
      result = (a * b) / 100.0;
      printf("Result = %.6f\n", result);
      snprintf(log_entry, ENTRY_LEN, "%.6f%% of %.6f = %.6f", b, a, result);
      add_history(log_entry);
      last_answer = result;
      break;
    case 10: // Memory
      printf("1) Store ans  2) Recall  3) Clear\n");
      read_line(input, sizeof(input));

      if (strcmp(input, "1") == 0)
      {
        memory_value = last_answer;
        memory_set = 1;
        printf("✅ Stored %.6f in memory\n", memory_value);
      }
      else if (strcmp(input, "2") == 0 && memory_set)
        printf("Memory = %.6f\n", memory_value);
      else if (strcmp(input, "3") == 0)
      {
        memory_set = 0;
        printf("🗑 Memory cleared\n");
      }
      else
        printf("❌ Invalid memory option\n");
      break;

    case 11:
      show_history();
      break;

    case 12:
      save_history("calculator_history.txt");
      break;

    case 13:
      history_count = 0;
      printf("🗑 History cleared\n");
      break;

    case 14:
      printf("Confirm exit? (y/n): ");
      read_line(input, sizeof(input));
      if (tolower(input[0]) == 'y')
      {
        printf("👋 Goodbye!\n");
        return 0;
      }
      break;

    default:
      printf("❌ Feature not implemented yet.\n");
    }
  }
}

// Output :
/*
========= ADVANCED CALCULATOR =========
1.  Addition (a + b)
2.  Subtraction (a - b)
3.  Multiplication (a * b)
4.  Division (a / b)
5.  Modulus (int % int)
6.  Power (a ^ b)
7.  Factorial (n!)
8.  Square Root
9.  Percentage
10. Memory (Store / Recall / Clear)
11. Show History
12. Save History to File
13. Clear History
14. Exit

=======================================
Enter choice: 7
Enter integer n: 5
Result = 120

=======================================
Enter choice: 11

---------- Calculation History ---------
 1) 5! = 120
----------------------------------------

=======================================
Enter choice: 6
Enter base a (or ans/mem): 2
Enter exponent b (or ans/mem): 10
Result = 1024.000000

*/
