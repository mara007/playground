#include <iostream>

#include "IniReader.h"

int main ()
{

    std::cout << "Hello World\n";

    IniReader ini_reader;
    ini_reader.parse ("my_ini.ini");

    std::string section, param, value;

    section = "SectionA";
    param   = "param_a";
    ini_reader.get_value (section, param, value);
    std::cout << "[" << section << "]:" << param << " = " << value << std::endl;

    section = "SectionA";
    param   = "param_b";
    ini_reader.get_value (section, param, value);
    std::cout << "[" << section << "]:" << param << " = " << value << std::endl;

    section = "SectionB";
    param   = "param_a";
    ini_reader.get_value (section, param, value);
    std::cout << "[" << section << "]:" << param << " = " << value << std::endl;

    return 0;
}
