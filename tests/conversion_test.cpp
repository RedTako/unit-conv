#include <iostream>
#include <cstdlib>
#include <cassert>

#include "Units.hpp"

template<class U1, class U2>
auto convert_and_print(const U1& source, const U2& target, double value)
{
    auto res = source.convert_to(target, value);
    std::cout << res << '\n';
    return res;
}

void approx_assert(double value, double expected, double tolerance = 0.1)
{
    assert(value >= expected - tolerance && value <= expected + tolerance);
}

int main(int argc, char const *argv[])
{
    PrimaryUnit meters{};
    SecondaryUnit kilometers({ {Operation::OpType::Multiply, 1'000} }); //kilometers to meters
    SecondaryUnit centimeters({ {Operation::OpType::Divide, 100} }); //centimeters to meters
    SecondaryUnit inches({ {Operation::OpType::Divide, 39.37} });

    PrimaryUnit celcius{};
    SecondaryUnit fahrenheit({  //fahrenheit to celcius
        { Operation::OpType::Subtract, 32 },
        { Operation::OpType::Divide, 1.8 }
    });
    SecondaryUnit kelvin({ {Operation::OpType::Subtract, 273.15} });

    auto result = convert_and_print(kilometers, meters, 1);
    assert(result == 1000);

    result = convert_and_print(meters, kilometers, 1000);
    assert(result == 1);

    result = convert_and_print(centimeters, kilometers, 100'000);
    assert(result == 1);

    result = convert_and_print(kilometers, centimeters, 1);
    assert(result == 100'000);

    result = convert_and_print(fahrenheit, celcius, 10);
    approx_assert(result, -12.2, 0.1);

    result = convert_and_print(celcius, fahrenheit, 10);
    approx_assert(result, 50);

    result = convert_and_print(kelvin, fahrenheit, 100);
    approx_assert(result, -279.67);

    result = convert_and_print(inches, meters, 1);
    approx_assert(result, 0.0254);

    result = convert_and_print(meters, inches, 1);
    approx_assert(result, 39.37);
    return 0;
}
