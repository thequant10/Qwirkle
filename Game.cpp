#include "Game.h"
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

Game::Game(Player *player1, Player *player2, LinkedList *bag, Board *board, Player *currentPlayer)
{
    this->player1 = player1;
    this->player2 = player2;
    this->bag = bag;
    this->board = board;
    this->currentPlayer = currentPlayer;
    this->terminateGame = false;
    this->gameOver = false;
}

Game::Game(std::vector<Player*> players, LinkedList *bag, Board *board, Player *currentPlayer)
{
    this->bag = bag;
    this->board = board;
    this->currentPlayer = currentPlayer;
    this->terminateGame = false;
    this->gameOver = false;
    this->players= players;
    int val=players.size();
    std::cout<<val<<endl;
    if(val==3){
        player1=players.at(0);
        player2=players.at(1);
        player3=players.at(2);
    }else{
        player1=players.at(0);
        player2=players.at(1);
        player3=players.at(2);
        player4=players.at(3);
    }
}

Game::~Game()
{
    delete player1;
    delete player2;
    delete bag;
    delete board;
}



//fixing when illegal moves are made such at placing two tiles at once but now valid
//colours
//testcases for implementations
void Game::executeGameplay()
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
                if (command.size() > 15)
                {
                    std::cout<<command.size()<<endl;
                    vector<bool> checkVec = checkInput(commandSplit, currentPlayer);
                    //correctCommand = playTurn(commandSplit);
                    //checkvec at 1 is false
                    if (checkVec.at(0) == true && checkVec.at(1) == true)
                    {
                        correctCommand = placeMultipleTIles(commandSplit, currentPlayer);
                    }
                    else if (command.length() < 15)
                    {
                        std::cout << "Invalid command" << endl;
                    }
                }else{
                    correctCommand=playTurn(commandSplit);
                }
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

void Game::executeMultipleGameplay()
{
    cout << endl
         << "Let's Play!" << endl
         << endl;
    while (!terminateGame && !gameOver)
    {
        turnNum++;
        string command = "";
        bool correctCommand = false;
        printMultipleGameStatus();
        do
        {
            cout << "> ";
            if (!cin.eof())
            {
                getline(cin, command);
                std::vector<string> commandSplit = processCommand(command);
                if (command.size() > 15)
                {

                    vector<bool> checkVec = checkInput(commandSplit, currentPlayer);
                    //correctCommand = playTurn(commandSplit);
                    //checkvec at 1 is false
                    if (checkVec.at(0) == true && checkVec.at(1) == true)
                    {
                        correctCommand = placeMultipleTIles(commandSplit, currentPlayer);
                    }
                    else if (command.length() < 15)
                    {
                        std::cout << "Invalid command" << endl;
                    }
                }
                else
                {
                    correctCommand = playTurn(commandSplit);
                }
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
            switchMultiplePlayers();
            if(players.size()==3){
                if (bag->isEmpty() && (player1->getHand()->isEmpty() || player2->getHand()->isEmpty()||player3->getHand()->isEmpty()))
                {
                    gameOver = true;
                }
            }else{
                if (bag->isEmpty() && (player1->getHand()->isEmpty() || player2->getHand()->isEmpty()||player3->getHand()->isEmpty()||player4->getHand()->isEmpty()))
                {
                    gameOver = true;
                }
            }
        }
    }
    if (gameOver == true)
    {
        cin.ignore(std::numeric_limits<char>::max(), '\n');
        cout << endl
             << "Game over" << endl;
        giveWinner(players);
    }
    cout << endl
         << "Goodbye" << endl;
}

void Game::giveWinner(vector<Player*> players){
    Player* winner=nullptr;
    int size= players.size();
    if(players.size()==3||players.size()==4){
        for(int k=0;k<size;++k){
            cout << "Score for " << players.at(k)->getName() << ": " << players.at(k)->getPoints() << endl;
        }
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                if(players.at(i)!=players.at(j)){
                    if(players.at(i)->getPoints()>players.at(j)->getPoints()){
                        winner=players.at(i);
                    }else{
                        winner=players.at(j);
                    }                    
                }
            }
        }        
    }
    if(winner==nullptr){
        cout << "The game was a draw!" << endl;
    }else{
        cout << "Player " << winner->getName() << " won!" << endl;
    }
    delete winner;
}


bool Game::playTurn(vector<string> userInput)
{
    //LinkedList* globalHand=currentPlayer->getHand();
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
                        //globalHand=playTile2(tile, locationRow, value + 1,currentPlayer->getHand());  
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
                cout << "The tile "<<userInput[INPUT_POS_2]+" is not in your hand"<<". Try again: " << endl<<endl;
                //board->expandBoard();
                //std::cout<<globalHand->toString()<<std::endl;
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
                cout << "THE TILE "<<userInput[INPUT_POS_2]+" is not in your hand "<<". Try again: " << endl<<endl;          
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
            cout << userInput[INPUT_POS_2] << endl;

            outputFileName = userInput[INPUT_POS_2];
            if(player3==nullptr&&player4==nullptr){
                GameSaver *gs = new GameSaver(player1, player2, board, bag, currentPlayer, outputFileName);
                delete gs;
            }else{
                GameSaver *gs = new GameSaver(players, board, bag, currentPlayer, outputFileName);
                delete gs;
            }
            //Don't switch player turn when saving
            returnVal = false;
            cout << endl
                 << "Game successfully saved" <<     endl
                 << endl;
        }
        else if (userInput[INPUT_POS_1] == "QUIT")
        { //user is quitting game
            terminateGame = true;
        }else if(userInput[INPUT_POS_1]=="HELP!"){
            std::cout<<"1.To place a tile type: Place TILE AT LOCATION"<<std::endl;
            std::cout<<"2.To replace a tile type: REPLACE TILE"<<endl;
            std::cout<<"3.To Save File: type SAVE [FILENAME]"<<endl;
            std::cout<<"4.To place multiple tiles: type Place TILE AT LOCATION Place TILE AT LOCATION PLACE..."<<endl;
            std::cout<<"4.To quit the game: type QUIT"<<endl;
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

bool Game::playTile(Tile *tile, int row, int col)
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

bool Game::checkTile(Tile *tile, int row, int col, int i)
{
    bool returnVal = true;

    if (((isValidMove(tile, row, col) && (hasAdjoiningTile(row, col)))))
    { //and move is legal

        //board->placeTile(tile, row, col);
        //updatePoints(row, col);
        //update the score
        returnVal=true;
    }else if(i==0){
        returnVal=true;
    }
    else
    {
        returnVal = false;
    }

    return returnVal;
}


LinkedList* Game::prevHand(LinkedList* list, Tile* tile){
    LinkedList* saveList= list;
    saveList->removeElement(tile);
    return saveList;
}

bool Game::isValidMove(Tile *userTile, int row, int col)
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

bool Game::hasAdjoiningTile(int row, int col)
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

void Game::replaceTile(Tile *tile)
{
    currentPlayer->getHand()->removeElement(tile);
    bag->addBack(tile);
    drawCard();
}

void Game::drawCard()
{
    Tile *newTile = bag->get(0);
    bag->removeFront();
    currentPlayer->getHand()->addBack(newTile);
}

void Game::switchPlayer()
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

void Game::switchMultiplePlayers(){
    if (currentPlayer->equals(player1))
    {
        currentPlayer = player2;
    }
    else if(currentPlayer->equals(player2))
    {
        currentPlayer = player3;
    }else if(currentPlayer->equals(player3))
    {
        if(players.size()==4){
            currentPlayer = player4;
        }else{
            currentPlayer = player1;
        }      
    }else if(currentPlayer->equals(player4))
    {
        currentPlayer = player1;
    }

}

//the solution to not placing stupied tiles
bool Game::checkNeighbours(int row, int col, bool diffShape, Tile *originalTile, bool isRow)
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

void Game::updatePoints(int row, int col)
{
    int pointsToAdd = countNeighbours(row, col);
    currentPlayer->addPoints(pointsToAdd);
}

int Game::countNeighbours(int row, int col)
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

int Game::countLine(int row, int col, Game::Direction direction)
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

bool Game::neighboursContains(Tile *tile, int row, int col)
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

bool Game::checkLine(int row, int col, Game::Direction direction, Tile *searchTile)
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

void Game::printGameStatus()
{
    cout << endl
         << currentPlayer->getName() << ", it's your turn" << endl;
    cout << "Score for " << player1->getName() << ": " << player1->getPoints() << endl;
    cout << "Score for " << player2->getName() << ": " << player2->getPoints() << endl;
    //board->printBoard();
    board->expandBoard();
    cout << "Your hand is" << endl;
    cout << currentPlayer->getHand()->toString() << endl
         << endl;
}

void Game::printMultipleGameStatus()
{
    cout << endl
         << currentPlayer->getName() << ", it's your turn" << endl;

    if(players.size()==3){
        cout << "Score for " << player1->getName() << ": " << player1->getPoints() << endl;
        cout << "Score for " << player2->getName() << ": " << player2->getPoints() << endl;
        cout << "Score for " << player3->getName() << ": " << player3->getPoints() << endl;
    }else{
        cout << "Score for " << player1->getName() << ": " << player1->getPoints() << endl;
        cout << "Score for " << player2->getName() << ": " << player2->getPoints() << endl;
        cout << "Score for " << player3->getName() << ": " << player3->getPoints() << endl;
        cout << "Score for " << player4->getName() << ": " << player4->getPoints() << endl;       
    }

    //board->printBoard();
    board->expandBoard();
    cout << "Your hand is" << endl;
    cout << currentPlayer->getHand()->toString() << endl
         << endl;
}



std::vector<std::string> Game::processCommand(std::string inputString)
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


bool Game::placeMultipleTIles(std::vector<string> commandSplit, Player* player)
{
    std::vector<string> multipleTile;
    bool correctCommand = false;
    bool commandC= true;
    bool check=true;
    int inputVal = commandSplit.size() / 4;
    if (commandSplit.size() > 5)
    {
        int k = 0;
        for (int i = 0; i < inputVal; ++i)
        {
            string newCommand = "";
            for (int j = 0; j < 4; ++j)
            {
                if (commandSplit[k] == "PLACE")
                {
                    newCommand = newCommand + commandSplit[k];
                    ++k;
                }
                else
                {
                    newCommand = newCommand + " " + commandSplit[k];
                    ++k;
                }
            }
            multipleTile = processCommand(newCommand);
            correctCommand = playTurn(multipleTile);
            if(correctCommand==false){
                commandC=false;
            }   
        //     if(player->equals(player1)&&player->getPoints()==1&&i==0){
        //     player1->addPoints(-1);
        // }
            // if(i!=1 && correctCommand==false){
            //     printGameStatus();              
            // }   
        }
    }else if(commandSplit.size() < 5){
        correctCommand = playTurn(commandSplit);
        commandC=correctCommand;
        check=false;
    }
    if(commandC==false&&check){
        board->expandBoard();
    }
    return commandC;
}


vector<bool> Game::checkInput(std::vector<string> commandSplit, Player* player)
{
    //return value
    vector<bool> retVec;
    std::vector<string> multipleTile;
    bool retval= false;
    int inputVal = commandSplit.size() / 4;
    bool absVal[inputVal]={0};
    int k = 0;
    bool absval2[inputVal]={0};
    int rowList[inputVal]={0};
    int colList[inputVal]={0};
    //checks if location is valid
    bool checkval;
        for (int i = 0; i < inputVal; ++i)
        {
            retval=false;
            checkval=true;
            string newCommand = "";
            for (int j = 0; j < 4; ++j)
            {
                if (commandSplit[k] == "PLACE")
                {
                    newCommand = newCommand + commandSplit[k];
                    ++k;
                }
                else
                {
                    newCommand = newCommand + " " + commandSplit[k];
                    ++k;
                }
            }
            multipleTile = processCommand(newCommand);
            int tileIndex = player->getHand()->existsAt(multipleTile[INPUT_POS_2].at(0), (multipleTile[INPUT_POS_2].at(1)) - ASCII_CONVERTER_DIGIT);
            Tile *tile = player->getHand()->get(tileIndex);
            int locationRow = (multipleTile[INPUT_POS_4].at(0)) - ASCII_CONVERTER_LETTER;
            rowList[i]=locationRow;
            std::string colVal = multipleTile[INPUT_POS_4];
            int colInt=0;
            //If the LinkedList returns a index for the found tile
            if (tileIndex > -1)
            {
                if (colVal.length() >= 2)
                {
                    char temp[2] = {colVal[1], colVal[2]};
                    std::string tempStr = temp;
                    try
                    {
                        int value = std::stoi(tempStr);
                        colInt=value;
                        colList[i]=value;
                        checkval= checkTile(tile, locationRow, value + 1,i);
                        absval2[i]=checkTile(tile, locationRow, value + 1,i);
                    }
                    catch (std::invalid_argument &e)
                    {
                        cout << "Location code is invalid!" << endl;
                    }
                }

                for(int g=0;g<i;g++){
                    if(locationRow+1==rowList[g]||locationRow-1==rowList[g]){
                        checkval=true;
                        absval2[i]=true;;
                        //std::cout<<multipleTile[INPUT_POS_2]<<endl;
                    }
                    if(colInt==colList[g]||colInt-1==colList[g]){
                        checkval=true;
                        absval2[i]=true;
                        //std::cout<<multipleTile[INPUT_POS_2]<<endl;
                    }
                }

                //if tile location is invalid
                if (checkval == false)
                {
                    cout << "THE TILE " << multipleTile[INPUT_POS_2] + " cannot be placed in " << multipleTile[INPUT_POS_4] << endl;
                }

            for(int m=0;m!=player->getHand()->size();++m){
                if(player->getHand()->get(m)==tile){
                    retval=true;
                    absVal[i]=retval;
                }
                if(tile==nullptr){
                    retval=false;
                }
            }
        }
        //if tile is invalid
        if (retval == false || tileIndex == -1)
        {
            cout << "THE TILE " << multipleTile[INPUT_POS_2] + " is not in your hand "
                 << ". Try again: " << endl
                 << endl;
        }
}
//check if both location and tile are valid
for (int i = 0; i < inputVal; ++i)
{
    if (absVal[i] == false)
    {
        retval = false;
    }
    if (absval2[i] == false)
    {
        checkval = false;
        //std::cout<<"apple"<<std::endl;
    }
}

retVec.push_back(retval);
retVec.push_back(checkval);
//std::cout<<retval<<endl;
//std::cout<<checkval<<endl;
return retVec;
}