#include "GameObjects.h"

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

bool Tank::canMoveOrRotate() const
{
    return turns_to_wait_for_backward < 0;
}

void Tank::tickWait()
{
    turns_to_wait_for_backward = max(turns_to_wait_for_backward - 1, -2);
    if (turns_to_wait_for_backward == 0){
        board.moveGameObject(this, board.getobjectLocation(this) - this->direction);
    }
}

void Tank::tickShootCooldown()
{
    if (shoot_cooldown > 0)
    {
        --shoot_cooldown;
    }
}

void Tank::action(TankAction command)
{
    GameBoard &board = this->board;
    BoardCell curr_cell = board.getobjectLocation(this);
    Direction dir = this->direction;

    tickShootCooldown();
    tickWait();

    switch (command)
    {
    case TankAction::NOTHING:
        break;

    case TankAction::FORWARD:
    {

        if (canMoveOrRotate())
        {
            board.moveGameObject(this, curr_cell + dir);
        }
        turns_to_wait_for_backward = -2;
        break;
    }

    case TankAction::BACKWARD:
    {
        if (turns_to_wait_for_backward == -2)
        {
            // Start backward wait period
            turns_to_wait_for_backward = 2;
        }
        else if (turns_to_wait_for_backward == -1)
        {
            // Perform the move
            board.moveGameObject(this, curr_cell - dir);
            turns_to_wait_for_backward = 0; // Stay in ready state for chained backwards
        }
        break;
    }

    case TankAction::TURN45LEFT:
    case TankAction::TURN90LEFT:
    case TankAction::TURN45RIGHT:
    case TankAction::TURN90RIGHT:
    {
        if (canMoveOrRotate())
        {
            int step = (command == TankAction::TURN45LEFT || command == TankAction::TURN45RIGHT) ? 1 : 2;
            if (command == TankAction::TURN45LEFT || command == TankAction::TURN90LEFT)
            {
                this->setDirection(dir - step);
            }
            else
            {
                this->setDirection(dir + step);
            }
        }
        break;
    }

    case TankAction::FIRE:
    {
        if (canShoot() && canMoveOrRotate())
        {
            Shell *new_shell = new Shell(board, dir);
            board.addObject(new_shell, curr_cell + dir);
            shoot_cooldown = 5;
        }
        break;
    }

    default:
        break;
    }
}