#pragma once
#include <fstream>

#include "../GamePlayer/GamePlayer.h"
#include "../GameBoard/GameBoard.h"

class GameManager{
private:
    GameBoard board;
    Player p1;
    Player p2;

    ofstream output_file;

public:
    GameManager(GameBoard board, Player p1, Player p2, string output_file_name);
    void play();
    //     // should call p1, p2 .getActions() and perform actions on the board until someone wins or there is a tie
    //     // output actions to the output file in some format    
};