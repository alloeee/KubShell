
#include <iostream>

#include <string>

#include <fstream>


int main() {



    std::cout << std::unitbuf;

    std::cerr << std::unitbuf;

    std::cout<<"$ ";

    const char* home = std::getenv("HOME");
    std::string historyPath = std::string(home) + "/.kubsh_history";

    std::string input;
    while (std::getline(std::cin, input)) {
        std::cout << input << "\n";
    
        if(!input.empty()){

	    std::ofstream history(historyPath,std::ios::app);

	    history << input<<"\n";
 	}



	if(input=="history")
	{
	    std::ifstream historyOutput(historyPath);
        std::string line;

        while(std::getline(historyOutput,line))

        {
            std::cout<<line<<"\n";
        }

    }

    else if (input == "\\q") {
        break;
    }
    std::cout << "$ ";
    }
}