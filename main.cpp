/*
 Jake Busson
 Computer Science
 Due: Dec. 17, 2024
 Lab Name: Lab 7 Battleship
 Lab Description: In lab 7 I was asked to make a working battleship gameplay against the computer. With the computer throwing out random shots at my placed ships. I think this is very intesting how you can make anything with C++ and actually be able to play it. While making the code, there were a few adjustments I had to make over time to make sure you couldn't place the ship acorss or off the board and make it impossible to win. I had fun with this lab and being able to use everyhting that we have learned throughout the semester. 
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

const int boardsize = 10;
typedef char board[boardsize][boardsize][2]; // 2nd index stores ship type

// Function prototypes
void iniboard(board Board);
void outboard(board Board, bool hideShips);
void Ushipplacement(board uFriendly);
void UserChoice(int &x, int &y);
void cShipplacement(board cFriendly);
bool userAttack(board cFriendly, board uEnemy);
bool computerAttack(board uFriendly, board cEnemy);
bool checkWin(board Friendly);
bool isShipSunk(board Board, int Shiptype);

// Main function
int main() {
    board uFriendly;  // User's ship board
    board uEnemy;     // User's attack board
    board cFriendly;  // Computer's ship board
    board cEnemy;     // Computer's attack board

    // Initialize boards
    iniboard(uFriendly);
    iniboard(uEnemy);
    iniboard(cFriendly);
    iniboard(cEnemy);

    cout << "Welcome to Battleship!\n";

    // User places ships
    cout << "User places ships:\n";
    Ushipplacement(uFriendly);

    // Computer places ships
    cout << "\nComputer is placing ships...\n";
    cShipplacement(cFriendly);

    // Main gameplay loop
    bool userTurn = true;
    bool gameOver = false;

    while (!gameOver) {
        cout << "\nUser's Ship Board:\n";
        outboard(uFriendly, false);

        cout << "\nUser's Attack Board:\n";
        outboard(uEnemy, true);

        if (userTurn) {
            cout << "\nYour turn to attack:\n";
            if (userAttack(cFriendly, uEnemy)) {
                cout << "You hit a ship!\n";
                // Check if the ship is sunk
                if (isShipSunk(cFriendly, 5)) {
                    cout << "You sunk the Carrier!\n";
                }
                if (isShipSunk(cFriendly, 4)) {
                    cout << "You sunk the Battleship!\n";
                }
                if (isShipSunk(cFriendly, 3)) {
                    cout << "You sunk the Cruiser!\n";
                }
                if (isShipSunk(cFriendly, 2)) {
                    cout << "You sunk the Submarine!\n";
                }
                if (isShipSunk(cFriendly, 1)) {
                    cout << "You sunk the Destroyer!\n";
                }
            } else {
                cout << "You missed!\n";
            }
        } else {
            cout << "\nComputer's turn to attack:\n";
            if (computerAttack(uFriendly, cEnemy)) {
                cout << "Computer hit your ship!\n";
            } else {
                cout << "Computer missed!\n";
            }
        }

        // Check for win conditions
        if (checkWin(cFriendly)) {
            cout << "\nCongratulations! You sank all enemy ships and won the game!\n";
            gameOver = true;
        } else if (checkWin(uFriendly)) {
            cout << "\nGame over! The computer sank all your ships. You lose.\n";
            gameOver = true;
        }

        userTurn = !userTurn;
    }

    return 0;
}

// Initialize the board with '~' to represent water
void iniboard(board Board) {
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
            Board[i][j][0] = '~'; // Display character
            Board[i][j][1] = '0'; // Ship type
        }
    }
}

// Output the board to the console
void outboard(board Board, bool hideShips) {
    cout << "    ";
    for (int col = 1; col <= boardsize; col++) {
        cout << col << " ";
    }
    cout << endl;

    for (int i = 0; i < boardsize; i++) {
        cout << setw(3) << i + 1 << " ";
        for (int j = 0; j < boardsize; j++) {
            if (hideShips && Board[i][j][0] == '#') {
                cout << "~ "; // Hide ships
            } else {
                cout << Board[i][j][0] << " ";
            }
        }
        cout << endl;
    }
}

// User places ships on their board
void Ushipplacement(board uFriendly) {
    int Shipsize = 0, choice, Shiptype, i, j;

    for (int c = 1; c <= 5; c++) {
        bool validPlacement = false;
        switch (c) {
        case 1:
            Shipsize = 5;
            Shiptype = 5;
            cout << "You are placing Carrier (5 spaces)\n";
            break;
        case 2:
            Shipsize = 4;
            Shiptype = 4;
            cout << "You are placing Battleship (4 spaces)\n";
            break;
        case 3:
            Shipsize = 3;
            Shiptype = 3;
            cout << "You are placing Cruiser (3 spaces)\n";
            break;
        case 4:
            Shipsize = 3;
            Shiptype = 2;
            cout << "You are placing Submarine (3 spaces)\n";
            break;
        case 5:
            Shipsize = 2;
            Shiptype = 1;
            cout << "You are placing Destroyer (2 spaces)\n";
            break;
        }

        while (!validPlacement) {
            UserChoice(j, i);
            cout << "Place the ship up(1), down(2), right(3), or left(4) from current point? ";
            cin >> choice;

            validPlacement = true;
            int x = i, y = j;

            // Check placement validity
            for (int a = 0; a < Shipsize; a++) {
                if (x < 0 || x >= boardsize || y < 0 || y >= boardsize || uFriendly[y][x][0] == '#') {
                    cout << "Invalid placement. Ships overlap or out of bounds. Retry.\n";
                    validPlacement = false;
                    break;
                }
                if (choice == 1) y--;
                else if (choice == 2) y++;
                else if (choice == 3) x++;
                else if (choice == 4) x--;
            }

            if (validPlacement) {
                x = i, y = j;
                for (int a = 0; a < Shipsize; a++) {
                    uFriendly[y][x][0] = '#';
                    uFriendly[y][x][1] = Shiptype;
                    if (choice == 1) y--;
                    else if (choice == 2) y++;
                    else if (choice == 3) x++;
                    else if (choice == 4) x--;
                }
            }
        }
        outboard(uFriendly, false);
    }
}

// Get user coordinates for attacking or ship placement
void UserChoice(int &x, int &y) {
    do {
        cout << "Enter an x coordinate (1-10): ";
        cin >> x;
        cout << "Enter a y coordinate (1-10): ";
        cin >> y;

        if (x < 1 || x > 10 || y < 1 || y > 10) {
            cout << "Invalid coordinates. Please enter values between 1 and 10.\n";
        }
    } while (x < 1 || x > 10 || y < 1 || y > 10);

    x--;
    y--;
}

// Computer places ships on its board
void cShipplacement(board cFriendly) {
    int Shipsize = 0, choice, Shiptype, i, j;

    srand(time(NULL));

    for (int c = 1; c <= 5; c++) {
        switch (c) {
        case 1: Shipsize = 5; Shiptype = 5; break;
        case 2: Shipsize = 4; Shiptype = 4; break;
        case 3: Shipsize = 3; Shiptype = 3; break;
        case 4: Shipsize = 3; Shiptype = 2; break;
        case 5: Shipsize = 2; Shiptype = 1; break;
        }

        bool validPlacement = false;

        while (!validPlacement) {
            i = rand() % boardsize;
            j = rand() % boardsize;
            choice = 1 + rand() % 4;

            validPlacement = true;
            int x = i, y = j;

            for (int a = 0; a < Shipsize; a++) {
                if (x < 0 || x >= boardsize || y < 0 || y >= boardsize || cFriendly[y][x][0] == '#') {
                    validPlacement = false;
                    break;
                }
                if (choice == 1) y--;
                else if (choice == 2) y++;
                else if (choice == 3) x++;
                else if (choice == 4) x--;
            }

            if (validPlacement) {
                x = i, y = j;
                for (int a = 0; a < Shipsize; a++) {
                    cFriendly[y][x][0] = '#';
                    cFriendly[y][x][1] = Shiptype;
                    if (choice == 1) y--;
                    else if (choice == 2) y++;
                    else if (choice == 3) x++;
                    else if (choice == 4) x--;
                }
            }
        }
    }
}

// User attacks the computer
bool userAttack(board cFriendly, board uEnemy) {
    int x, y;
    UserChoice(x, y);

    if (uEnemy[y][x][0] == 'H' || uEnemy[y][x][0] == 'M') {
        cout << "You've already attacked this location. Try again.\n";
        return userAttack(cFriendly, uEnemy);
    }

    if (cFriendly[y][x][0] == '#') {
        uEnemy[y][x][0] = 'H';
        cFriendly[y][x][0] = 'H';
        return true;
    } else {
        uEnemy[y][x][0] = 'M';
        return false;
    }
}

// Computer attacks the user
bool computerAttack(board uFriendly, board cEnemy) {
    int x = rand() % boardsize;
    int y = rand() % boardsize;

    while (cEnemy[y][x][0] == 'H' || cEnemy[y][x][0] == 'M') {
        x = rand() % boardsize;
        y = rand() % boardsize;
    }

    if (uFriendly[y][x][0] == '#') {
        cEnemy[y][x][0] = 'H';
        uFriendly[y][x][0] = 'H';
        return true;
    } else {
        cEnemy[y][x][0] = 'M';
        return false;
    }
}

// Check if all ships are sunk
bool checkWin(board Friendly) {
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
            if (Friendly[i][j][0] == '#') {
                return false;
            }
        }
    }
    return true;
}

// Check if a specific ship is sunk
bool isShipSunk(board Board, int Shiptype) {
    for (int i = 0; i < boardsize; i++) {
        for (int j = 0; j < boardsize; j++) {
            if (Board[i][j][1] == Shiptype && Board[i][j][0] != 'H') {
                return false;
            }
        }
    }
    return true;
}
