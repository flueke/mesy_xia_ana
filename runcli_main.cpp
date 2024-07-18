#include "my_experiment.h"

#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    auto result = process_one_listfile(argv[1]);

    return result ? 0 : 1;
}
