#include "Units.hpp"
#include <iostream>
#include <string>

std::string get_op_name(Operation::OpType op)
{
    using namespace std::string_literals;
    switch (op)
    {
        case Operation::OpType::Add: return "Add"s;
        case Operation::OpType::Subtract: return "Subtract"s;
        case Operation::OpType::Multiply: return "Multiply"s;
        case Operation::OpType::Divide: return "Divide"s;
        default: throw "unknown";
    }
}

#define println(line) std::cout << line << '\n'

int main(int argc, char const *argv[])
{
    std::ifstream unit_file(UNIT_FILE, std::ios::in);
    auto map = read_units_file(unit_file);

    for(auto& entry : map)
    {
        std::cout << entry.first << '\n';
        println("-------------------------------------");
        for(auto& unit : entry.second)
        {
            std::string unit_str = unit.first;
            auto unit_secondary = dynamic_cast<SecondaryUnit*>(unit.second.get());
            if(unit_secondary != nullptr)
            {
                unit_str += "{ ";
                for(auto& data : unit_secondary->conversion_data)
                {
                    unit_str += get_op_name(data.op.op_type);
                    unit_str += " | ";
                    unit_str += std::to_string(data.parameter);
                    unit_str += ",";
                }
                unit_str += " }";
            }
            println(unit_str);
            
        }
        println("");
    }
    
    println("");

    

    return 0;
}
