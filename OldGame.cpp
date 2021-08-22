#include "OldGame.h"
#include "LinkedList.h"
#include "Player.h"
#include "Board.h"
#include "Tile.h"
#include "GameInit.h"
#include "GameSaver.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <ios>
#include <limits>
#define ASCII_CONVERTER_DIGIT 48
#define ASCII_CONVERTER_LETTER 65
//Due to implementation of columns, we are operating with cols 1 to 26 for indexing
#define COLUMN_MAX 26
#define COLUMN_MIN 1
#define ROW_MAX 25
#define ROW_MIN 0

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::string;

OldGame::OldGame(Player *player1, Player *player2, LinkedList *bag, Board *board, Player *currentPlayer)
{
    this->player1 = player1;
    this->player2 = player2;
    this->bag = bag;
    this->board = board;
    this->currentPlayer = currentPlayer;
    this->terminateGame = false;
    this->gameOver = false;
}

OldGame::~OldGame()
{
    delete player1;
    delete player2;
    delete bag;
    delete board;
}

void OldGame::executeGameplay()
{
    cout << endl
         << "Let's Play!" << endl
         << endl;
    while (!terminateGame && !gameOver)
    {
        turnNum++;
        string command = "";
        bool correctCommand = false;
        printGameStatus();
        do
        {
            cout << "> ";
            if (!cin.eof())
            {
                getline(cin, command);
                std::vector<string> commandSplit = processCommand(command);
                correctCommand = playTurn(commandSplit);
            }
            //check for EOF character
            else
            {
                terminateGame = true;
            }

        } while (!correctCommand && !terminateGame);

        //only if EOF is not done
        if (!terminateGame)
        {
            switchPlayer();

            if (bag->isEmpty() && (player1->getHand()->isEmpty() || player2->getHand()->isEmpty()))
            {
                gameOver = true;
            }
        }
    }
    if (gameOver == true)
    {
        cin.ignore(std::numeric_limits<char>::max(), '\n');
        cout << endl
             << "Game over" << endl;
        cout << "Score for " << player1->getName() << ": " << player1->getPoints() << endl;
        cout << "Score for " << player2->getName() << ": " << player2->getPoints() << endl;

        if (player1->getPoints() > player2->getPoints())
        {
            cout << "Player " << player1->getName() << " won!" << endl;
        }
        else if (player2->getPoints() > player1->getPoints())
        {
            cout << "Player " << player2->getName() << " won!" << endl;
        }
        else if (player2->getPoints() == player1->getPoints())
        {
            cout << "The game was a draw!" << endl;
        }
    }
    cout << endl
         << "Goodbye" << endl;
}

bool OldGame::playTurn(vector<string> userInput)
{
    bool returnVal = false;

    if (userInput.size() > 0 && userInput.size() < INPUT_SIZE_MAX)
    {
        if (userInput[INPUT_POS_1] == "PLACE" && userInput[INPUT_POS_2] != "" && userInput[INPUT_POS_3] == "AT" && userInput[INPUT_POS_4] != "" && !(userInput[INPUT_POS_2].length() > 2))
        {
            int locationRow = (userInput[INPUT_POS_4].at(0)) - ASCII_CONVERTER_LETTER;
            std::string colVal = userInput[INPUT_POS_4];

            int tileIndex = currentPlayer->getHand()->existsAt(userInput[INPUT_POS_2].at(0), (userInput[INPUT_POS_2].at(1)) - ASCII_CONVERTER_DIGIT);

            //If the LinkedList returns a index for the found tile
            if (tileIndex > -1)
            {
                Tile *tile = currentPlayer->getHand()->get(tileIndex);
                if (colVal.length() >= 2)
                {
                    char temp[2] = {colVal[1], colVal[2]};
                    std::string tempStr = temp;
                    try
                    {
                        int value = std::stoi(tempStr);
                        returnVal = playTile(tile, locationRow, value + 1);
                    }
                    catch (std::invalid_argument &e)
                    {
                        cout << "Location code is invalid!" << endl;
                    }
                }
                else
                {
                    cout << "Location code is invalid!" << endl;
                }
            }
            else
            {
                cout << "That tile is not in your hand. Try again: " << endl;
            }
        }
        else if (userInput[INPUT_POS_1] == "REPLACE" && userInput[INPUT_POS_2] != "" && userInput.size() == 2 && !(userInput[INPUT_POS_2].length() > 2))
        { //user is replacing tile
            int tileIndex = currentPlayer->getHand()->existsAt(userInput[INPUT_POS_2].at(0), (userInput[INPUT_POS_2].at(1)) - ASCII_CONVERTER_DIGIT);
            //If the LinkedList returns a index for the found tile
            if (tileIndex > -1)
            {
                Tile *tile = currentPlayer->getHand()->get(tileIndex);
                replaceTile(tile);
                returnVal = true;
            }
            else
            {
                cout << "That tile is not in your hand. Try again: " << endl;
            }
            //the case where user does replace command at the start of the game, still allow placement of first tile
            if (turnNum == 1)
            {
                turnNum--;
            }
        }
        else if (userInput[INPUT_POS_1] == "SAVE" && userInput.size() > 1)
        { //user is saving game
            std::string outputFileName;

            outputFileName = userInput[INPUT_POS_2];

            GameSaver *gs = new GameSaver(player1, player2, board, bag, currentPlayer, outputFileName);
            delete gs;
            //Don't switch player turn when saving
            returnVal = false;
            cout << endl
                 << "Game successfully saved" << endl
                 << endl;
        }
        else if (userInput[INPUT_POS_1] == "QUIT")
        { //user is quitting game
            terminateGame = true;
        }
        else
        {
            cout << "Command not recognised. Please try again: " << endl
                 << endl;
            returnVal = false;
        }
    }
    else
    {
        cout << "Command not recognised. Please try again: " << endl
             << endl;
        returnVal = false;
    }
    return returnVal;
}

bool OldGame::playTile(Tile *tile, int row, int col)
{
    bool returnVal = true;

    if (isValidMove(tile, row, col) && (hasAdjoiningTile(row, col) || turnNum == 1))
    { //and move is legal

        board->placeTile(tile, row, col);
        updatePoints(row, col);
        //update the score
        if (!bag->isEmpty())

        {
            drawCard();
        }
        currentPlayer->getHand()->removeElement(tile);
    }
    else
    {
        cout << "Invalid move. Try again: " << endl;

        returnVal = false;
    }

    return returnVal;
}

bool OldGame::isValidMove(Tile *userTile, int row, int col)
{
    bool returnVal = true;
    bool diffShape = false;
    bool neighbour1check = true;
    bool neighbour2check = true;
    bool neighbour3check = true;
    bool neighbour4check = true;
    bool isRow = true;
    int newRow = 0;
    int newCol = 0;
    //First check if the row and col are within the board
    if (!(row >= ROW_MIN && col >= COLUMN_MIN && row <= ROW_MAX && col <= COLUMN_MAX))
    {
        returnVal = false;
    }
    else if (board->getTileAt(row, col) != nullptr)
    {
        returnVal = false;
    }
    else if (neighboursContains(userTile, row, col))
    {
        returnVal = false;
    }
    else
    {
        for (int j = 0; j < 2; j++)
        {
            for (int i = -1; i < 2; i++)
            {
                if (j == 0)
                { //first check column
                    newRow = row + i;
                    newCol = col;
                    isRow = false;
                }
                else
                { //then check row
                    newRow = row;
                    newCol = col + i;
                    isRow = true;
                }
                Tile *currentTile = board->getTileAt(newRow, newCol);
                if (currentTile != nullptr)
                {
                    diffShape = false;
                    //if neither colour nor shape are the same, move is not valid
                    if (currentTile->shape != userTile->shape && currentTile->colour != userTile->colour)
                    {
                        returnVal = false;
                    }
                    else if (currentTile->colour == userTile->colour)
                    {
                        //if the colour is the same, all shapes in this line should be different
                        diffShape = true;
                        returnVal = checkNeighbours(newRow, newCol, diffShape, currentTile, isRow);
                    }
                    else
                    {
                        //otherwise, the colour should be different ie. diffShape=false as it was initialised
                        returnVal = checkNeighbours(newRow, newCol, diffShape, currentTile, isRow);
                    }
                    if (j == 0 && i == -1)
                    {
                        neighbour1check = returnVal;
                    }
                    else if (j == 0 && i == 1)
                    {
                        neighbour2check = returnVal;
                    }
                    else if (j == 1 && i == -1)
                    {
                        neighbour3check = returnVal;
                    }
                    else if (j == 1 && i == 1)
                    {
                        neighbour4check = returnVal;
                    }
                }
            }
        }
        returnVal = neighbour1check && neighbour2check && neighbour3check && neighbour4check;
    }
    return returnVal;
}

bool OldGame::hasAdjoiningTile(int row, int col)
{
    bool hasAdjoining = false;

    if (row < ROW_MAX)
    {
        if (board->hasTileAt(row + 1, col))
        {
            hasAdjoining = true;
        }
    }
    if (row > ROW_MIN)
    {
        if (board->hasTileAt(row - 1, col))
        {
            hasAdjoining = true;
        }
    }

    if (col < COLUMN_MAX)
    {
        if (board->hasTileAt(row, col + 1))
        {
            hasAdjoining = true;
        }
    }
    if (col > COLUMN_MIN)
    {
        if (board->hasTileAt(row, col - 1))
        {
            hasAdjoining = true;
        }
    }

    return hasAdjoining;
}

void OldGame::replaceTile(Tile *tile)
{
    currentPlayer->getHand()->removeElement(tile);
    bag->addBack(tile);
    drawCard();
}

void OldGame::drawCard()
{
    Tile *newTile = bag->get(0);
    bag->removeFront();
    currentPlayer->getHand()->addBack(newTile);
}

void OldGame::switchPlayer()
{
    if (currentPlayer->equals(player1))
    {
        currentPlayer = player2;
    }
    else
    {
        currentPlayer = player1;
    }
}

bool OldGame::checkNeighbours(int row, int col, bool diffShape, Tile *originalTile, bool isRow)
{
    bool returnVal = true;
    Tile *neighbours[2] = {};
    if (isRow)
    {
        neighbours[0] = board->getTileAt(row, col + 1);
        neighbours[1] = board->getTileAt(row, col - 1);
    }
    else
    {
        neighbours[0] = board->getTileAt(row + 1, col);
        neighbours[1] = board->getTileAt(row - 1, col);
    }
    for (Tile *neighbour : neighbours)
    {
        if (neighbour != nullptr)
        {

            //cout << "neighbour found" << endl;
            if (neighbour->equals(originalTile->colour, originalTile->shape))
            {
                returnVal = true;
            }

            //if shape is meant to be different, check it is different
            if (diffShape && (neighbour->shape == originalTile->shape))
            {
                returnVal = false;
            }
            //if colour is meant to be different, check it is different
            else if (!diffShape && (neighbour->colour == originalTile->colour))
            {
                returnVal = false;
            }
        }
    }
    return returnVal;
}

void OldGame::updatePoints(int row, int col)
{
    int pointsToAdd = countNeighbours(row, col);
    currentPlayer->addPoints(pointsToAdd);
}

int OldGame::countNeighbours(int row, int col)
{
    bool upEmpty = true;
    bool downEmpty = true;
    bool leftEmpty = true;
    bool rightEmpty = true;
    int downVal = 0;
    int upVal = 0;
    int rightVal = 0;
    int leftVal = 0;

    Direction d;
    int count = 0;

    if (row < ROW_MAX)
    {
        if (board->hasTileAt(row + 1, col))
        {
            d = Down;
            downVal = countLine(row + 1, col, d) + 1;
            downEmpty = false;
        }
    }
    if (row > ROW_MIN)
    {
        if (board->hasTileAt(row - 1, col))
        {
            d = Up;
            upVal = countLine(row - 1, col, d) + 1;
            upEmpty = false;
        }
    }

    if (col < COLUMN_MAX)
    {
        if (board->hasTileAt(row, col + 1))
        {
            d = Right;
            rightVal = countLine(row, col + 1, d) + 1;
            rightEmpty = false;
        }
    }
    if (col > COLUMN_MIN)
    {
        if (board->hasTileAt(row, col - 1))
        {
            d = Left;
            leftVal = countLine(row, col - 1, d) + 1;
            leftEmpty = false;
        }
    }

    count = downVal + upVal + rightVal + leftVal;

    if (upEmpty && downEmpty && leftEmpty && rightEmpty)
    {
        count = 1;
    }

    // Check for any qwirkles created by adding a tile to the end of the line of tiles
    if (downVal == 6 || upVal == 6 || rightVal == 6 || leftVal == 6)
    {
        cout << endl
             << "QWIRKLE!!!" << endl
             << endl;
    }

    //eliminate the double count of the placed tile if the tile creates a line which extends both ways
    if (!upEmpty && !downEmpty)
    {
        count = count - 1;

        //checks for qwirkles when tile is placed between two lines connecting them vertically
        if (downVal + upVal - 1 == 6)
        {
            cout << "QWIRKLE!!!" << endl;
        }
    }

    if (!leftEmpty && !rightEmpty)
    {
        //checks for qwirkles when tile is placed between two lines connecting them horizontally
        if (leftVal + rightVal - 1 == 6)
        {
            cout << "QWIRKLE!!!" << endl;
        }
        count = count - 1;
    }

    return count;
}

int OldGame::countLine(int row, int col, OldGame::Direction direction)
{
    int retVal = 0;

    int y = 0;
    int x = 0;

    if (direction == Up)
    {
        y = -1;
    }
    else if (direction == Down)
    {
        y = 1;
    }
    else if (direction == Left)
    {
        x = -1;
    }
    else if (direction == Right)
    {
        x = 1;
    }

    if (row + y >= ROW_MIN && row + y <= ROW_MAX && col + x >= COLUMN_MIN && col + x <= COLUMN_MAX)
    {
        if (!board->hasTileAt(row + y, col + x))
        {
            retVal = 1;
        }
        else
        {
            retVal = countLine(row + y, col + x, direction) + 1;
        }
    }
    else
    {
        retVal = 1;
    }

    return retVal;
}

bool OldGame::neighboursContains(Tile *tile, int row, int col)
{
    Direction d;
    bool tileFound = false;
    if (row < ROW_MAX)
    {
        if (board->hasTileAt(row + 1, col))
        {
            d = Down;
            tileFound = checkLine(row + 1, col, d, tile);
        }
    }
    if (row > ROW_MIN)
    {
        if (board->hasTileAt(row - 1, col) && !tileFound)
        {
            d = Up;
            tileFound = checkLine(row - 1, col, d, tile);
        }
    }
    if (col < COLUMN_MAX)
    {
        if (board->hasTileAt(row, col + 1) && !tileFound)
        {
            d = Right;
            tileFound = checkLine(row, col + 1, d, tile);
        }
    }
    if (col > COLUMN_MIN)
    {
        if (board->hasTileAt(row, col - 1) && !tileFound)
        {
            d = Left;
            tileFound = checkLine(row, col - 1, d, tile);
        }
    }

    return tileFound;
}

bool OldGame::checkLine(int row, int col, OldGame::Direction direction, Tile *searchTile)
{
    bool retVal = false;

    int y = 0;
    int x = 0;

    if (direction == Up)
    {
        y = -1;
    }
    else if (direction == Down)
    {
        y = 1;
    }
    else if (direction == Left)
    {
        x = -1;
    }
    else if (direction == Right)
    {
        x = 1;
    }

    if (board->getTileAt(row, col)->equals(searchTile->colour, searchTile->shape))
    {
        retVal = true;
    }
    else if (row + y >= ROW_MIN && row + y <= ROW_MAX && col + x >= COLUMN_MIN && col + x <= COLUMN_MAX)
    {
        if (!board->hasTileAt(row + y, col + x))
        {
            retVal = false;
        }
        else
        {
            retVal = checkLine(row + y, col + x, direction, searchTile);
        }
    }
    return retVal;
}

void OldGame::printGameStatus()
{
    cout << endl
         << currentPlayer->getName() << ", it's your turn" << endl;
    cout << "Score for " << player1->getName() << ": " << player1->getPoints() << endl;
    cout << "Score for " << player2->getName() << ": " << player2->getPoints() << endl;
    board->printBoard();
    cout << "Your hand is" << endl;
    cout << currentPlayer->getHand()->toString() << endl
         << endl;
}

std::vector<std::string> OldGame::processCommand(std::string inputString)
{
    //Make string uppercase to reduce invalid inputs
    std::transform(inputString.begin(), inputString.end(), inputString.begin(), ::toupper);
    std::vector<string> commandSplit;
    std::istringstream iss(inputString);
    for (string inputString; iss >> inputString;)
    {
        commandSplit.push_back(inputString);
    }
    return commandSplit;
}