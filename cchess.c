#include <stdio.h>		// argc, argv
#include <stdlib.h>		// rand, srand
#include <string.h>		// strncmp
#include <time.h>		// time

/*
	Function: createBoard
	Sets up the board for the game by creating a 2d array and populating it with the positon of each piece.
	Takes no parameters.
	Returns a double pointer to the 2d character array.
*/

char ** createBoard()
{
	int height = 9;
	int width = 9;

	// Create and allocate the heap memory required for each row of the array.
	char **board;
	board = (char **)malloc(height * sizeof(int *));

	// Allocate memory for each column in the 2d array.
	for (int i = 0; i < height; ++i)
		board[i] = (char *)malloc(width * sizeof(int));

	// Setup the algebraic notation.
	for (int i = 1; i < 9; ++i)
		board[0][i] = 96 + i;

	for (int i = 1; i < 9; ++i)
		board[i][0] = 57 - i;

	// Populate the board with the starting positions of each piece.
	// Pawns
	for (int i = 1; i < 9; ++i)
	{
		board[2][i] = 'p';
		board[7][i] = 'P';
	}
	// Rooks
	board[1][1] = 'r';
	board[1][8] = 'r';
	board[8][1] = 'R';
	board[8][8] = 'R';
	// Knights
	board[1][2] = 'k';
	board[1][7] = 'k';
	board[8][2] = 'K';
	board[8][7] = 'K';
	// Bishops
	board[1][3] = 'b';
	board[1][6] = 'b';
	board[8][3] = 'B';
	board[8][6] = 'B';
	// Queens
	board[1][4] = 'q';
	board[8][5] = 'Q';
	// Kings
	board[1][5] = 'x';
	board[8][4] = 'X';

	return board;
}

/*
	Function: render
	Prints the board in a easy to understand manner.
	Takes a double pointer to the char array.
	Returns nothing.
*/

void render(char **board)
{
	// Clears the screen.
	system("clear");

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 0; j < 9; ++j)
		{
			char c = board[i][j];

			if (c == 0)
				printf("   ");
			else
				printf(" %c ", board[i][j]);

			printf("%s", "|");
		}
		printf("\n--- --- --- --- --- --- --- --- ---\n");
	}
}

/*
	Function: freeBoard
	Free the heap memory associated with the board.
	Takes parameter double pointer to the board.
	Returns nothing.
*/

void freeBoard(char **board)
{
	// First free the memory associated with each array within the large array.
	for (int i = 0; i < 9; i++)
	{
		free(board[i]);
	}

	// Then free the memory from the mother array.
	free(board);
}

/*
	Function: delay
	A simple function to delay time.
	Takes an amount of milliseconds as a parameter.
	Returns nothing.
*/

void delay(int milliseconds)
{
    long pause;
    clock_t now,then;

    // Determines how long the pause should be as an amount of processor ticks.
    pause = milliseconds*(CLOCKS_PER_SEC/1000);
    now = then = clock();

    // Loops until the correct amout of processor ticks has been achieved.
    while( (now-then) < pause )
        now = clock();
}

/*
	Function: validMove
	Determines if a move is valid.
	Takes parameter string of move.
	Returns either a 1 or 0 depending of if the move is valid.
*/

int validMove(char *move, int turn, char **board)
{
	// Get the integer values for the move.
	int startx = move[0] - 96;
	int starty = 9 - (move[1] - 48);
	int endx = move[2] - 96;
	int endy = 9 - (move[3] - 48);


	// Get the piece being moved.
	char piece = board[starty][startx];

	// Check to see if the start and end positions are valid.
	if (startx < 1 | startx > 8)
		return 0;
	if (starty < 1 | starty > 8)
		return 0;
	if (endx < 1 | endx > 8)
		return 0;
	if (endy < 1 | endy > 8)
		return 0;

	// Check to see if the wrongly owned piece is being moved.
	if (turn % 2 == 0 && (piece < 65 | piece > 90))
		return 0;
	if (turn % 2 == 1 && (piece < 97 | piece > 122))
		return 0;

	// Don't allow friendly fire.
	if (board[starty][startx] >= 65 && board[starty][startx] <= 90 && board[endy][endx] >= 65 && board[endy][endx] <= 90)
		return 0;
	if (board[starty][startx] >= 97 && board[starty][startx] <= 122 && board[endy][endx] >= 97 && board[endy][endx] <= 122)
		return 0;

	// Check if move is valid for a pawn.
	if (piece == 'p')
	{
		// Check to see if attacking.
		if (board[endy][endx] >= 65 && board[endy][endx] <= 90)
		{
			// Only allow diagonal attacks.
			if (endy - starty == 1 && (endx - startx == 1 | endx - startx == -1))
				return 1;
			else
				return 0;
		}
		else
		{
			// Allow two space move from starting position if nothing in the way.
			if (endy - starty == 2 && starty == 2 && board[3][startx] == 0 && endx - startx == 0)
				return 1;
			// Allow standard one space forward move.
			else if (endy - starty == 1 && endx - startx == 0)
				return 1;
			else
				return 0;
		}
	}
	else if (piece == 'P')
	{
		// Check to see if attacking.
		if (board[endy][endx] >= 97 && board[endy][endx] <= 122)
		{
			// Only allow diagonal attacks.
			if (endy - starty == -1 && (endx - startx == 1 | endx - startx == -1))
				return 1;
			else
				return 0;
		}
		else
		{
			// Allow two space move from starting position if nothing in the way.
			if (endy - starty == -2 && starty == 7 && board[6][startx] == 0 && endx - startx == 0)
				return 1;
			// Allow standard one space forward move.
			else if (endy - starty == -1 && endx - startx == 0)
				return 1;
			else
				return 0;
		}
	}

	// Check to see if move is valid for rook.
	else if (piece == 'r' | piece == 'R')
	{
		// Rooks must move horizontally
		if (endy - starty == 0)
		{
			// Check to make sure there are no pieces in between the start and end positions.
			if (endx > startx)
			{
				for (int i = startx + 1; i < endx; i++)
				{
					if (board[starty][i] != 0)
						return 0;
				}
				return 1;
			}
			else
			{
				for (int i = startx - 1; i > endx; i--)
				{
					if (board[starty][i] != 0)
						return 0;
				}
				return 1;
			}
		}
		// or vertically.
		else if (endx - startx == 0)
		{
			// Check to make sure there are no pieces in between the start and end positions.
			if (endy > starty)
			{
				for (int i = starty + 1; i < endy; i++)
				{
					if (board[i][startx] != 0)
						return 0;
				}
				return 1;
			}
			else
			{
				for (int i = starty - 1; i > endy; i--)
				{
					if (board[i][startx] != 0)
						return 0;
				}
				return 1;
			}
		}
		else
			return 0;
	}

	// Check to see if move is valid for a knight.
	else if (piece == 'k' | piece == 'K')
	{
		// Only allow proper knight moves.
		if (endy - starty == 1 && endx - startx == 2)
			return 1;
		else if (endy - starty == 1 && endx - startx == -2)
			return 1;
		else if (endy - starty == -1 && endx - startx == 2)
			return 1;
		else if (endy - starty == -1 && endx - startx == -2)
			return 1;
		else if (endy - starty == 2 && endx - startx == 1)
			return 1;
		else if (endy - starty == 2 && endx - startx == -1)
			return 1;
		else if (endy - starty == -2 && endx - startx == 1)
			return 1;
		else if (endy - starty == -2 && endx - startx == -1)
			return 1;
		else
			return 0;
	}

	// Check to see if move is valid for a bishop.
	else if (piece == 'b' | piece == 'B')
	{
		// Only allow diagonal moves.
		if (endx - startx == endy - starty | endx - startx == -(endy - starty))
		{
			// Get the direction of the move.
			int xdir = endx - startx;
			int ydir = endy - starty;

			// Make sure no pieces are in the way.
			if (xdir > 0 && ydir > 0)
			{
				for (int i = 1; i < endx - startx; ++i)
				{
					if (board[starty+i][startx+i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir > 0 && ydir < 0)
			{
				for (int i = 1; i < endx - startx; ++i)
				{
					if (board[starty-i][startx+i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir < 0 && ydir > 0)
			{
				for (int i = 1; i < -(endx - startx); ++i)
				{
					if (board[starty+i][startx-i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir < 0 && ydir < 0)
			{
				for (int i = 1; i < -(endx - startx); ++i)
				{
					if (board[starty-i][startx-i] != 0)
						return 0;
				}
				return 1;
			}
			else
				return 0;
		}
		else
			return 0;
	}

	// Check to see if a move is valid for a queen.
	else if (piece == 'q' | piece == 'Q')
	{
		// Allow diagonal moves.
		if (endx - startx == endy - starty | endx - startx == -(endy - starty))
		{
			// Get the direction of the move.
			int xdir = endx - startx;
			int ydir = endy - starty;

			// Make sure no pieces are in the way.
			if (xdir > 0 && ydir > 0)
			{
				for (int i = 1; i < endx - startx; ++i)
				{
					if (board[starty+i][startx+i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir > 0 && ydir < 0)
			{
				for (int i = 1; i < endx - startx; ++i)
				{
					if (board[starty-i][startx+i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir < 0 && ydir > 0)
			{
				for (int i = 1; i < -(endx - startx); ++i)
				{
					if (board[starty+i][startx-i] != 0)
						return 0;
				}
				return 1;
			}
			else if (xdir < 0 && ydir < 0)
			{
				for (int i = 1; i < -(endx - startx); ++i)
				{
					if (board[starty-i][startx-i] != 0)
						return 0;
				}
				return 1;
			}
			else
				return 0;
		}
		// or horizontal moves.
		else if (endy - starty == 0)
		{
			// Check to make sure there are no pieces in between the start and end positions.
			if (endx > startx)
			{
				for (int i = startx + 1; i < endx; i++)
				{
					if (board[starty][i] != 0)
						return 0;
				}
				return 1;
			}
			else
			{
				for (int i = startx - 1; i > endx; i--)
				{
					if (board[starty][i] != 0)
						return 0;
				}
				return 1;
			}
		}
		// or vertically.
		else if (endx - startx == 0)
		{
			// Check to make sure there are no pieces in between the start and end positions.
			if (endy > starty)
			{
				for (int i = starty + 1; i < endy; i++)
				{
					if (board[i][startx] != 0)
						return 0;
				}
				return 1;
			}
			else
			{
				for (int i = starty - 1; i > endy; i--)
				{
					if (board[i][startx] != 0)
						return 0;
				}
				return 1;
			}
		}
		else
			return 0;
	}

	// Check to see if move is valid for a king.
	else if (piece == 'x' | piece == 'X')
	{
		if (endx - startx > 1 | endx - startx < -1)
			return 0;
		else if (endy - starty > 1 | endy - starty < -1)
			return 0;
		else
			return 1;
	}

	return 1;
}

/*
	Function: isCheckmate
	Checks to see if a valid move is checkmate
	Takes parameters string of move and double pointer to the board.
	Returns either a 1 or 0 depeding on if it is checkmate.
*/

int isCheckmate(char *move, int turn, char **board)
{
	// Get the integer values for end position of move.
	int endx = move[2] - 96;
	int endy = 9 - (move[3] - 48);

	// Check to see if the end position is the opponents king.
	if (turn % 2 == 0)
	{
		if (board[endy][endx] == 'x')
			return 1;
		else
			return 0;
	}
	else
		if (board[endy][endx] == 'X')
			return 1;
		else
			return 0;
}

/*
	Function: makeMove
	Conducts a valid move.
	Takes parameters string of move and double pointer to the board.
	Returns nothing.
*/

void makeMove(char *move, char **board)
{
	// Get the integer values for the move.
	int startx = move[0] - 96;
	int starty = 9 - (move[1] - 48);
	int endx = move[2] - 96;
	int endy = 9 - (move[3] - 48);
	
	// Conduct the move.
	board[endy][endx] = board[starty][startx];
	board[starty][startx] = 0;
}

/*
	Function: main
	Runs the game then frees all memory.
	Takes standard parameters argc and argv.
	Returns a 0.
*/

// Setup some variables.
int done = 0;
int turn = 0;
char move[4];
char players[1];

int main(int argc, char *argv[])
{
	// Get a random seed.
	srand(time(NULL));

	// Initialize the board.
	char **board = createBoard();

	// Determine if 1-player or 2-player game.
	printf("%s", "How many players? (1-2): ");
	scanf("%s", players);

	// Determine if single player or two player.
	if (strncmp(players, "2", 1) == 0)
	{
		// Run two player game loop until done.
		while (done == 0)
		{
			render(board);

			// Announce whos move it is.
			if (turn % 2 == 0)
				printf("%s", "Capital move: ");
			else
				printf("%s", "Lowercase move: ");

			// Get the move.
			scanf("%s", move);

			if (strncmp(move, "exit", 4) == 0)
				done = 1;
			else
			{
				if (validMove(move, turn, board) == 1)
				{
					if (isCheckmate(move, turn, board) == 1)
					{
						render(board);
						if (turn % 2 == 0)
							printf("%s\n", "Capital wins.");
						else
							printf("%s\n", "Lowercase wins.");
						done = 1;
					}
					else
					{
						makeMove(move, board);
						turn += 1;
					}
				}
			}
		}
	}
	else if (strncmp(players, "1", 1) == 0)
	{
		// Run player vs computer game until done.
		while (done == 0)
		{
			render(board);

			// Determine whos turn it is.
			if (turn % 2 == 0)
			{
				// If it is the players turn, get the desired move.
				printf("%s", "Your move: ");
				
				// Get the move.
				scanf("%s", move);
			}
			else
			{
				// If it is the computers turn, make a random move.
				// Setup some variables.
				int randMove[4];
				move[0] = '?';

				// Generate random moves until a valid one is found.
				while (validMove(move, turn, board) != 1)
				{
					// Make some random integers for the move
					randMove[0] = (rand() % 8) + 97;
					randMove[1] = (rand() % 8) + 49;
					randMove[2] = (rand() % 8) + 97;
					randMove[3] = (rand() % 8) + 49;

					// Combine them to make a move.
					move[0] = (char)randMove[0];
					move[1] = (char)randMove[1];
					move[2] = (char)randMove[2];
					move[3] = (char)randMove[3];
				}

				// Check for checkmate, otherwise make move.
				if (isCheckmate(move, turn, board) == 1)
				{
					render(board);

					if (turn % 2 == 0)
						printf("%s\n", "Player wins.");
					else
						printf("%s\n", "Computer wins.");
					
					done = 1;
				}
				else
				{
					makeMove(move, board);
					turn += 1;
				}
			}

			if (strncmp(move, "exit", 4) == 0)
				done = 1;
			else
			{
				if (validMove(move, turn, board) == 1)
				{
					if (isCheckmate(move, turn, board) == 1)
					{
						render(board);

						if (turn % 2 == 0)
							printf("%s\n", "Player wins.");
						else
							printf("%s\n", "Computer wins.");
						
						done = 1;
					}
					else
					{
						makeMove(move, board);
						turn += 1;
					}
				}
			}
		}
	}
	else if (strncmp(players, "0", 1) == 0)
	{
		//render(board);
		int randMove[4];

		int games = 0, cwins = 0, lwins = 0;

		while (games < 10000)
		{
			board = createBoard();
			done = 0;
			// Play two computers against eachother.
			while (done == 0)
			{
				// Setup some variables.
				// Make a random move to start off. Haven't figured out why this is necessary yet
				// Make some random integers for the move
				randMove[0] = (rand() % 8) + 97;
				randMove[1] = (rand() % 8) + 49;
				randMove[2] = (rand() % 8) + 97;
				randMove[3] = (rand() % 8) + 49;

				// Combine them to make a move.
				move[0] = (char)randMove[0];
				move[1] = (char)randMove[1];
				move[2] = (char)randMove[2];
				move[3] = (char)randMove[3];

				// Generate random moves until a valid one is found.
				while (validMove(move, turn, board) != 1)
				{
					// Make some random integers for the move
					randMove[0] = (rand() % 8) + 97;
					randMove[1] = (rand() % 8) + 49;
					randMove[2] = (rand() % 8) + 97;
					randMove[3] = (rand() % 8) + 49;

					// Combine them to make a move.
					move[0] = (char)randMove[0];
					move[1] = (char)randMove[1];
					move[2] = (char)randMove[2];
					move[3] = (char)randMove[3];
				}

				// Check for checkmate, otherwise make move.
				if (isCheckmate(move, turn, board) == 1)
				{
					//render(board);

					if (turn % 2 == 0)
						//printf("%s\n", "Capital wins.");
						cwins++;
					else
						//printf("%s\n", "Lowercase wins.");
						lwins++;
					
					done = 1;
				}
				else
				{
					makeMove(move, board);
					//render(board);
					turn += 1;
				}

				//delay(100);
			}
			games++;
		}

		printf("Capital wins: %d\n", cwins);
		printf("Lowercase wins: %d\n", lwins);
	}

	freeBoard(board);

	return 0;
}
