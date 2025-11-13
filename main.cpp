#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include<sstream>


int main() 
{

    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;



    //std::cout << "$ ";


    //Сохранение пути в переменную для истории
    const char* home=std::getenv("HOME");
    std::string historyPath=std::string(home)+"/.kubsh_history";



    std::string input;


    while (std::getline(std::cin, input))
    {
        //Запись в историю
        if(!input.empty())
        {
	        std::ofstream history(historyPath,std::ios::app);//Открываем в режиме добавления
	        history << input<<"\n";
 	    }


        //history
        if(input=="history")
        {
            //Читаем из файла который в historyPath пока не закончатся строки
            std::ifstream historyOutput(historyPath);
                std::string line;
                while(std::getline(historyOutput,line))
                {
                    std::cout<<line<<"\n";
                }
        }


        //  \q
        else if (input == "\\q")
        {
            break;
        }


        //  echo
        //Если начинаем debug '
        // заканчиваем ', считываем с открытия апострофа до закрытия
        else if (input.substr(0, 7) == "debug '" && input[input.length() - 1] == '\'')
        {

            std::cout << input.substr(7, input.length() - 8) << std::endl;    

        }



        //   \e $
        else if (input.substr(0,4) == "\\e $")
        {
            std::string varName = input.substr(4);
            const char* value = std::getenv(varName.c_str());//Преобразуем C-строку в C++ строку

            if(value != nullptr)
            {
                std::string valueStr = value;
                
                bool has_colon = false;//Флаг для проверки наличия двоеточий
                for (char c : valueStr)//Проходим по символам из строки
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
                            std::cout << current_part << "\n";//Когда встречаем двоеточие - выводим накопленную часть
                            current_part = "";//Сбрасываем временную строку для следующей части
                        }
                        else 
                        {
                            current_part += c;//Иначе добавляем символ к строке

                        }
                    }
                    std::cout << current_part << "\n";//Выводим последнюю часть (после последнего двоеточия)
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

    else 
    {
        pid_t pid = fork();
        
        if (pid == 0) 
        {
            // Создаем копии строк для аргументов
            std::vector<std::string> tokens;
            std::vector<char*> args;
            std::string token;
            std::istringstream iss(input);
            
            while (iss >> token) 
            {
                tokens.push_back(token);  // сохраняем копии
            }
            
            // Преобразуем в char*
            for (auto& t : tokens) 
            {
                args.push_back(const_cast<char*>(t.c_str()));
            }
            args.push_back(nullptr);
            
            std::cout << "DEBUG: Executing: ";
            for (int i = 0; args[i] != nullptr; i++) 
            {
                std::cout << "[" << args[i] << "] ";
            }
            std::cout << "\n";
            
            execvp(args[0], args.data());
            
            std::cout << args[0] << ": command not found\n";
            exit(1);
            
        } 
        else if (pid > 0) 
        {
            int status;
            waitpid(pid, &status, 0);
        } 
        else 
        {
            std::cerr << "Failed to create process\n";
        }
    }

        //else std::cout<<input<<": command not found\n";


        //std::cout<<"$ ";

    }

}