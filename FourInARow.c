/*  This is my simple four-in-a-row game coded in C and played through the console with two human players.
 *   
 *  Author: Kyle Nieva
 *  Date: April 2021
*/
#include <stdio.h>
#define BOARD_ROWS 6
#define BOARD_COLS 7
#define WIN_LINE 4 
#define MAX_TURNS BOARD_ROWS * BOARD_COLS

void initialize(char gameBoard[BOARD_ROWS][BOARD_COLS]);
void printBoard(char gameBoard[BOARD_ROWS][BOARD_COLS]);
_Bool playerTurn(char player, char gameBoard[BOARD_ROWS][BOARD_COLS], int turnCount);
_Bool checkWin(char player, char gameBoard[BOARD_ROWS][BOARD_COLS], int discRow, int discCol);



int main(){
    char gameBoard[BOARD_ROWS][BOARD_COLS];
    char player = 'X';
    int turnCount = 1;
    initialize(gameBoard);
    // main game loop
    while(turnCount <= MAX_TURNS) {
        if(playerTurn(player, gameBoard, turnCount)) { // ends game when a player wins from their turn
            printf("Congratulations Player %c, you win the game!\n", player);
            return 0;
        }
        system("clear");
        player = (player == 'X') ? 'O' : 'X';   // switch player
        turnCount++;
    }
    printf("Draw! Game Over.\n");
    return 0;
}

// Fills game board 2D array with ' ' characters
void initialize(char gameBoard[BOARD_ROWS][BOARD_COLS]) {
    system("clear");
    for(int i = 0; i < BOARD_ROWS; i++) {
        for(int j = 0; j < BOARD_COLS; j++) {
            gameBoard[i][j] = ' ';
        }
    }
}

// Prints current state of the game board in human readable format with numbered columns
void printBoard(char gameBoard[BOARD_ROWS][BOARD_COLS]) {
    printf("\n");
    printf(" 1  2  3  4  5  6  7 \n");
    for(int i = 0; i < BOARD_ROWS; i++){
        for(int j = 0; j < BOARD_COLS; j++) {
            printf("[%c]", gameBoard[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Executes player turn to drop disc into board and check if it is game-winning
// Returns true if player wins and false otherwise
_Bool playerTurn(char player, char gameBoard[BOARD_ROWS][BOARD_COLS], int turnCount) {
    int discCol = -1, discRow = 0; // represents location of dropped disc
    // Prompts player for column to drop disc into and checks for invalid inputs like out of bounds or full column
    do {
        printf("Turn %d", turnCount);
        printBoard(gameBoard);
        printf("Player %c: Your turn. Enter which column to drop disc into (1-7): ", player);
        scanf("%d", &discCol);
    } while(--discCol < 0 || discCol > 6 || (gameBoard[0][discCol] != ' ')); 

    system("clear");

    // Searches for last disc dropped into the column
    while(discRow < BOARD_ROWS) {
        if(gameBoard[discRow][discCol] == ' ')
            discRow++;
        else
            break;
    }
    gameBoard[--discRow][discCol] = player;
    printBoard(gameBoard);

    return checkWin(player, gameBoard, discRow, discCol);
}
    
// Check for win in 7 directions around dropped disc
// Returns true if disc forms a line of at least 4 matching discs and false otherwise
_Bool checkWin(char player, char gameBoard[BOARD_ROWS][BOARD_COLS], int discRow, int discCol) {
    // vertical (only down)
    if(discRow <= BOARD_ROWS - WIN_LINE) {
        int count;
        for(count = 1; count < WIN_LINE; count++) {
            if(gameBoard[discRow + count][discCol] != player)
                break;
        }
        if(count >= WIN_LINE)
            return 1;
    }
    
    // list of alignments
    int direction[3][2] = {{0, 1},    // horizontal
                           {1, -1},   // '/' diagonal
                           {1, 1}};   // '\' diagonal

    // Loops through each alignment
    for(int i = 0; i < 3; i++) {
        int countP = 1, countN = 0;
        int rowDir = direction[i][0], colDir = direction[i][1];
        int currRow, currCol;
        // Loops through each direction, positive and negative, along the alignment
        for(int j = 0; j < 2; j++) {
            if(j == 0) {     // + direction
                currRow = discRow + countP*rowDir;
                currCol = discCol + countP*colDir;
            }
            else {     // - direction
                currRow = discRow - (countN + 1)*rowDir;
                currCol = discCol - (countN + 1)*colDir;
            }
            // Counts each matching disc...
            while(countP + countN < WIN_LINE && currRow < BOARD_ROWS && currRow >= 0 && currCol < BOARD_COLS && currCol >= 0) {
                if(gameBoard[currRow][currCol] == player) {
                    if(j == 0){     // ...in the positive ditection
                        countP++;
                        currRow = discRow + countP*rowDir;
                        currCol = discCol + countP*colDir;
                    }
                    else {          // ...in the negative direction
                        countN++;
                        currRow = discRow - (countN + 1)*rowDir;
                        currCol = discCol - (countN + 1)*colDir;
                    }
                }
                else
                    break;
            }
        }
        // Finally counts matches from each direction
        if(countP + countN >= WIN_LINE)
            return 1;
    }
    return 0;
}