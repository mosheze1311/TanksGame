#include "GameRulesTester.h"
#include "TestPlayer.h"
#include "TestAlgorithm.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

/*
 * ========================== Game Rules Tester ==========================
 *
 * This tester is designed **only** to validate the correctness of the **Game Rules**
 * as enforced by the GameManager — including board state transitions and the output
 * format of step and conclusion logs.
 * It does **not** test Player logic or TankAlgorithm decision-making.
 *
 * ========================== Assumptions & Approved Behavior ==========================
 * The following game rule behaviors are assumed and were approved:
 * 1. A GetBattleInfo request overrides a MoveBackward request issued for the same step (i.e. cancels wait, and performed).
 * 2. If a tank is scheduled to move backward, but during the same step issued a
 *    MoveForward or GetBattleInfo request, the backward movement is canceled.
 *
 * These decisions may effect some tests.
 *
 * ========================== Integration Instructions ==========================
 * To integrate this tester with your own TanksGame implementation:
 *
 * 1. Copy the entire tester folder into your project directory.
 * 2. In "GameRulesTester.h":
 *    a. Update the `#include` path to match your GameManager header file,
 *       unless it is already named `GameManager.h`.
 *    b. In the getManagerOutputFile() method, a `static const string` is used to
 *       align with our Logger's design. If this causes issues, you may remove the
 *       `static` qualifier.
 *
 * ========================== How to Use ==========================
 * Run 'make && ./game_tester'
 * Note: when running the tester all prints to the terminal are dismissed — do not be alarmed.
 *
 * =======================================================================
 */

std::vector<GameRulesTester> getInputHandlingTests()
{
    std::vector<GameRulesTester> tests;
    tests.emplace_back("valid_input", "TestInputs/normal.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("eqaul_sign_padding", "TestInputs/equal_signs_padded.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("two_words_map", "TestInputs/two_words_map.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");

    // These tests should trigger writing to input_errors.txt
    tests.emplace_back("too_many_rows", "TestInputs/too_many_rows.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("too_many_cols", "TestInputs/too_many_cols.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("not_enough_rows", "TestInputs/not_enough_rows.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("not_enough_cols", "TestInputs/not_enough_cols.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");
    tests.emplace_back("unknown_chars_in_map", "TestInputs/unknown_chars_in_map.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "valid_input_test_output.txt");

    // cannot recover but should not crash - should trigger printing error message to output
    tests.emplace_back("invalid_board_details1 - wrong key", "TestInputs/invalid_board_details1.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details2 - missing key", "TestInputs/invalid_board_details2.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details3 - keys out of order", "TestInputs/invalid_board_details3.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details4 - non integer key", "TestInputs/invalid_board_details4.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details5 - no map title", "TestInputs/invalid_board_details5.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details6 - empty", "TestInputs/invalid_board_details6.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);
    tests.emplace_back("invalid_board_details7 - file not exist", "TestInputs/invalid_board_details7.txt", TestPlayerFactory{}, TestAlgorithmFactory(), "invalid_input_test_output.txt", true);

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
    tests.emplace_back("shoot_cooldown_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(shoot_cooldown_test_actions), "shoot_cooldown_test1_output.txt");

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
    tests.emplace_back("shoot_cooldown_test2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(shoot_cooldown_test2_actions), "shoot_cooldown_test2_output.txt");

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
    tests.emplace_back("backward_wait_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(backward_wait_test1_actions), "backward_wait_test1_output.txt");

    std::vector<ActionRequest> backward_wait_test2_actions = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveBackward, // Ignore
        ActionRequest::MoveBackward, // Ignore
        ActionRequest::MoveBackward, // doubleBackwards (streak)
    };
    tests.emplace_back("backward_wait_test2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(backward_wait_test2_actions), "backward_wait_test2_output.txt");

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
    tests.emplace_back("cancel_backward_wait1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_1), "cancel_backward_wait_test1_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_2 = {
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateRight45,
        ActionRequest::MoveBackward,
        ActionRequest::MoveForward,
        ActionRequest::RotateRight90,
    };
    tests.emplace_back("cancel_backward_wait2", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_2), "cancel_backward_wait_test2_output.txt");

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
    tests.emplace_back("cancel_backward_wait3", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_3), "cancel_backward_wait_test3_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_4 = {
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateRight45,
        ActionRequest::MoveBackward,
        ActionRequest::GetBattleInfo,
        ActionRequest::RotateRight90,
    };
    tests.emplace_back("cancel_backward_wait4", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_4), "cancel_backward_wait_test4_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_5 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::MoveForward,
        ActionRequest::Shoot,
    };
    tests.emplace_back("cancel_backward_wait5", "TestScenes/test_scene2.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_5), "cancel_backward_wait_test5_output.txt");

    std::vector<ActionRequest> cancel_backward_wait_6 = {
        ActionRequest::MoveBackward,
        ActionRequest::DoNothing,
        ActionRequest::GetBattleInfo,
        ActionRequest::Shoot,
    };
    tests.emplace_back("cancel_backward_wait6", "TestScenes/test_scene2.txt", TestPlayerFactory{}, TestAlgorithmFactory(cancel_backward_wait_6), "cancel_backward_wait_test6_output.txt");

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
    tests.emplace_back("repeated_backward_test1", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(repeated_backward), "repeated_backward_test1_output.txt");

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
    tests.emplace_back("moving_into_walls_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(moving_into_walls), "moving_into_walls_test1_output.txt");

    std::vector<ActionRequest> p1_move_into_mine = {
        ActionRequest::RotateLeft90,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward, // from here on should not happen because p1 die
        ActionRequest::Shoot,
        ActionRequest::RotateLeft45,
    };
    tests.emplace_back("p1_move_into_mine_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(p1_move_into_mine), "p1_move_into_mine_test1_output.txt");

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
    tests.emplace_back("p2_move_into_mine_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory(p2_move_into_mine), "p2_move_into_mine_test1_output.txt");

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
    tests.emplace_back("players_move_into_mine_test", "TestScenes/test_scene3.txt", TestPlayerFactory{}, TestAlgorithmFactory(both_players_move_into_mine), "players_move_into_mine_test_output.txt");

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
    tests.emplace_back("shells_collide_on_cell_test", "TestScenes/test_scene4.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_collide_test), "shells_collide_on_cell_test_output.txt");
    tests.emplace_back("shells_collide_on_pass_test", "TestScenes/test_scene5.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_collide_test), "shells_collide_on_cell_test_output.txt");

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
    tests.emplace_back("tanks_collide_on_cell_test", "TestScenes/test_scene4.txt", TestPlayerFactory{}, TestAlgorithmFactory(tanks_collide_test), "tanks_collide_on_cell_test_output.txt");
    tests.emplace_back("tanks_collide_on_pass_test", "TestScenes/test_scene5.txt", TestPlayerFactory{}, TestAlgorithmFactory(tanks_collide_test), "tanks_collide_on_pass_test_output.txt");

    std::vector<ActionRequest> shells_vertical_collision_test_p1 = {
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
    };
    std::vector<ActionRequest> shells_vertical_collision_test_p2 = {
        ActionRequest::RotateLeft90,
        ActionRequest::Shoot,
    };
    tests.emplace_back("shells_vertical_collision_test", "TestScenes/test_scene9.txt", TestPlayerFactory{}, TestAlgorithmFactory(shells_vertical_collision_test_p1, shells_vertical_collision_test_p2), "shells_vertical_collision_test_output.txt");

    std::vector<ActionRequest> immediate_win_test = {
        ActionRequest::GetBattleInfo,
    };
    tests.emplace_back("immediate_win_test", "TestScenes/test_scene6.txt", TestPlayerFactory{}, TestAlgorithmFactory(immediate_win_test), "immediate_win_test_output.txt");

    std::vector<ActionRequest> immediate_tie_test = {
        ActionRequest::GetBattleInfo,
    };
    tests.emplace_back("immediate_tie_test", "TestScenes/test_scene7.txt", TestPlayerFactory{}, TestAlgorithmFactory(immediate_tie_test), "immediate_tie_test_output.txt");

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
    tests.emplace_back("two_cells_collision_test", "TestScenes/test_scene8.txt", TestPlayerFactory{}, TestAlgorithmFactory(two_cells_test_p1, two_cells_test_p2), "two_cells_collision_test_output.txt");

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
    tests.emplace_back("done_shells_tie_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{done_shells_tie}, "done_shells_tie_test_output.txt");

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
    tests.emplace_back("done_shells_regular_tie_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{done_shells_regular_tie}, "done_shells_regular_tie_test_output.txt");

    std::vector<ActionRequest> ignore_killed = {
        ActionRequest::MoveBackward,
        ActionRequest::Shoot,
        ActionRequest::Shoot,
    };
    tests.emplace_back("ignored_killed_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{ignore_killed}, "ignored_killed_test_output.txt");

    std::vector<ActionRequest> shells_speed_test_p1 = {
        ActionRequest::Shoot,
    };
    tests.emplace_back("shells_speed_test", "TestScenes/test_scene10.txt", TestPlayerFactory{}, TestAlgorithmFactory{shells_speed_test_p1, {}}, "shells_speed_test_output.txt");

    std::vector<ActionRequest> wall_hp_test = {
        ActionRequest::Shoot,
        ActionRequest::MoveForward,
        ActionRequest::MoveForward,
        ActionRequest::DoNothing,
        ActionRequest::DoNothing,
        ActionRequest::Shoot,
        ActionRequest::MoveForward,
    };
    tests.emplace_back("wall_hp_test", "TestScenes/test_scene1.txt", TestPlayerFactory{}, TestAlgorithmFactory{wall_hp_test}, "wall_hp_test_output.txt");

    return tests;
}

std::vector<GameRulesTester> getMultipleTanksTests()
{
    std::vector<GameRulesTester> tests;

    std::vector<ActionRequest> one_die_tie_requests = {
        ActionRequest::Shoot,
    };
    tests.emplace_back("one_die_tie_test", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{one_die_tie_requests}, "one_die_tie_test_output.txt");

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
    tests.emplace_back("one_die_zero_shells_tie_test", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{one_die_zero_shells_tie_requests}, "one_die_zero_shells_tie_test_output.txt");

    std::vector<ActionRequest> p1_win_multitank_p1 = {
        ActionRequest::Shoot};
    std::vector<ActionRequest> p1_win_multitank_p2 = {};
    tests.emplace_back("p1_win_multitank_test1", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{p1_win_multitank_p1, p1_win_multitank_p2}, "p1_win_multitank_test1_output.txt");

    std::vector<ActionRequest> p1_win_multitank = {
        ActionRequest::MoveForward,
    };
    tests.emplace_back("p1_win_multitank_test2", "TestScenes/test_scene11.txt", TestPlayerFactory{}, TestAlgorithmFactory{p1_win_multitank}, "p1_win_multitank_test2_output.txt");

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
    tests.emplace_back("multiple_objects_collision_test", "TestScenes/test_scene12.txt", TestPlayerFactory{}, TestAlgorithmFactory{multiple_objects_collision_p1, multiple_objects_collision_p2}, "multiple_objects_collision_test_output.txt");

    return tests;
};

std::vector<GameRulesTester> getSatelliteViewTests(){
    std::vector<GameRulesTester> tests;

    return tests;

}
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
    }
    std::cout << "Passed " << passed_tests << "/" << tests.size() << " tests\n"
              << std::endl;
    return 0;
}

void clearConsole()
{
    std::cout << "\033[3J\033[H\033[2J\033[H"; // Clear screen and move cursor to top-left
    std::cout << "\033c"; // THIS RESET THE BOARD - MAY CAUSE PROBLEMS
}

void printNotes()
{
    std::string notes = "PLEASE NOTE, NOT TESTING THE FOLLOWING REQUIREMENTS:\n"
                        "\tWhen failed reading the input file you should print an error message to the *terminal*\n"
                        "\tYou should write a short description of *all* recovered errors found in the input file, into input_errors.txt file\n";

    std::cout << notes;
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

    printNotes();
}
