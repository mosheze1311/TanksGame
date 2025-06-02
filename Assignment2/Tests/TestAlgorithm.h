#pragma once
#include "../common/TankAlgorithmFactory.h"
#include "../common/TankAlgorithm.h"

#include <vector>

class TestAlgorithm : public TankAlgorithm
{
private:
    size_t step = -1;
    std::vector<ActionRequest> action_requests;

    int getCurrentStep()
    {
        return step;
    }
    void advanceStep()
    {
        ++step;
    }

public:
    TestAlgorithm(std::vector<ActionRequest> action_requests) : action_requests(action_requests) {};
    ~TestAlgorithm() override = default;

    ActionRequest getAction() override
    {
        this->advanceStep();
        size_t idx = this->getCurrentStep();
        if (idx < action_requests.size())
            return action_requests[idx];

        return ActionRequest::DoNothing;
    };

    void updateBattleInfo(BattleInfo &battle_info) override
    {
        (void)battle_info; // ignore pedantic errors
    };
};

class TestAlgorithmFactory : public TankAlgorithmFactory
{
private:
    std::vector<ActionRequest> algorithm1_requests;
    std::vector<ActionRequest> algorithm2_requests;

public:
    TestAlgorithmFactory(std::vector<ActionRequest> algorithm1_requests, std::vector<ActionRequest> algorithm2_requests) : algorithm1_requests(algorithm1_requests), algorithm2_requests(algorithm2_requests) {};
    TestAlgorithmFactory(std::vector<ActionRequest> algorithms_requests) : TestAlgorithmFactory(algorithms_requests, algorithms_requests) {};
    TestAlgorithmFactory(){};

    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override
    {
        (void)tank_index;
        if (player_index % 2 == 1){
            return std::make_unique<TestAlgorithm>(algorithm1_requests);
        }else
        {
            return std::make_unique<TestAlgorithm>(algorithm2_requests);
        }
    }
};
