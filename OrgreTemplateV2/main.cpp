#include "PaddleGame.h"

#include <iostream>

int main(int argc, char** argv)
{
    try
    {
        PaddleGame app;
        app.initApp();
        app.getRoot()->startRendering();
        app.clear();
        app.closeApp();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred during execution: " << e.what() << '\n';
        return 1;
    }

    return 0;
}