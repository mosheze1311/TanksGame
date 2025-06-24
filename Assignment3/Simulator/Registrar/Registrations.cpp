#include "../../common/GameManagerRegistration.h"
#include "../../common/TankAlgorithmRegistration.h"
#include "../../common/PlayerRegistration.h"
#include "Registrar.h"

GameManagerRegistration::GameManagerRegistration(GameManagerFactory factory)
{
    Registrar<GameManagerFactory>::getRegistrar().addFactoryToLastEntry(factory);
}

PlayerRegistration::PlayerRegistration(PlayerFactory factory)
{
    Registrar<PlayerFactory>::getRegistrar().addFactoryToLastEntry(factory);
}

TankAlgorithmRegistration::TankAlgorithmRegistration(TankAlgorithmFactory factory)
{
    Registrar<TankAlgorithmFactory>::getRegistrar().addFactoryToLastEntry(factory);
};
