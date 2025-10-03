
#include <iostream>
#include "src/Application.h"

int main()
{
    Application application;
    try
    {
        application.run();
    }
    catch (std::runtime_error& runtime_error)
    {
        std::cout << runtime_error.what();
    }
    return 0;
}


