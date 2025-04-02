enum class game_object
{
    tank1 = '1',
    tank2 = '2',
    wall = '#',
    mine = '@'
};

enum class Direction
{
    UP,
    UPR,
    RIGHT,
    DOWNR,
    DOWN,
    DOWNL,
    LEFT,
    UPL
};

class Mine
{
private:
public:
    Mine();
};
class Wall
{
public:
    Wall();
};
class Tank
{
public:
    Tank();
};
