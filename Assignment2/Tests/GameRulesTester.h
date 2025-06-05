#pragma once
#include "TestPlayer.h"
#include "TestAlgorithm.h"
#include "GameManager.h" // CHANGE THIS TO YOUR MANAGER CLASS HEADER

#include <string>
#include <filesystem>
#include <fstream>

class GameRulesTester
{
private:
    // === Attributes === //
    std::string title;
    std::string input_file;

    TestPlayerFactory player_factory;
    TestAlgorithmFactory algo_factory;

    std::string expected_output_file;

    bool expect_read_board_fail;

    // === Files Getters === //
    std::string getManagerOutputFile() const
    {
        // Our Logger is built to allow only one output file per run - therefore, using static to return always the first output file created in the tests
        static const std::string input_file_name = std::filesystem::path(input_file).stem().string(); // this is a standard library function the return the name of a file
        return "output_" + input_file_name + ".txt";
    }

    std::string getExpectedOutputFile() const
    {
        return "ExpectedOutputs/" + expected_output_file;
    }

    std::string getSavedOutputFile() const
    {
        return "RecievedOutputs/" + expected_output_file;
    }
    
    std::string getPlayerOutputFile() const
    {
        return "GetBattleInfoOutputs/get_battle_info_log.txt";
    }

    std::string getPlayerTestOutputFile() const
    {
        return "GetBattleInfoOutputs/get_battle_info_expected_log.txt"; // empty_file
    }
    
    // === Clear Files === //
    void clearPlayerOutputFile() const
    {
        std::ofstream o_file(getPlayerOutputFile(), std::ios::trunc);
    }

    void clearManagerOutputFile() const
    {
        std::ofstream ofs(getManagerOutputFile(), std::ios::trunc);
    }

    void clearInputErrorsFile() const
    {
        std::ofstream ofs("input_errors.txt", std::ios::trunc);
    }

    // === Save Manager Output to new File === //
    void saveOutput() const
    {
        std::ifstream manager_output_file(getManagerOutputFile());
        std::ofstream saved_output_file(getSavedOutputFile(), std::ios::trunc);

        std::string line;
        while (std::getline(manager_output_file, line))
        {
            saved_output_file << line << '\n';
        }
    }

    // === Compare Files === //
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

    // === Redirect std::cout === //
    std::streambuf * redirectCout(std::streambuf* new_buf) const // return original streambuf
    {
        std::streambuf *original_buf = std::cout.rdbuf();
        std::cout.rdbuf(new_buf);
        return original_buf;
    }


    // === Test === //
    bool runTest() const
    {
        // run test
        GameManager game(this -> player_factory, this->algo_factory);
        if (!game.readBoard(this->input_file))
            return expect_read_board_fail;
        game.run();

        // check test results
        bool correct_manager_output = areFilesIdentical(getManagerOutputFile(), getExpectedOutputFile());
        bool correct_get_battle_info = areFilesIdentical(getPlayerOutputFile(), getPlayerTestOutputFile());
        return correct_manager_output && correct_get_battle_info;
    }

public:
    // === Constructor === //
    GameRulesTester(const std::string &title, const std::string &input_file,
                    const TestPlayerFactory &player_factory, const TestAlgorithmFactory &algo_factory,
                    const std::string &expected_output_file,
                    bool expect_read_board_fail = false)
        : title(title), input_file(input_file),
          player_factory(player_factory), algo_factory(algo_factory),
          expected_output_file(expected_output_file),
          expect_read_board_fail(expect_read_board_fail) {};

    // === Public Evaluation Function === //
    bool evalTest() const
    {
        // redirect cout to null until end of test
        std::ofstream null_file("/dev/null");
        std::streambuf *original_cout_buf = redirectCout(null_file.rdbuf());

        // test
        bool success = runTest();

        // redirect cout back to original file
        redirectCout(original_cout_buf);

        std::cout << this->title << (success ? (": PASSED") : (": FAILED")) << std::endl;
        if (!success)
        {
            saveOutput(); // save the output to a new file
            std::cout << "see expected output at '" << getExpectedOutputFile() << "' vs recieved at '" + this->getSavedOutputFile() + "'" << std::endl;
        }
        clearManagerOutputFile();
        clearPlayerOutputFile();
        return success;
    }
};
