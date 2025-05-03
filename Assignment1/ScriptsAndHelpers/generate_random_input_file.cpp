#include <iostream>
#include <random>
#include <vector>
#include <map>

using namespace std;
#define CELL pair<int, int>
enum class GameObjectType
{
    TANK1 = '1',
    TANK2 = '2',
    WALL = '#',
    MINE = '@'
};
CELL random_cell(int h, int w)
{
    int lower = 0;
    std::random_device rd;
    std::mt19937 gen_h(rd());
    std::mt19937 gen_w(rd());
    std::uniform_int_distribution<int> dist_h(lower, h-1);
    std::uniform_int_distribution<int> dist_w(lower, w-1);
    int random_row = dist_h(gen_h);
    int random_col = dist_w(gen_w);
    return {random_col, random_row};
}

bool is_occupied_cell(map<int, map<int, GameObjectType>> board, CELL c)
{
    if (board.find(c.first) == board.end())
    {
        return false;
    }
    if (board[c.first].find(c.second) == board[c.first].end())
    {
        return false;
    }
    return true;
}

struct game_general_params
{
    int h, w, tanks, walls, mines;
};


class Game
{
private:
    game_general_params gp;
    map<GameObjectType, vector<CELL>> objects_locations;
    map<int, map<int, GameObjectType>> board;

    void add_game_objects(int count, GameObjectType go)
    {
        objects_locations[go] = vector<CELL>();
        for (int i = 0; i < count; i++)
        {
            CELL c;
            do
            {
                c = random_cell(this->gp.h, this->gp.w);
            } while (is_occupied_cell(this->board, c));
            board[c.first][c.second] = go;
            objects_locations[go].push_back(c);
        }
    }

public:
    Game(game_general_params &gp)
    {
        this->gp = gp;
        add_game_objects(gp.mines, GameObjectType::MINE);
        add_game_objects(gp.tanks, GameObjectType::TANK1);
        add_game_objects(gp.tanks, GameObjectType::TANK2);
        add_game_objects(gp.walls, GameObjectType::WALL);
    }


    void create_input_file()
    {
        cout << gp.h << " " << gp.w << " " << gp.tanks << " " << gp.tanks << " " << gp.walls << " " << gp.mines << endl;
        for (GameObjectType go : {GameObjectType::TANK1, GameObjectType::TANK2, GameObjectType::WALL, GameObjectType::MINE}){
            for (int i = 0; i < objects_locations[go].size(); i++)
            {
                cout << (char)go << " "
                     << objects_locations[go][i].first << " "
                     << objects_locations[go][i].second << endl;
            }
        }
    }

    void draw_game(){
        map<GameObjectType, string> draw_map = {
            {GameObjectType::MINE, "💥"},
            {GameObjectType::WALL, "⬛"},
            {GameObjectType::TANK1, "🚙"},
            {GameObjectType::TANK2, "🚜"}};
        vector<string> empty_blocks({"⬜", "⬜"});
        string game_border_portal = "🌀";
        for (int i = 0 ; i < this->gp.w + 2; i++){
            cout << game_border_portal;
        }
        cout <<endl;
        for (int i = 0; i < this->gp.h; i++)
        {
            cout << game_border_portal;
            for (int j = 0; j < this->gp.w; j++)
            {
                if (is_occupied_cell(this->board,{j,i})){
                    cout << draw_map[board[j][i]];
                }
                else{
                    cout << empty_blocks[(i + j) % 2];
                }
            }
            cout << game_border_portal << endl;
        }
        for (int i = 0; i < this->gp.w + 2; i++)
        {
            cout << game_border_portal;
        }
        cout << endl;

    }
};


void request_game_params(game_general_params &g)
{
    cout << "height: ";
    cin >> g.h;
    cout << "width: ";
    cin >> g.w;
    cout << "tanks per player: ";
    cin >> g.tanks;
    cout << "walls: ";
    cin >> g.walls;
    cout << "mines: ";
    cin >> g.mines;
}


int main()
{
    game_general_params gp;
    request_game_params(gp);
    Game g(gp);
    g.create_input_file();
    cout << endl << endl;
    g.draw_game();

    return 0;
}