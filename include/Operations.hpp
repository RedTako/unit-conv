#pragma once
#include <stdexcept>

struct Operation
{
    enum class OpType { Add, Subtract, Multiply, Divide };
    OpType op_type;
    double calc(double a, double b)
    {
        switch (op_type)
        {
            case OpType::Add:           return a + b;
            case OpType::Subtract:      return a - b;
            case OpType::Multiply:      return a * b;
            case OpType::Divide:        return a / b;
        }
        throw std::runtime_error("operation type error");
    }

    OpType get_inverse_operation() const 
    {
        switch(op_type)
        {
            case OpType::Add:           return OpType::Subtract;
            case OpType::Subtract:      return OpType::Add;

            case OpType::Multiply:      return OpType::Divide;
            case OpType::Divide:        return OpType::Multiply;
        }
        throw std::runtime_error("operation invert error");
    }
};

struct BakedOperation
{
    Operation op;
    double parameter;
};