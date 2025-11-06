#include "Application.hpp"
#include  "Window.hpp"
#include <stdexcept>
#include <iostream>
int main(){

    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
    return 0;
}
