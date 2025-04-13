#include "GamePlayer.h"

Player::Player(){};
vector<TankAction> Player::getActions(){return vector<TankAction>(1,TankAction::FORWARD);};