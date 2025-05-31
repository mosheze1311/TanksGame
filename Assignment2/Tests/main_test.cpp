#include "GameManager/GameManager.h"
#include "PlayerFactory/MyPlayerFactory.h"
#include "TankAlgorithm/TestAlgorithm.h"

#include <memory>

std::unique_ptr<PlayerFactory> getPlayerFactory()
{
    return std::make_unique<MyPlayerFactory>();
}

bool validateArgc(int argc)
{
    if (argc != 2 && argc != 3)
    {
        Logger::runtime().log(std::format("Arguments do not match to programm - expecting 1 (or 2 for drawing), got {}", argc - 1));
        return false;
    }

    return true;
}
bool validateDrawingType (DrawingType& dt, int argc, char** argv){
    if (argc == 3) // if drawing is requested
    {
        try
        {
            dt = DrawingTypes::fromInt(std::stoi(argv[2]));
        }
        catch (...)
        {
            Logger::runtime().log(std::format("If given, 3rd argument must be a number"));
            return false;
        }
    }
    return true;
}

std::vector<std::unique_ptr<TankAlgorithmFactory>> getTankTests()
{
    std::vector<std::unique_ptr<TankAlgorithmFactory>> tests;
    std::vector<ActionRequest> shoot_cooldown_test_actions = {
        ActionRequest::Shoot,
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(shoot_cooldown_test_actions));

    std::vector<ActionRequest> backward_wait_test_actions = {
        ActionRequest::MoveBackward,
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(backward_wait_test_actions));

    std::vector<ActionRequest> cancel_backward_wait_1 = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(cancel_backward_wait_1));

    std::vector<ActionRequest> cancel_backward_wait_2 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward, // not sure what should happen here
        ActionRequest::Shoot,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(cancel_backward_wait_2));

    std::vector<ActionRequest> battle_info_while_waiting_backwards_1 = {
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(battle_info_while_waiting_backwards_1));

    std::vector<ActionRequest> battle_info_while_waiting_backwards_2 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::GetBattleInfo,
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(battle_info_while_waiting_backwards_2));

        // === Repeated Backward Movement (after initial wait) ===
    std::vector<ActionRequest> repeated_backward = {
        ActionRequest::MoveBackward,  // wait-1
        ActionRequest::DoNothing,     // wait-2
        ActionRequest::DoNothing,     // execute backward
        ActionRequest::MoveBackward,  // immediate backward (no wait)
        ActionRequest::MoveBackward   // immediate backward
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(repeated_backward));

    // === Illegal Shoot Spam Test (shoot cooldown test) ===
    std::vector<ActionRequest> shoot_spam = {
        ActionRequest::Shoot,         // legal
        ActionRequest::Shoot,         // illegal (cooldown)
        ActionRequest::DoNothing,     // wait-1
        ActionRequest::Shoot,         // illegal
        ActionRequest::DoNothing,     // wait-2
        ActionRequest::DoNothing,     // wait-3
        ActionRequest::Shoot          // legal again
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(shoot_spam));

    // === Cancel Backward with Forward ===
    std::vector<ActionRequest> cancel_backward = {
        ActionRequest::MoveBackward,  // triggers wait
        ActionRequest::MoveForward,   // should cancel wait and stay in place
        ActionRequest::MoveBackward,  // triggers new wait
        ActionRequest::DoNothing,     // wait
        ActionRequest::DoNothing,     // move backward
        ActionRequest::MoveForward    // should succeed
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(cancel_backward));

    // === Rotation Ignored During Backward Wait ===
    std::vector<ActionRequest> rotate_during_backward = {
        ActionRequest::MoveBackward,   // wait start
        ActionRequest::RotateLeft90,   // should be ignored
        ActionRequest::RotateRight90,  // should be ignored
        ActionRequest::DoNothing       // wait ends and moves backward
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(rotate_during_backward));

    // === Shell Cooldown Overlap With Move ===
    std::vector<ActionRequest> shoot_and_move = {
        ActionRequest::Shoot,         // legal
        ActionRequest::MoveForward,   // valid
        ActionRequest::MoveForward,   // valid
        ActionRequest::Shoot,         // illegal (cooldown)
        ActionRequest::DoNothing,     // wait
        ActionRequest::Shoot          // legal again
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(shoot_and_move));

    // === GetBattleInfo Chain Call ===
    std::vector<ActionRequest> chain_battle_info = {
        ActionRequest::GetBattleInfo,
        ActionRequest::GetBattleInfo,
        ActionRequest::GetBattleInfo,
        ActionRequest::DoNothing
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(chain_battle_info));

    // === DoNothing Spam ===
    std::vector<ActionRequest> idle_loop = {
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing
    };
    tests.push_back(std::make_unique<TestAlgorithmFactory>(idle_loop));

    return tests;
}

int doTests(const std::string &file_path, DrawingType dt)
{
    auto tests = getTankTests();
    char skip = 0;
    for (auto &test : tests)
    {
        GameManager game(getPlayerFactory(), std::move(test));
        // possibly run the game here or add a test result
        if (!game.readBoard(file_path))
            continue;

        game.run(dt);
        std::cout << "press enter to continue...";
        std::cin >> skip;
    }
    return 0;
}

int main(int argc, char **argv)
{
    if(!validateArgc(argc))
        return EXIT_SUCCESS;

    const std::string file_path = argv[1];

    DrawingType dt = DrawingType::NONE; // default
    if (!validateDrawingType(dt, argc, argv))
        return EXIT_SUCCESS;

    doTests(file_path, dt);
}
