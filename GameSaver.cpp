#include "GameSaver.h"
#include "Player.h"
#include "Board.h"
#include "LinkedList.h"
#include <iostream>
using std::endl;

GameSaver::GameSaver(Player *player1, Player *player2, Board *board, LinkedList *bag, Player *currentPlayer, std::string outputFileName)
{
    outFile.open("Tests/Test-3-TestSaving/" + outputFileName + ".txt");

    outputPlayer(player1);
    outputPlayer(player2);

    outFile << "26,26" << endl;

    outFile << board->getState() << endl;
    outFile << bag->toString() << endl;
    outFile << currentPlayer->getName();
}

void GameSaver::outputPlayer(Player *player)
{
    outFile << player->getName() << endl;
    outFile << player->getPoints() << endl;
    outFile << player->getHand()->toString() << endl;
}

GameSaver::GameSaver(std::vector<Player*> players, Board *board, LinkedList *bag, Player *currentPlayer, std::string outputFileName)
{
    outFile.open("Tests/Test-3-TestSaving/" + outputFileName + ".txt");
    int size= players.size();
    if (size == 3)
    {
        outputPlayer(players.at(0));
        outputPlayer(players.at(1));
        outputPlayer(players.at(2));
    }
    else
    {
        outputPlayer(players.at(0));
        outputPlayer(players.at(1));
        outputPlayer(players.at(2));
        outputPlayer(players.at(3));
    }

    outFile << "26,26" << endl;

    outFile << board->getState() << endl;
    outFile << bag->toString() << endl;
    outFile << currentPlayer->getName();
}