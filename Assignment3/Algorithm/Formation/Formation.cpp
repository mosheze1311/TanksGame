#include "Formation.h"

namespace Algorithm_211388913_322330820
{
    // === Constructor === //
    Formation::Formation() {}

    // === Getters === //
    size_t Formation::size() const
    {
        return formation_tanks.size();
    }
    bool Formation::contains(const BoardCell &location) const {
        return this->formation_tanks.contains(location);
    }

    std::set<BoardCell> Formation::getTankFormation() const
    {
        return formation_tanks;
    }

    BoardCell Formation::getFormationCenter() const
    {
        float x = 0, y = 0;
        for (const BoardCell &cell : getTankFormation())
        {
            x += (cell.getX() / this->size());
            y += (cell.getY() / this->size());
        }

        return BoardCell(x,y);
    }

    // === Opertors === //
    
    bool Formation::operator<(const Formation& other) const{
        return this->formation_tanks < other.formation_tanks;
    }

    void Formation::insert(const BoardCell& tank_location)
    {
        formation_tanks.insert(tank_location);
    }


    // === Static Creation Method === //
    std::set<Formation> Formation::createFormationGroups(const std::set<BoardCell> &all_tanks_locations, std::function<int(const BoardCell &, const BoardCell &)> distance)
    {
        std::set<Formation> formations;
        std::set<BoardCell> copy_locations = all_tanks_locations;
        while (!copy_locations.empty())
        {
            BoardCell formation_leader = *copy_locations.begin();
            copy_locations.erase(formation_leader);

            Formation formation;
            formation.insert(formation_leader);

            // calculate distances to all tanks and sort decending
            std::vector<std::pair<BoardCell, int>> distances;
            for (const auto &cell : copy_locations)
            {
                distances.emplace_back(cell, distance(formation_leader, cell));
            }
            std::sort(distances.begin(), distances.end(),
                      [](const auto &a, const auto &b)
                      { return a.second < b.second; });

            // add closest tanks to formation
            for (size_t i = 0; i < distances.size() && formation.size() < 3; ++i)
            {
                BoardCell closest_tank_location = distances[i].first;
                formation.insert(closest_tank_location);
                copy_locations.erase(closest_tank_location);
            }

            formations.insert(formation);
        }
        return formations;
    }
}