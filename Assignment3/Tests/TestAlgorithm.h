#pragma once
#include "../common/TankAlgorithmFactory.h"
#include "../common/TankAlgorithm.h"

#include <vector>

class TestAlgorithm : public TankAlgorithm
{
private:
    // === Attributes === //
    size_t step = -1;
    std::vector<ActionRequest> action_requests;

    // === Steps Management === //
    int getCurrentStep()
    {
        return step;
    }
    
    void advanceStep()
    {
        ++step;
    }

    // === Handle With Player File === //
    std::string getPlayerFile() const
    {
        return "GetBattleInfoOutputs/get_battle_info_log.txt";
    }
    
    void logBattleInfo() const
    {
        std::ofstream o_file(getPlayerFile(), std::ios::app);
        if (!o_file)
            return;

        o_file << "GetBattleInfo" << std::endl;
    }

public:
    // === Constructor === //
    TestAlgorithm(std::vector<ActionRequest> action_requests) : action_requests(action_requests) {};
    
    // === Destructor === //
    ~TestAlgorithm() override = default;

    // === Interface Functions === //
    ActionRequest getAction() override
    {
        this->advanceStep();
        size_t idx = this->getCurrentStep();
        if (idx < action_requests.size()){
            if (action_requests[idx] == ActionRequest::GetBattleInfo){
                logBattleInfo();
            }   

            return action_requests[idx];
        }


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
    // === Attributes === //
    std::vector<ActionRequest> algorithm1_requests;
    std::vector<ActionRequest> algorithm2_requests;

public:
    // === Constructors === //
    TestAlgorithmFactory(std::vector<ActionRequest> algorithm1_requests, std::vector<ActionRequest> algorithm2_requests) : algorithm1_requests(algorithm1_requests), algorithm2_requests(algorithm2_requests) {};
    TestAlgorithmFactory(std::vector<ActionRequest> algorithms_requests) : TestAlgorithmFactory(algorithms_requests, algorithms_requests) {};
    TestAlgorithmFactory(){};

    // === Destructor === //
    ~TestAlgorithmFactory() override = default;
    
    // === Interface Function === //
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
