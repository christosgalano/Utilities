#include <iostream>
#include <string>
#include <limits>

/// A namespace with data and functions regarding the logging of coloured messages to ///
/// and the pausing and clearing of the console (linux terminal only) ...             ///

namespace ColouredLogger {
    enum Colour {
        BLACK    = 30,
        RED      = 31,
        GREEN    = 32,
        YELLOW   = 33,
        BLUE     = 34,
        MAGENTA  = 35,
        CYAN     = 36,
        WHITE    = 37
    };

    inline void LogMessageColoured(const std::string& message, Colour color) {
        std::string temp = std::to_string(color) + "m" + message;
        std::cout << "\033[1;" << temp << "\033[0m";
    }

    inline void Pause() {
        std::cout << "Press enter to continue...";
        std::cin.get();
    }

    inline void PauseAndClear() {
        std::cout << "Press enter to continue...";
        std::cin.get();
        system("clear");        
    }

    inline void PauseWithMessage(const std::string& message) {
        std::cout << message;
        std::cin.get(); 
    }

    inline void PauseAndClearWithMessage(const std::string& message) {
        std::cout << message;
        std::cin.get();
        system("clear"); 
    }

    inline void IgnoreLine() {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}
