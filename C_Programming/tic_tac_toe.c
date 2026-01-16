#include <stdio.h>
#include <stdlib.h>

#define SIZE 3
#define EMPTY ' '

typedef enum
{
  false,
  true
} bool;

/* Function Prototypes */
void initializeBoard(char board[SIZE][SIZE]);
void printBoard(const char board[SIZE][SIZE]);
bool isValidMove(const char board[SIZE][SIZE], int row, int col);
char checkWinner(const char board[SIZE][SIZE]);
bool isDraw(const char board[SIZE][SIZE]);
void clearInputBuffer(void);

/* Initialize board */
void initializeBoard(char board[SIZE][SIZE])
{
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      board[i][j] = EMPTY;
}

/* Display board */
void printBoard(const char board[SIZE][SIZE])
{
  printf("\n    0   1   2\n");
  printf("  -------------\n");
  for (int i = 0; i < SIZE; i++)
  {
    printf("%d |", i);
    for (int j = 0; j < SIZE; j++)
    {
      printf(" %c |", board[i][j]);
    }
    printf("\n  -------------\n");
  }
}

/* Validate move */
bool isValidMove(const char board[SIZE][SIZE], int row, int col)
{
  return (row >= 0 && row < SIZE &&
          col >= 0 && col < SIZE &&
          board[row][col] == EMPTY);
}

/* Check winner */
char checkWinner(const char board[SIZE][SIZE])
{
  for (int i = 0; i < SIZE; i++)
  {
    if (board[i][0] == board[i][1] &&
        board[i][1] == board[i][2] &&
        board[i][0] != EMPTY)
      return board[i][0];

    if (board[0][i] == board[1][i] &&
        board[1][i] == board[2][i] &&
        board[0][i] != EMPTY)
      return board[0][i];
  }

  if (board[0][0] == board[1][1] &&
      board[1][1] == board[2][2] &&
      board[0][0] != EMPTY)
    return board[0][0];

  if (board[0][2] == board[1][1] &&
      board[1][1] == board[2][0] &&
      board[0][2] != EMPTY)
    return board[0][2];

  return EMPTY;
}

/* Check draw */
bool isDraw(const char board[SIZE][SIZE])
{
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE; j++)
      if (board[i][j] == EMPTY)
        return false;
  return true;
}

/* Clear input buffer */
void clearInputBuffer(void)
{
  while (getchar() != '\n')
    ;
}

int main(void)
{
  char board[SIZE][SIZE];
  char playerNames[2][20];
  char symbols[2] = {'X', 'O'};
  int row, col;
  char winner;
  char playAgain;

  printf("=== TIC TAC TOE ===\n");
  printf("Enter Player 1 name (X): ");
  scanf("%19s", playerNames[0]);

  printf("Enter Player 2 name (O): ");
  scanf("%19s", playerNames[1]);

  do
  {
    initializeBoard(board);
    int turn = 0;

    while (true)
    {
      printBoard(board);
      int current = turn % 2;

      printf("%s (%c), enter row & column: ",
             playerNames[current], symbols[current]);

      if (scanf("%d %d", &row, &col) != 2)
      {
        printf("Invalid input. Try again.\n");
        clearInputBuffer();
        continue;
      }

      if (!isValidMove(board, row, col))
      {
        printf("Invalid move. Cell occupied or out of range.\n");
        continue;
      }

      board[row][col] = symbols[current];

      winner = checkWinner(board);
      if (winner != EMPTY)
      {
        printBoard(board);
        printf("\n🎉 %s wins!\n",
               (winner == 'X') ? playerNames[0] : playerNames[1]);
        break;
      }

      if (isDraw(board))
      {
        printBoard(board);
        printf("\n It's a draw!\n");
        break;
      }

      turn++;
    }

    printf("\nPlay again? (y/n): ");
    scanf(" %c", &playAgain);

  } while (playAgain == 'y' || playAgain == 'Y');

  printf("\nThanks for playing!\n");
  return 0;
}

// OUTPUT:
/*
=== TIC TAC TOE ===
Enter Player 1 name (X): Kuresh
Enter Player 2 name (O): Phagal

    0   1   2
  -------------
0 |   |   |   |
  -------------
1 |   |   |   |
  -------------
2 |   |   |   |
  -------------
Kuresh (X), enter row & column: 0 0

    0   1   2
  -------------
0 | X |   |   |
  -------------
1 |   |   |   |
  -------------
2 |   |   |   |
  -------------
Phagal (O), enter row & column: 2 0

    0   1   2
  -------------
0 | X |   |   |
  -------------
1 |   |   |   |
  -------------
2 | O |   |   |
  -------------
Kuresh (X), enter row & column: 0 2

    0   1   2
  -------------
0 | X |   | X |
  -------------
1 |   |   |   |
  -------------
2 | O |   |   |
  -------------
Phagal (O), enter row & column: 2 1

    0   1   2
  -------------
0 | X |   | X |
  -------------
1 |   |   |   |
  -------------
2 | O | O |   |
  -------------
Kuresh (X), enter row & column: 0 1

    0   1   2
  -------------
0 | X | X | X |
  -------------
1 |   |   |   |
  -------------
2 | O | O |   |
  -------------

Kuresh wins!

Play again? (y/n):
*/
