#pragma once
#include "../GameBoard/GameBoard.h"

// a shallow copy of a board that does not own the objects.
class GameBoardShallowCopy
{
private:
    int width;
    int height;
    std::unordered_map<GameObject *, BoardCell> objects_locations;
    std::map<BoardCell, std::unordered_set<GameObject *>> board;

public:
    GameBoardShallowCopy(const GameBoard &b)
    {
        this->width = b.getWidth();
        this->height = b.getHeight();
        for (GameObject *obj : b.getAllGameObjects())
        {
            BoardCell c = b.getObjectLocation(obj).value();
            this->addObject(obj, c);
        }
    };

    // get a set of all objects on the cell
    std::unordered_set<GameObject *> getObjectsOnCell(const BoardCell &c) const
    {
        auto it = board.find(c);
        if (it != board.end())
        {
            return it->second;
        }
        return {};
    };

    // get all objects on the board
    std::vector<GameObject *> getAllGameObjects() const
    {
        std::vector<GameObject *> res;
        for (const std::pair<GameObject *, BoardCell> iter : this->objects_locations)
        {
            res.push_back(iter.first);
        }
        return res;
    };

    // get an optional cell location of an object on board. if object not on board, return nullopt.
    std::optional<BoardCell> getObjectLocation(const GameObject *go) const
    {
        auto iter = this->objects_locations.find(const_cast<GameObject *>(go));
        if (iter == this->objects_locations.end())
        {
            return std::nullopt;
        }
        return iter->second;
    };

    // Check if the specific object exists on board
    bool isObjectOnBoard(GameObject *obj) const
    {
        return this->objects_locations.find(obj) != this->objects_locations.end();
    };

    // Add an object to the board on the requested cell
    void addObject(GameObject *obj, const BoardCell &c)
    {
        this->objects_locations[obj] = c;
        this->board[c].insert(obj);
    };
};
