#include "../common/TankAlgorithmFactory.h"

#include "../TankAlgorithm/AbstractTankAlgorithm.h"

#include <vector>

class TestAlgorithm : public AbstractTankAlgorithm
{
private:
    std::vector<ActionRequest> action_requests;

protected:
    ActionRequest getActionLogic() const override
    {
        size_t idx = this->getCurrentStep() - 1;
        if (idx < action_requests.size()) 
            return action_requests[idx];

        return ActionRequest::DoNothing;
    };

public:
    TestAlgorithm(std::vector<ActionRequest> action_requests, int tank_idx, int player_idx) :AbstractTankAlgorithm(tank_idx, player_idx), action_requests(action_requests) {};
    ~TestAlgorithm() override = default;
};

class TestAlgorithmFactory : public TankAlgorithmFactory
{
private:
    std::vector<ActionRequest> algorithm_requests;

public:
    TestAlgorithmFactory(std::vector<ActionRequest> algorithm_requests): algorithm_requests(algorithm_requests){};

    std::unique_ptr<TankAlgorithm> create(int player_index, int tank_index) const override{
        return std::make_unique<TestAlgorithm>(algorithm_requests, tank_index, player_index);
    }
};


