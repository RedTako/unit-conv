#include "Units.hpp"
#include <regex>
#include <stdexcept>
#include <string>

double add(double a, double b) { return a + b; }
double sub(double a, double b) { return a - b; }
double mul(double a ,double b) { return a * b; }
double div(double a ,double b) { return a / b; }

UnitBase::UnitBase(bool b): is_primary(b) { }
UnitBase::~UnitBase() {}

PrimaryUnit::PrimaryUnit(): UnitBase(true) {}
PrimaryUnit::~PrimaryUnit() {}

SecondaryUnit::SecondaryUnit(): UnitBase(false) {}
SecondaryUnit::SecondaryUnit(std::vector<BakedOperation> ops): UnitBase(false), conversion_data(ops) {}
SecondaryUnit::~SecondaryUnit() {}

double PrimaryUnit::convert_to(const UnitBase& target, double value) const
{
    if(target.is_primary) return value;
    else
    {
        const auto& target_unit = static_cast<const SecondaryUnit&>(target);
        // for(BakedOperation o : target_unit.conversion_data)
        // {
        //     auto inverse_op = Operation{ o.op.get_inverse_operation() };
        //     value = inverse_op.calc(value, o.parameter);
        //     // res += o.op.calc(value, o.parameter);
        // }
        for(int i = target_unit.conversion_data.size() - 1; i >= 0; i--)
        {
            auto& baked_op = target_unit.conversion_data[i];
            auto inverse_op = Operation{ baked_op.op.get_inverse_operation() };
            value = inverse_op.calc(value, baked_op.parameter);
        }
        return value;
    }

    return 0;
}

double SecondaryUnit::convert_to(const UnitBase& target, double value) const
{
    if(target.is_primary)
    {
        for(BakedOperation o : conversion_data)
        {
            value = o.op.calc(value, o.parameter);
        }
        return value;
    }    
    else
    {
        PrimaryUnit p;
        auto primary_value = convert_to(p, value);
        auto res = p.convert_to(target, primary_value);
        return res;
    }

    return 0;
}

bool is_empty_or_whitespace(const std::string& s)
{
    bool empty = true;
    for(char c : s)
    {
        if(c != ' ')
            empty = false;
    }
    return empty;
}

CategoryMap read_units_file(std::istream& input)
{
    using namespace std::string_literals;
    std::string current_category_name = "";
    CategoryMap map;
    while (!input.eof())
    {
        std::string line;
        std::getline(input, line);

        if(line[0] == '#') //comment line
            continue;
        else if(line[0] == '[') //category delim
        {
            std::string category_name = "";
            int index = 1;
            while (line[index] != ']')
            {
                category_name += line[index];
                index++;
            }
            current_category_name = category_name;
            if(map.count(current_category_name) == 0)
            {
                map.emplace(current_category_name, UnitMap{});
            }
            
        }
        else
        {
            if(is_empty_or_whitespace(line)) continue;

            constexpr auto regex_pattern = "^"
                                           "(?:\\s)*"
                                           "(\\w+)"
                                           "(?:\\s)*"
                                           "="
                                           "(?:\\s)*"
                                           "([x0-9\\+\\-\\*\\/\\.]+)"
                                           "(?:\\s)*"
                                           "$";


            std::regex r(regex_pattern);
            std::smatch match_result;
            
            bool has_match = std::regex_match(line, match_result, r);
            if(!has_match) throw std::runtime_error("no match found on string "s + line);
            else if(match_result.size() < 3) throw std::runtime_error("insufficient match captures");
            
            auto unit_name = match_result[1].str();
            auto unit_data = match_result[2].str();

            std::unique_ptr<UnitBase> unit_ptr = nullptr;
            if(unit_data == "x") //is primary unit
            {
                unit_ptr.reset(new PrimaryUnit());
            }
            else
            {
                std::vector<BakedOperation> operations;
                
                std::regex r(R"([\-\+\*\/][0-9\.]+)");
                std::smatch match_result;
                
                std::string buffer = unit_data;
                while(std::regex_search(buffer, match_result, r))
                {

                // bool has_match = std::regex_match(unit_data, match_result, r);
                // if(!has_match) throw std::runtime_error("no match found on string "s + unit_data);
                // for(int i = 1; i < match_result.size(); i++)
                // {
                    auto sub_str = match_result.str();
                    printf("%s\n", sub_str.c_str());
                    char op = sub_str[0];
                    auto value_str = sub_str.substr(1);
                    auto value = std::stod(value_str);

                    BakedOperation operation;
                    operation.op = [&]() -> Operation {
                        switch(op)
                        {
                            case '+': return { Operation::OpType::Add };
                            case '-': return { Operation::OpType::Subtract };
                            case '*': return { Operation::OpType::Multiply };
                            case '/': return { Operation::OpType::Divide };
                            default: throw std::runtime_error("unknown operator error: "s + op);
                        }
                    }();
                    operation.parameter = value;
                    operations.push_back(operation);
                    buffer = match_result.suffix();
                }
                if(operations.size() == 0) throw std::runtime_error("no operations found error"s);
                unit_ptr.reset(new SecondaryUnit(operations));
            }
            
            map[current_category_name].emplace(unit_name, std::move(unit_ptr));
        }
        
    }
    
    // throw "not done yet";
    return map;
}