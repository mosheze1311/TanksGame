#pragma once

#include "../Tactics/Tactic.h"

#include <functional>
#include <map>
#include <vector>
#include <set>

namespace Algorithm_211388913_322330820
{

    struct TankDetails
    {
        size_t num_shells;
        BoardCell location;
        Direction dir;
        std::unique_ptr<Tactic> tactic;

        // === Constructor === //
        TankDetails() : num_shells(0), tactic(nullptr){}
        TankDetails(size_t num_shells, BoardCell location, Direction dir, std::unique_ptr<Tactic> tactic)
            : num_shells(num_shells), location(location), dir(dir), tactic(std::move(tactic)) {}

        bool operator<(const TankDetails & other) const
        {
            return std::tie(num_shells, location, dir) < std::tie(other.num_shells, other.location, other.dir);
        }

        bool operator==(const TankDetails & other) const
        {
            return std::tie(num_shells, location, dir) == std::tie(other.num_shells, other.location, other.dir);
        }

        // === Copy Constructor & Operator === //
        TankDetails(const TankDetails &other)
            : num_shells(other.num_shells),
              location(other.location),
              dir(other.dir),
              tactic(other.tactic ? other.tactic->clone() : nullptr) {}

        TankDetails &operator=(const TankDetails &other)
        {
            if (this == &other)
                return *this;

            num_shells = other.num_shells;
            location = other.location;
            dir = other.dir;
            tactic = other.tactic ? other.tactic->clone() : nullptr;
            return *this;
        }

        // === Move Constructor & Operator === //
        TankDetails(TankDetails &&other) noexcept = default;

        TankDetails &operator=(TankDetails &&other) noexcept = default;
    };

    class Formation
    {

    private:
        // === Attributes === //
        std::set<BoardCell> formation_tanks;

    public:
        // === Constructor === //
        Formation();

        // === Operators === //
        bool operator<(const Formation &other) const;
        void insert(const BoardCell& tank_location);

        // === Getters === //
        std::set<BoardCell> getTankFormation() const;
        BoardCell getFormationCenter() const;
        size_t size() const;
        bool contains(const BoardCell &location) const;

        // === Logic Functions === //
        static std::set<Formation> createFormationGroups(const std::set<BoardCell>& all_tanks_locations, std::function<int(const BoardCell &, const BoardCell &)> distance);

        // === Helpers === //

    };
}
