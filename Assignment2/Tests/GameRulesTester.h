#pragma once
#include "TestPlayer.h"
#include "TestAlgorithm.h"
#include "../GameManager/GameManager.h"

#include <string>
#include <filesystem>
#include <fstream>

class GameRulesTester
{
private:
    std::string title;
    std::string input_file;

    TestPlayerFactory player_factory;
    TestAlgorithmFactory algo_factory;

    std::string expected_output_file;

    bool expect_read_board_fail;

    std::string getMatchingOutputFile() const
    {
        // Our Logger is built to allow only one output file per run - therefore, using static to return always the first output file created in the tests
        static const std::string input_file_name = std::filesystem::path(input_file).stem().string(); // this is a standard library function the return the name of a file
        return "output_" + input_file_name + ".txt";
    }

    bool areFilesIdentical(const std::string &file1, const std::string &file2) const
    {
        std::ifstream f1(file1, std::ios::binary | std::ios::ate);
        std::ifstream f2(file2, std::ios::binary | std::ios::ate);

        if (!f1 || !f2)
            return false; // failed to open one of the files

        if (f1.tellg() != f2.tellg())
            return false; // different size

        f1.seekg(0);
        f2.seekg(0);

        return std::equal(std::istreambuf_iterator<char>(f1),
                          std::istreambuf_iterator<char>(),
                          std::istreambuf_iterator<char>(f2));
    }

    void clearOutputFile() const
    {
        std::ofstream ofs(getMatchingOutputFile(), std::ios::trunc);
    }

    void clearInputErrorsFile() const
    {
        std::ofstream ofs("input_errors.txt", std::ios::trunc);
    }
   
    bool runTest() const
    {
        this->clearOutputFile();
        // this->clearInputErrorsFile();

        GameManager game(this->player_factory, this->algo_factory);
        if (!game.readBoard(this->input_file))
            return expect_read_board_fail;
        game.run();

        return areFilesIdentical(getMatchingOutputFile(), this->expected_output_file);
    }

public:
    GameRulesTester(const std::string &title, const std::string &input_file,
                    const TestPlayerFactory &player_factory, const TestAlgorithmFactory &algo_factory,
                    const std::string &expected_output_file, bool expect_read_board_fail = false)
        : title(title), input_file(input_file),
          player_factory(player_factory), algo_factory(algo_factory),
          expected_output_file(expected_output_file), expect_read_board_fail(expect_read_board_fail) {};

    bool evalTest() const
    {
        bool success = runTest();
        std::cout << this->title << (success ? (": PASSED") : (": FAILED")) << std::endl;
        if (!success)
        {
            std::cout << "see expected output at '" << this->expected_output_file << "'" << std::endl;
        }

        return success;
    }
};
