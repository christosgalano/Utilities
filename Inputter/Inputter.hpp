#include <iostream>
#include <string>
#include <limits>

/// A namespace with functions that prompt the user for input and then they ///
/// make sure that the input is correct ...                                 ///

namespace Inputter {
    inline std::string GetInput(const std::string& message, const std::string& check1, const std::string& check2) {
        std::string answer;
        do {
            std::cout << message;    
            std::getline(std::cin, answer);
            std::cout << '\n';
            if (answer != check1 && answer != check2)
                std::cout << "Wrong input (" << check1 << " / " << check2 << ")\n";
       
        } while (answer != check1 && answer != check2 && !std::cin.eof());
        return answer; 
    }

    inline std::string GetInput(const std::string& message, const std::string& check1,
                                const std::string& check2, const std::string& check3) 
    {
        std::string answer;
        do {
            std::cout << message;        
            std::getline(std::cin, answer);
            std::cout << '\n';
            if (answer != check1 && answer != check2 && answer != check3)
                std::cout << "Wrong input (" << check1 << " / " << check2 << " / " << check3 << ")\n";

        } while (answer != check1 && answer != check2 && answer != check3 && !std::cin.eof());
        return answer; 
    }

    inline std::string GetInput(const std::string& message, const std::string& check1, const std::string& check2, 
                                const std::string& check3, const std::string& check4) 
    {
        std::string answer;
        do {
            std::cout << message;    
            std::getline(std::cin, answer);
            std::cout << '\n';
            if (answer != check1 && answer != check2 && answer != check3 && answer != check4)
                std::cout << "Wrong input (" << check1 << " / " << check2 << " / " << check3 << " / " << check4 << ")\n";
         
        } while (answer != check1 && answer != check2 && answer != check3 && answer != check4 && !std::cin.eof());
        return answer; 
    }

    inline int GetInput(const std::string& message, int lower_bound, int upper_bound) {
        int choice;
        do {
            std::cout << message;    
            std::cin >> choice;
            std::cout << '\n';
            if (std::cin.fail()) {  // If a previous extraction failed
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Remove the bad input
            }

            if (choice < lower_bound || choice > upper_bound)
                std::cout << "Wrong input (" << lower_bound << " to " << upper_bound << ")\n";
        
        } while ((choice < lower_bound || choice > upper_bound) && !std::cin.eof());
        return choice; 
    }
}

// hello