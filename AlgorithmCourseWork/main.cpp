#include "Controller.h"
#include <cstdlib>  

int main() 
{
    std::system("chcp 1251");

    Controller courseWork("data.txt");

    courseWork.PrintUsers();

    return 0;
}
