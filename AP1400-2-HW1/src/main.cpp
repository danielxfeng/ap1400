
#include <iostream>
#include <gtest/gtest.h>

using namespace std;

int main(int argc, char **argv)
{
    if (false) // make false to run unit-tests
    {
        cout << "This is a library." << endl;
    }
    else
    {
        ::testing::InitGoogleTest(&argc, argv);
        std::cout << "RUNNING TESTS ..." << std::endl;
        int ret{RUN_ALL_TESTS()};
        if (!ret)
            std::cout << "<<<SUCCESS>>>" << std::endl;
        else
            std::cout << "FAILED" << std::endl;
    }
    return 0;
}