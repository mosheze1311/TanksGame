#include "GameBoard/GameBoard.h"
#include "GameManager/GameManager.h"
#include "GamePlayer/GamePlayer.h"

int main(int argc, char *argv[])
{
    if (argc != 2){
        //todo: add error looging.
        return 0;
    }
    const string file_path = argv[1];
    GameBoard b(file_path);
    Player p1;
    Player p2;

    GameManager gm(b, p1, p2, "");
    gm.play();
}