#include "GameObjects.h"

#include "../GameBoard/GameBoard.h"

Tank::Tank(GameBoard &b, GameObjectType t, Direction dir, int spd, int hp)
    : MovableObject(b, dir, spd, hp), type(t) {}

void Tank::printType() const
{
}

GameObjectType Tank::getObjectType() const
{
    return type;
}

void Tank::shoot()
{
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
    return shoot_cooldown == 0;
}

bool Tank::onBackwardCooldown() const
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

void Tank::tickWait()
{
    turns_to_wait_for_backward = max(turns_to_wait_for_backward - 1, -2);
    if (turns_to_wait_for_backward == 0)
    {
        if (auto opt_cell = board.getObjectLocation(this))
        {
            board.moveGameObject(this, *opt_cell - this->direction);
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

bool Tank::canTankMove(BoardCell target)
{

    auto objects = board.getObjectsOnCell(target);
    bool is_wall = false;

    for (GameObject *obj : objects)
    {
        if (obj->getObjectType() == GameObjectType::wall)
        {
            is_wall = true;
        }
    }

    if (!is_wall)
    {
        board.moveGameObject(this, target);
    }

    return is_wall;
}

bool Tank::action(TankAction command)
{
    /*
 * This function checks if a tank can do a specific action, and if so – does it.
 * We use the Direction enum and helper functions to check the validity of each action
 * the tank may take in a specific turn. The tank can fire, rotate, and move forward/backward
 * while following certain rules. If moving backward, the tank must wait 2 turns 
 * (not doing any actions) unless the action is forward, which canceles the backward command.
 * special case- the tank can move backward consecutively if the command on the 2 turn after a
 * backward call is backward. only breaks when a non-backward command is called.
 * If the tank shoots, it must have a 4 turn cooldown, which means it won't be able to shoot 
 * for the next 4 turns.
 * There must be 1 action per turn. 
 */
    GameBoard &board = this->board;
    BoardCell curr_cell;
    if (auto opt_cell = board.getObjectLocation(this))
    {
        curr_cell = *opt_cell;
    }
    else
    {
        return false; // Tank not in board.
    }
    Direction dir = this->direction;

    tickShootCooldown();
    tickWait();

    switch (command)
    {
    case TankAction::NOTHING:
        return true; // allways correct, does nothing. 
        break;

    case TankAction::FORWARD:
    {

        if (onBackwardCooldown())
        {
            return !this->canTankMove(curr_cell + dir); // canTankMove checks and moves a tank if possibale.
            //  It returns ture if there is a wall (can't move) and false otherwise.
        }
        turns_to_wait_for_backward = -2;
        return true; // Cancels a previous backward command, is valid.
        break;
    }

    case TankAction::BACKWARD:
    {
        if (turns_to_wait_for_backward == -2)
        {
            // Start backward wait period
            turns_to_wait_for_backward = 2;
            return true;  
        }
        else if (turns_to_wait_for_backward == -1)
        {
            // Perform the move
            this->canTankMove(curr_cell + dir);
            turns_to_wait_for_backward = 0; // Stay in ready state for chained backwards
            
        }
        return true; // Tank moved backward or Backward command called (2 turns wating time). Both valid.
        break;
    }

    case TankAction::TURN45LEFT:
    case TankAction::TURN90LEFT:
    case TankAction::TURN45RIGHT:
    case TankAction::TURN90RIGHT:
    {
        if (onBackwardCooldown())
        {
            int step = (command == TankAction::TURN45LEFT || command == TankAction::TURN45RIGHT) ? 1 : 2;
            if (command == TankAction::TURN45LEFT || command == TankAction::TURN90LEFT)
            {
                this->setDirection(DirectionUtils::rotateRight(dir, step));
            }
            else
            {
                this->setDirection(DirectionUtils::rotateLeft(dir, step));
            }
            return true; // Can rotate left or right.
        }

        return false; // On backward cooldown. TODO - check if this counts as valid/invalid
        break;
    }

    case TankAction::FIRE:
    {
        if (canShoot() && onBackwardCooldown())
        {
            Shell *new_shell = new Shell(board, dir);
            board.addObject(new_shell, curr_cell + dir);
            shoot_cooldown = 5;
            return true; // Can preform shooting act.
        }

        return false; // On cloodowns, can't prefome shooting act.
        break;
    }

    default:
    return false; // Should never get here.  
        break;
    }
}