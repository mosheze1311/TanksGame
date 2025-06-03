#include "GameRulesTester.h"
#include "TestPlayer.h"
#include "TestAlgorithm.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

std::vector<GameRulesTester> getInputHandlingTests()
{
    std::vector<GameRulesTester> tests;
    tests.emplace_back("valid_input", "TestInputs/normal.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("eqaul_sign_padding", "TestInputs/equal_signs_padded.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("two_words_map", "TestInputs/two_words_map.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");

    // TODO: must check for documented input errors in file, these files should cause writings to input_errors
    tests.emplace_back("too_many_rows", "TestInputs/too_many_rows.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("too_many_cols", "TestInputs/too_many_cols.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("not_enough_rows", "TestInputs/not_enough_rows.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("not_enough_cols", "TestInputs/not_enough_cols.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");
    tests.emplace_back("unknown_chars_in_map", "TestInputs/unknown_chars_in_map.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/valid_input_test_output.txt");

    // cannot recover but should not crash
    tests.emplace_back("invalid_board_details1", "TestInputs/invalid_board_details1.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details2", "TestInputs/invalid_board_details2.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details3", "TestInputs/invalid_board_details3.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details4", "TestInputs/invalid_board_details4.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details5", "TestInputs/invalid_board_details5.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details6", "TestInputs/invalid_board_details6.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "ExpectedOutputs/invalid_input_test_output.txt", true);

    return tests;
}

std::vector<GameRulesTester> getGameRulesTests()
{
    std::vector<GameRulesTester> tests;
    std::vector<ActionRequest> shoot_cooldown_test_actions = {
        ActionRequest::Shoot,
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot, // ignore
        ActionRequest::Shoot,
    };
    tests.emplace_back("shoot_cooldown_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(shoot_cooldown_test_actions), "ExpectedOutputs/shoot_cooldown_test1_output.txt");

    std::vector<ActionRequest> shoot_cooldown_test2_actions = {
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft45,
        ActionRequest::RotateLeft90,
        ActionRequest::Shoot,
        ActionRequest::RotateRight45,
        ActionRequest::RotateRight90,
        ActionRequest::GetBattleInfo,
        ActionRequest::MoveBackward,
    };
    tests.emplace_back("shoot_cooldown_test2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(shoot_cooldown_test2_actions), "ExpectedOutputs/shoot_cooldown_test2_output.txt");

    std::vector<ActionRequest> backward_wait_test1_actions = {
        ActionRequest::MoveBackward,
        ActionRequest::RotateLeft45, // ignore
        ActionRequest::RotateLeft90, // ignore
        ActionRequest::DoNothing,
        ActionRequest::MoveBackward,
        ActionRequest::RotateRight45, // ignore
        ActionRequest::RotateRight90, // ignore
        ActionRequest::DoNothing,
        ActionRequest::MoveBackward,
        ActionRequest::Shoot, // ignore
    };
    tests.emplace_back("backward_wait_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(backward_wait_test1_actions), "ExpectedOutputs/backward_wait_test1_output.txt");

    std::vector<ActionRequest> backward_wait_test2_actions = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveBackward, // Ignore
        ActionRequest::MoveBackward, // Ignore
        ActionRequest::MoveBackward, // doubleBackwards (streak)
    };
    tests.emplace_back("backward_wait_test2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(backward_wait_test2_actions), "ExpectedOutputs/backward_wait_test2_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_1 = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft45,
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft90,
    };
    tests.emplace_back("cancel_backward_wait1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_1), "ExpectedOutputs/cancel_backward_wait_test1_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_2 = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateRight45,
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateRight90,
    };
    tests.emplace_back("cancel_backward_wait2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_2), "ExpectedOutputs/cancel_backward_wait_test2_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_3 = {
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::Shoot,
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateLeft45,
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateLeft90,
    };
    tests.emplace_back("cancel_backward_wait3", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_3), "ExpectedOutputs/cancel_backward_wait_test3_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_4 = {
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateRight45,
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateRight90,
    };
    tests.emplace_back("cancel_backward_wait4", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_4), "ExpectedOutputs/cancel_backward_wait_test4_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_5 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
    };
    tests.emplace_back("cancel_backward_wait5", "TestScenes/test_scene2.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_5), "ExpectedOutputs/cancel_backward_wait_test5_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_6 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::GetBattleInfo,
        ActionRequest::Shoot,
    };
    tests.emplace_back("cancel_backward_wait6", "TestScenes/test_scene2.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_6), "ExpectedOutputs/cancel_backward_wait_test6_output.txt");

    std::vector<ActionRequest> repeated_backward = {
        ActionRequest::MoveBackward, // wait-1
        ActionRequest::DoNothing,    // wait-2
        ActionRequest::DoNothing,    // execute backward
        ActionRequest::MoveBackward, // immediate backward (no wait)
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveBackward, // immediate backward
    };
    tests.emplace_back("repeated_backward_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(repeated_backward), "ExpectedOutputs/repeated_backward_test1_output.txt");

    std::vector<ActionRequest> moving_into_walls = {
        ActionRequest::MoveBackward, // wait-1
        ActionRequest::DoNothing,    // wait-2
        ActionRequest::DoNothing,    // execute backward
        ActionRequest::MoveBackward, // immediate backward (no wait)
        ActionRequest::MoveBackward, // immediate backward
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
    };
    tests.emplace_back("moving_into_walls_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(moving_into_walls), "ExpectedOutputs/moving_into_walls_test1_output.txt");

    std::vector<ActionRequest> p1_move_into_mine = {
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward, // from here on should not happen because p1 die
        ActionRequest::Shoot,
        ActionRequest::RotateLeft45,
    };
    tests.emplace_back("p1_move_into_mine_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(p1_move_into_mine), "ExpectedOutputs/p1_move_into_mine_test1_output.txt");

    std::vector<ActionRequest> p2_move_into_mine = {
        ActionRequest::RotateLeft90,
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::Shoot, // will not happen
    };
    tests.emplace_back("p2_move_into_mine_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(p2_move_into_mine), "ExpectedOutputs/p2_move_into_mine_test1_output.txt");

    std::vector<ActionRequest> both_players_move_into_mine = {
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft90, // from here on should not happen because players die
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
    };
    tests.emplace_back("players_move_into_mine_test", "TestScenes/test_scene3.txt", TestPlayerFactory{}, TestAlgorithmFactory(both_players_move_into_mine), "ExpectedOutputs/players_move_into_mine_test_output.txt");

    std::vector<ActionRequest> shells_collide_test = {
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
    };
    tests.emplace_back("shells_collide_on_cell_test", "TestScenes/test_scene4.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_collide_test), "ExpectedOutputs/shells_collide_on_cell_test_output.txt");
    tests.emplace_back("shells_collide_on_pass_test", "TestScenes/test_scene5.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_collide_test), "ExpectedOutputs/shells_collide_on_cell_test_output.txt");

    std::vector<ActionRequest> tanks_collide_test = {
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
    };
    tests.emplace_back("tanks_collide_on_cell_test", "TestScenes/test_scene4.txt", TestPlayerFactory{}, TestAlgorithmFactory(tanks_collide_test), "ExpectedOutputs/tanks_collide_on_cell_test_output.txt");
    tests.emplace_back("tanks_collide_on_pass_test", "TestScenes/test_scene5.txt", TestPlayerFactory{}, TestAlgorithmFactory(tanks_collide_test), "ExpectedOutputs/tanks_collide_on_pass_test_output.txt");

    std::vector<ActionRequest> shells_vertical_collision_test_p1 = {
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
    };
    std::vector<ActionRequest> shells_vertical_collision_test_p2 = {
        ActionRequest::RotateLeft90,
        ActionRequest::Shoot,
    };
    tests.emplace_back("shells_vertical_collision_test", "TestScenes/test_scene9.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_vertical_collision_test_p1, shells_vertical_collision_test_p2), "ExpectedOutputs/shells_vertical_collision_test_output.txt");

    std::vector<ActionRequest> immediate_win_test = {
        ActionRequest::GetBattleInfo,
    };
    tests.emplace_back("immediate_win_test", "TestScenes/test_scene6.txt", TestPlayerFactory{}, TestAlgorithmFactory(immediate_win_test), "ExpectedOutputs/immediate_win_test_output.txt");

    std::vector<ActionRequest> immediate_tie_test = {
        ActionRequest::GetBattleInfo,
    };
    tests.emplace_back("immediate_tie_test", "TestScenes/test_scene7.txt", TestPlayerFactory{}, TestAlgorithmFactory(immediate_tie_test), "ExpectedOutputs/immediate_tie_test_output.txt");

    std::vector<ActionRequest> two_cells_test_p1 = {
        ActionRequest::RotateRight90,
        ActionRequest::RotateRight90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
    };
    std::vector<ActionRequest> two_cells_test_p2 = {
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
    };
    tests.emplace_back("two_cells_collision_test", "TestScenes/test_scene8.txt", TestPlayerFactory{}, TestAlgorithmFactory(two_cells_test_p1, two_cells_test_p2), "ExpectedOutputs/two_cells_collision_test_output.txt");

    std::vector<ActionRequest> done_shells_tie = {
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
    };
    tests.emplace_back("done_shells_tie_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{done_shells_tie}, "ExpectedOutputs/done_shells_tie_test_output.txt");

    std::vector<ActionRequest> done_shells_regular_tie = {
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,

        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
    };
    tests.emplace_back("done_shells_regular_tie_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{done_shells_regular_tie}, "ExpectedOutputs/done_shells_regular_tie_test_output.txt");

    std::vector<ActionRequest> ignore_killed = {
        ActionRequest::MoveBackward,
        ActionRequest::Shoot,
        ActionRequest::Shoot,
    };
    tests.emplace_back("ignored_killed_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{ignore_killed}, "ExpectedOutputs/ignored_killed_test_output.txt");

    std::vector<ActionRequest> shells_speed_test_p1 = {
        ActionRequest::Shoot,
    };
    tests.emplace_back("shells_speed_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{shells_speed_test_p1, {}}, "ExpectedOutputs/shells_speed_test_output.txt");

    return tests;
}

std::vector<GameRulesTester> getMultipleTanksTests()
{
    std::vector<GameRulesTester> tests;

    std::vector<ActionRequest> one_die_tie_requests = {
        ActionRequest::Shoot,
    };
    tests.emplace_back("one_die_tie_test", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{one_die_tie_requests}, "ExpectedOutputs/one_die_tie_test_output.txt");

    std::vector<ActionRequest> one_die_zero_shells_tie_requests = {
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
    };
    tests.emplace_back("one_die_zero_shells_tie_test", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{one_die_zero_shells_tie_requests}, "ExpectedOutputs/one_die_zero_shells_tie_test_output.txt");

    std::vector<ActionRequest> p1_win_multitank_p1 = {
        ActionRequest::Shoot};
    std::vector<ActionRequest> p1_win_multitank_p2 = {};
    tests.emplace_back("p1_win_multitank_test1", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{p1_win_multitank_p1, p1_win_multitank_p2}, "ExpectedOutputs/p1_win_multitank_test1_output.txt");

    std::vector<ActionRequest> p1_win_multitank = {
        ActionRequest::MoveForward,
    };
    tests.emplace_back("p1_win_multitank_test2", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{p1_win_multitank}, "ExpectedOutputs/p1_win_multitank_test2_output.txt");

    std::vector<ActionRequest> multiple_objects_collision_p1 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::MoveBackward, // one object should succeed backwards and extend streak, the second should not succeed and start wait again
        ActionRequest::RotateLeft90,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward,
    };
    std::vector<ActionRequest> multiple_objects_collision_p2 = {
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,        
        ActionRequest::DoNothing,
        ActionRequest::RotateLeft90,
        ActionRequest::Shoot,
    };
    tests.emplace_back("multiple_objects_collision_test", "TestScenes/test_scene12.txt", TestPlayerFactory{}, TestAlgorithmFactory{multiple_objects_collision_p1, multiple_objects_collision_p2}, "ExpectedOutputs/multiple_objects_collision_test_output.txt");

    return tests;
};

int evalTests(const std::vector<GameRulesTester> &tests)
{
    int passed_tests = 0;
    for (const GameRulesTester &test : tests)
    {
        bool success = test.evalTest();
        if (success)
        {
            ++passed_tests;
        }
        else
        {
            break;
        }
    }
    std::cout << "Passed " << passed_tests << "/" << tests.size() << " tests\n"
              << std::endl;
    return 0;
}

void clearConsole()
{
    std::cout << "\033[2J\033[H"; // Clear screen and move cursor to top-left
}

int main()
{

    clearConsole();

    std::cout << "=== Input Handling Tests ===" << std::endl;
    evalTests(getInputHandlingTests());

    std::cout << "=== Rules Tests ===" << std::endl;
    evalTests(getGameRulesTests());

    std::cout << "=== Multiple Tanks Rules Tests ===" << std::endl;
    evalTests(getMultipleTanksTests());

    // std::cout << "=== Bonus Tests ===" << std::endl;
}
