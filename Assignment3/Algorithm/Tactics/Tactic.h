#pragma once

#include "../../common/ActionRequest.h"

#include "../../UserCommon/GameBoard/BoardCell.h"

#include <memory>

namespace Algorithm_211388913_322330820
{
    // === Forward Declaration === //
    class CompetitionTankAlgorithm;

    using namespace UserCommon_211388913_322330820;
    class Tactic
    {
    protected:
    public:
        Tactic() {}
        virtual ~Tactic() = default;
        virtual ActionRequest getNextTacticAction(const CompetitionTankAlgorithm &algo) = 0;
        virtual std::unique_ptr<Tactic> clone() const = 0;
    };

    template <typename T>
    class TacticCRTP : public Tactic
    {
        std::unique_ptr<Tactic> clone() const override{
            return std::make_unique<T>(static_cast<const T&>(*this));
        }

    };

    class Bait : public TacticCRTP<Bait>
    {
    private:
        BoardCell base_location;
        BoardCell enemy_location;

        bool base_is_ready = false;

    public:
        Bait(const BoardCell &base_location, const BoardCell &enemy_location, bool base_is_ready);

        ActionRequest getNextTacticAction(const CompetitionTankAlgorithm &algo) override;
    };

    class Camper : public TacticCRTP<Camper>
    {
    private:
        BoardCell base_location;
        BoardCell enemy_location;

    public:
        Camper(const BoardCell &base_location, const BoardCell &enemy_location);
        ActionRequest getNextTacticAction(const CompetitionTankAlgorithm &algo) override;
    };

    class ScaredWolf : public TacticCRTP<ScaredWolf>
    {
    public:
        ActionRequest getNextTacticAction(const CompetitionTankAlgorithm &algo) override;
    };

    class Attacker : public TacticCRTP<Attacker>
    {
    private:
        BoardCell enemy_location;

    public:
        explicit Attacker(const BoardCell &enemy_location) : enemy_location(enemy_location)
        {
        }

        ActionRequest getNextTacticAction(const CompetitionTankAlgorithm &algo) override;
    };
}