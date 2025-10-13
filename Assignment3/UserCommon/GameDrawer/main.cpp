#include "InputOutputSimulator.h"

int main(int argc, char **argv)
{
    if (argc!= 4){
        std::cout << "required: input, output, drawingType(int)" << std::endl;
        return EXIT_SUCCESS;
    }
    std::string input_file = argv[1];
    std::string output_file = argv[2];

    DrawingType dt  = DrawingTypes::fromInt(std::stoi(argv[3]));

    InputOutputSimulator(input_file, output_file, dt);
}
