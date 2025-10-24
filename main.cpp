
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

    else if (input.substr(0, 7) == "debug '" && input[input.length() - 1] == '\''){

        std::cout << input.substr(7, input.length() - 8) << std::endl;    
    
    }

    else if (input.substr(0,4) == "\\e $")

    {

        std::string varName = input.substr(4);

        const char* value = std::getenv(varName.c_str());



    if(value != nullptr)

        {

            std::string valueStr = value;//Преобразуем C-строку в C++ строку



            bool has_colon = false;//Флаг для проверки наличия двоеточий

            for (char c : valueStr)

            {

                if (c == ':') 

                {

                    has_colon = true;

                    break;

                }

            }



            if (has_colon) 

            {

                std::string current_part = "";//Временная строка для накопления текущей части пути

                for (char c : valueStr)//Разбиваем строку по двоеточиям

                {

                    if (c == ':') 

                    {


                        std::cout<< current_part << "\n";//Когда встречаем двоеточие - выводим накопленную часть

                        current_part = "";//Сбрасываем временную строку для следующей части

                    }

                    else 

                    {

                        current_part += c;//Иначе добавляем символ к строке

                    }

                }


                std::cout<< current_part << "\n";//Выводим последнюю часть (после последнего двоеточия)


            }

            else 

            { 

                std::cout << valueStr << "\n";//Если двоеточий нет - просто выводим значение как есть

            }

        }

        else

        {

            std::cout << varName << ": не найдено\n";

        }

    }

    else std::cout<<input<<": command not found\n";

    std::cout << "$ ";

    }

}