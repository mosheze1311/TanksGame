#include "GameObjects.h"

#include "../GameBoard/GameBoard.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"
namespace UserCommon_211388913_322330820
{

#define tank_speed 1 // not in config because should not be changed

    //=== Constructors ===
    Tank::Tank(GameBoard &b, GameObjectType t, Direction dir, size_t tank_num_shells)
        : MovableObject(b, dir, tank_speed, ConfigReader::getConfig().getTankHp(), ConfigReader::getConfig().getTankCollisionDamage()), type(t), shells(tank_num_shells) {}

    //=== Getters ===
    GameObjectType Tank::getObjectType() const
    {
        return type;
    }

    int Tank::getShells() const
    {
        return shells;
    }

    //=== Setters ===
    void Tank::setShells(int new_shells)
    {
        shells = new_shells;
    }

    //=== Shooting Logic ===
    void Tank::reload(int amount)
    {
        shells += amount;
    }

    bool Tank::canShoot() const
    {
        return shells > 0 && cooldown_handler.isDoneShootCooldown();
    }

    //=== Action Logic ===
    bool Tank::playTankRound(ActionRequest command)
    {
        /*
         * This function checks if a tank can do a specific action, and if so – does it.
         * We use the Direction enum and helper functions to check the validity of each action
         * the tank may take in a specific turn. The tank can fire, rotate, and move forward/backward
         * while following certain rules.
         *  1.   If moving backward, the tank must wait 2 turns
         *       (not doing any actions) unless the action is forward, which canceles the backward command.
         *  2.   special case - the tank can move backward consecutively if the command on the 2 turn after a
         *       backward call is backward. only breaks when a non-backward command is called.
         *  3.   If the tank shoots, it must have a 4 turn cooldown, which means it won't be able to shoot
         *       for the next 4 turns.
         *  4.   There must be 1 action per turn.
         */

        // tick clock for the shoot cooldown and backward wait
        cooldown_handler.tickCooldowns();

        bool was_backwards_due = cooldown_handler.isBackwardsDue(); // must be before validateAndPerformAction
        bool is_valid = validateAndPerformAction(command);
        performPostponedBackward(was_backwards_due); // must be after validateAndPerformAction
        return is_valid;
    }

    bool Tank::validateAndPerformAction(ActionRequest command)
    {
        switch (command)
        {
        case ActionRequest::DoNothing:
            return true; // allways correct, does nothing.

        case ActionRequest::GetBattleInfo:
            return performBattleInfoAction();

        case ActionRequest::MoveForward:
            return performForwardAction();

        case ActionRequest::MoveBackward:
            return performBackwardAction();

        case ActionRequest::RotateLeft45:
        case ActionRequest::RotateLeft90:
        case ActionRequest::RotateRight45:
        case ActionRequest::RotateRight90:
            return performTurnAction(command);

        case ActionRequest::Shoot:
            return performShootAction();

        default:
            return false; // Should never get here.
            break;
        }
    }

    //=== Movement ===
    std::optional<BoardCell> Tank::getForwardCell() const
    {
        if (auto current_cell = this->getCurrentCell())
        {
            return board.getNextCellInDirection(current_cell.value(), this->getDirection());
        }
        return std::nullopt;
    }

    std::optional<BoardCell> Tank::getCurrentCell() const
    {
        if (auto opt_cell = board.getObjectLocation(this))
        {
            return opt_cell.value();
        }
        return std::nullopt;
    }

    std::optional<BoardCell> Tank::getBackwardCell() const
    {
        if (auto current_cell = this->getCurrentCell())
        {
            return board.getNextCellInDirection(current_cell.value(), DirectionUtils::getOppositeDirection(this->getDirection()));
        }
        return std::nullopt;
    }

    bool Tank::performForwardAction()
    {
        // cancel backwards if relevant
        if (cooldown_handler.isPendingBackwards())
        {
            cooldown_handler.cancelBackwardWait();
            return true;
        }

        // move forwards if allowed to
        if (auto opt_forward_cell = this->getForwardCell())
        {
            BoardCell forward_cell = opt_forward_cell.value();
            if (canMoveToCell(forward_cell))
            {
                moveToCell(forward_cell);
                return true;
            }
        }
        return false;
    }

    bool Tank::canMoveToCell(BoardCell target) const
    {
        // assuming that the target is in reach from current position
        return GameCollisionHandler::isObjectAllowedToStepOn(this->board, this->getObjectType(), target);
    }

    void Tank::moveToCell(BoardCell target)
    {
        this->board.moveGameObject(this, target);
    }

    bool Tank::performBackwardAction()
    {
        // can't backward while waiting
        if (cooldown_handler.isPendingBackwards())
        {
            return false;
        }

        // if can move backwards at current turn
        else if (cooldown_handler.canImmediateBackwards())
        {
            if (!canMoveBackwards()) // blocked by something
                return false;

            moveBackwards();
            return true;
        }

        // if not pending and can't immediately backwards - must initiate a backwards countdown
        else
        {
            cooldown_handler.startBackwardWait();
            return true;
        }
    }

    void Tank::performPostponedBackward(bool was_due_before_action)
    {
        /*
            this function must be called after handling this step ActionRequest:

            'was_due_before_action' prevent double backwards when extending backwards streak
            'isBackwardsDue' prevent backwards after a canceling-backwards action

            'cancelBackwardsWait' must be called after the handling of this step ActionRequest to prevent additional action
        */

        if (!cooldown_handler.isBackwardsDue() || !was_due_before_action)
            return;

        if (canMoveBackwards())
        {
            moveBackwards();
        }
        else
        {
            cooldown_handler.cancelBackwardWait();
        }
    }

    bool Tank::canMoveBackwards() const
    {
        auto opt_backward_cell = this->getBackwardCell();
        if (!opt_backward_cell.has_value())
            return false;

        BoardCell backward_cell = opt_backward_cell.value();
        return canMoveToCell(backward_cell);
    }

    void Tank::moveBackwards()
    {
        auto opt_backward_cell = this->getBackwardCell();
        if (!opt_backward_cell.has_value())
            return;

        BoardCell backward_cell = opt_backward_cell.value();

        // actual movement
        moveToCell(backward_cell);
        cooldown_handler.extendBackwardsStreak();
    }

    //=== Turning ===
    bool Tank::performTurnAction(ActionRequest command)
    {
        if (cooldown_handler.isPendingBackwards())
        { // ignore action
            return false;
        }

        turn(command);
        return true;
    }

    void Tank::turn(ActionRequest command)
    {
        int steps = (command == ActionRequest::RotateLeft45 || command == ActionRequest::RotateRight45) ? 1 : 2;
        auto rotationFunc = (command == ActionRequest::RotateLeft45 || command == ActionRequest::RotateLeft90)
                                ? DirectionUtils::rotateLeft
                                : DirectionUtils::rotateRight;

        this->setDirection(rotationFunc(this->getDirection(), steps));
    }

    //=== Shooting ===
    bool Tank::performShootAction()
    {
        if (this->cooldown_handler.isPendingBackwards())
            return false;

        if (this->canShoot())
        {
            shoot();
            return true;
        }

        return false; // On cooldowns, can't preform shooting act.
    }

    void Tank::shoot()
    {
        // get the shell spawn location (infront of barrel)
        auto opt_forward_cell = getForwardCell();
        if (!opt_forward_cell.has_value())
            return;
        BoardCell forward_cell = opt_forward_cell.value();

        // add shell to board
        board.addTankShell(std::make_unique<Shell>(board, this->getDirection()), forward_cell);

        // modify tank shoot cooldown and shells count
        cooldown_handler.beginShoot();
        shells--;
    }

    // === Battle Info === //
    bool Tank::performBattleInfoAction()
    {
        this->cooldown_handler.cancelBackwardWait();
        return true;
    };

    //=== Drawing ===
    std::string Tank::getDrawing(DrawingType t) const
    {
        static const std::vector<std::string> regular_emojis = {"🚙", "🚜", "🚗", "🚕", "🚓", "🚒", "🏎️ ", "🛻 ", "🚚"};
        static const std::vector<std::string> tennis_emojis = {"🏸", "🏓", "T", "🥎", "🏐", "🏉", "🏈", "⚽", "⚾"};
        static const std::vector<std::string> scifi_emojis = {"👽", "👾", "🤖", "🛸", "🚀", "🛰️ ", "🌌", "🔫", "🧬"};
        static const std::vector<std::string> pirate_emojis = {"⛴️ ", "🚢", "⚓", "🏴", "🦜", "🗺️ ", "🏝️ ", "💣", "🪙 "};
        static const std::vector<std::string> mideast_emojis = {"🇮🇱", "🇮🇷", "🇸🇦", "🇪🇬", "🇹🇷", "🇸🇾", "🇯🇴", "🇮🇶", "🇱🇧"};

        int index = static_cast<int>(this->getObjectType()) - static_cast<int>(GameObjectType::TANK1);
        index = std::clamp(index, 0, 8); // clamp to valid emoji range

        switch (t)
        {
        case DrawingType::REGULAR:
            return regular_emojis[index];
        case DrawingType::TENNIS:
            return tennis_emojis[index];
        case DrawingType::SCIFI:
            return scifi_emojis[index];
        case DrawingType::PIRATE:
            return pirate_emojis[index];
        case DrawingType::MIDDLE_EAST:
            return mideast_emojis[index];
        default:
            return regular_emojis[index];
        }
    }
}