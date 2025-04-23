#include "GameObjects.h"

#include "../GameBoard/GameBoard.h"
#include "../GameCollisionHandler/GameCollisionHandler.h"

Tank::Tank(GameBoard &b, GameObjectType t, Direction dir, int spd, int hp)
    : MovableObject(b, dir, spd, hp), type(t) {}

void Tank::printType() const
{
}

GameObjectType Tank::getObjectType() const
{
    return type;
}

void Tank::destroyed()
{
    setHP(0);
}

int Tank::getShells() const
{
    return shells;
}

void Tank::setShells(int new_shells)
{
    shells = new_shells;
}

void Tank::reload(int amount)
{
    shells += amount;
}

bool Tank::canShoot() const
{
    return shells > 0 && shoot_cooldown == 0;
}

bool Tank::notPendingBackwards() const
{
    return turns_to_wait_for_backward < 0;
}

int Tank::getBackwardWait() const
{
    return turns_to_wait_for_backward;
}

int Tank::getShootCooldown() const
{
    return shoot_cooldown;
}

void Tank::tickBackwardsWait()
{
    turns_to_wait_for_backward = max(turns_to_wait_for_backward - 1, -2);
    if (turns_to_wait_for_backward == 0)
    {
        if (auto opt_cell = board.getObjectLocation(this))
        {
            board.moveGameObject(this, opt_cell.value() - this->getDirection());
        }
    }
}

void Tank::tickShootCooldown()
{
    if (shoot_cooldown > 0)
    {
        --shoot_cooldown;
    }
}

bool Tank::action(TankAction command)
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
    tickShootCooldown();
    tickBackwardsWait();

    // validate and perform the action
    return validateAndPerformAction(command);
}

string Tank::getDrawing(DrawingType t) const
{
    bool is_t1 = this->getObjectType() == GameObjectType::tank1;

    switch (t)
    {
    case DrawingType::REGULAR:
        return is_t1 ? "🚙" : "🚜";
    case DrawingType::TENNIS:
        return is_t1 ? "🏸" : "🏓";
    case DrawingType::SCIFI:
        return is_t1 ? "👽" : "👾";
    case DrawingType::PIRATE:
        return is_t1 ? "⛴️" : "🚢";
    case DrawingType::MIDDLE_EAST:
        return is_t1 ? "🇮🇱" : "🇮🇷";
    default:
        return is_t1 ? "🚙" : "🚜";
    }
}

bool Tank::validateAndPerformAction(TankAction command)
{
    switch (command)
    {
    case TankAction::NOTHING:
        return true; // allways correct, does nothing.

    case TankAction::FORWARD:
        return performForwardAction();

    case TankAction::BACKWARD:
        return performBackwardAction();

    case TankAction::TURN45LEFT:
    case TankAction::TURN90LEFT:
    case TankAction::TURN45RIGHT:
    case TankAction::TURN90RIGHT:
       return performTurnAction(command);

    case TankAction::FIRE:
        return performShootAction();

    default:
        return false; // Should never get here.
        break;
    }
}

void Tank::cancelBackwardsWait()
{
    this->turns_to_wait_for_backward = -2;
}

void Tank::startBackwardsWait()
{
    this->turns_to_wait_for_backward = 2;
}

bool Tank::canImmediateBackwards() const
{
    return this->turns_to_wait_for_backward == -1;
}

bool Tank::isPendingBackwards() const
{
    return turns_to_wait_for_backward >= 0;
}

bool Tank::performForwardAction()
{
    // cancel backwards if relevant
    if (isPendingBackwards())
    {
        cancelBackwardsWait();
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

bool Tank::canMoveToCell(BoardCell target)
{
    // assuming that the target is in reach from current position
    return GameCollisionHandler::isObjectAllowedToStepOn(this->board, this->getObjectType(), target);
}

optional<BoardCell> Tank::getForwardCell()
{
    if (auto current_cell = this->getCurrentCell())
    {
        return current_cell.value() + this->getDirection();
    }
    return nullopt;
}

optional<BoardCell> Tank::getCurrentCell()
{
    if (auto opt_cell = board.getObjectLocation(this))
    {
        return opt_cell.value();
    }
    return nullopt;
}

optional<BoardCell> Tank::getBackwardCell()
{
    if (auto current_cell = this->getCurrentCell())
    {
        return current_cell.value() - this->getDirection();
    }
    return nullopt;
}

bool Tank::performBackwardAction()
{
    // can't backward while waiting
    if (isPendingBackwards())
    {
        return false;
    }

    // if can move backwards at current turn
    else if (canImmediateBackwards())
    {
        if (!canMoveBackwards()) // blocked by something
            return false;

        moveBackwards();
        return true;
    }

    // if not pending and can't immediately backwards - must initiate a backwards countdown
    else
    {
        startBackwardsWait();
        return true;
    }
}

void Tank::extendBackwardsStreak()
{
    turns_to_wait_for_backward = 0; // Stay in ready state for chained backwards
}

bool Tank::canMoveBackwards()
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
    extendBackwardsStreak();
}

bool Tank::performTurnAction(TankAction command)
{
    if (isPendingBackwards())
    { // ignore action
        return false;
    }

    turn(command);
    return true;
}

void Tank::turn(TankAction command)
{
    int steps = (command == TankAction::TURN45LEFT || command == TankAction::TURN45RIGHT) ? 1 : 2;
    auto rotationFunc = (command == TankAction::TURN45LEFT || command == TankAction::TURN90LEFT)
                            ? DirectionUtils::rotateLeft
                            : DirectionUtils::rotateRight;
    
    this->setDirection(rotationFunc(this->getDirection(), steps));
}

bool Tank::performShootAction(){
    if (this->isPendingBackwards())
        return false;

    if (this->canShoot()){
        shoot();
        return true;
    }

    return false; // On cloodowns, can't preform shooting act. 
}

void Tank::shoot()
{
    // get the shell spawn location (infront of barrel)
    auto opt_forward_cell = getForwardCell();
    if (!opt_forward_cell.has_value())
        return;
    BoardCell forward_cell = opt_forward_cell.value();

    // add shell to board
    board.addObject(new Shell(board, this->getDirection()), forward_cell);
    board.useTankShell(); // make sure bord knows that the shell was shot from a tank and not just added to board.

    // modify tank shoot cooldown and shells count
    shoot_cooldown = 5;
    shells--;
}

void Tank::moveToCell(BoardCell target){
    this->board.moveGameObject(this, target);
}