#include "GamePlayer.h"

Player::Player(){};
vector<Action> Player::getActions(){return vector<Action>(1,Action::FORWARD);};