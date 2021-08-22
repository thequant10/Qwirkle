#ifndef ASSIGN2_GAME_H
#define ASSIGN2_GAME_H

#include <string>
#include <vector>

#define INPUT_SIZE_MAX 5
#define ASCII_CONVERTER_DIGIT 48
#define ASCII_CONVERTER_LETTER 65
#define INPUT_POS_1 0
#define INPUT_POS_2 1
#define INPUT_POS_3 2
#define INPUT_POS_4 3

class LinkedList;
class Tile;
class Board;
class Player;

class Game
{
public:
    //normal game
    Game(Player *player1, Player *player2, LinkedList *bag, Board *board, Player *currentPlayer);
    //multiple player game
    Game(std::vector<Player*> players, LinkedList *bag, Board *board, Player *currentPlayer);

    ~Game();
    void executeGameplay();
    void executeMultipleGameplay();
    enum Direction
    {
        Up,
        Down,
        Left,
        Right
    };

private:
    bool playTurn(std::vector<std::string> userInput);
    void switchPlayer();
    bool playTile(Tile *tile, int row, int col);
    void replaceTile(Tile *tile);
    void drawCard();
    bool isValidMove(Tile *userTile, int row, int col);
    bool hasAdjoiningTile(int row, int col);
    bool checkNeighbours(int row, int col, bool diffShape, Tile *originalTile, bool isRow);
    void updatePoints(int row, int col);
    int countNeighbours(int row, int col);
    int countLine(int row, int col, Game::Direction direction);
    bool neighboursContains(Tile *tile, int row, int col);
    bool checkLine(int row, int col, Game::Direction direction, Tile *searchTile);
    bool playRound();
    void printGameStatus();
    //void multipleTile(std::vector<string> commandSplit);
    //place multiple tiles
    bool placeMultipleTIles(std::vector<std::string> commandSplit, Player* player);
    //gives previous hand
    LinkedList* prevHand(LinkedList* list, Tile* tile);
    //checks input for multiple tiles
    std::vector<bool> checkInput(std::vector<std::string> commandSplit, Player* player);
    //vector of players for multiple players
    std::vector<Player*> players;
    //swithces mulitple players
    void switchMultiplePlayers();
    //places mulitple players
    void printMultipleGameStatus();
    //check if tile location is valid
    bool checkTile(Tile *tile, int row, int col, int i);
    //gives winner for multiple players
    void giveWinner(std::vector<Player*> players);

    std::vector<std::string> processCommand(std::string inputString);

    Player *player1;
    Player *player2;
    Player *player3;
    Player *player4;
    LinkedList *bag;
    Board *board;
    Player *currentPlayer;
    bool terminateGame;
    bool gameOver;
    int turnNum = 0;
};

#endif // ASSIGN2_GAME_H