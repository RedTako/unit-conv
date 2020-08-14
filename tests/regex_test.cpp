#include <iostream>
#include <regex>
#include <string>
#include <cassert>

int main(int argc, char const *argv[])
{
    constexpr auto regex_pattern = "^"
                                    "(?:\\s)*"
                                    "(\\w+)"
                                    "(?:\\s)*"
                                    "="
                                    "(?:\\s)*"
                                    "([x0-9\\+\\-\\*\\/]+)"
                                    "(?:\\s)*"
                                    "$";

    constexpr auto raw_regex_pattern = R"(^(?:\s)*(\w+)(?:\s)*=(?:\s)*([x0-9\+\-\*\/]+)(?:\s)*$)";


    std::regex r(regex_pattern);
    std::string test_str = "test  = x+2";

    std::smatch results;
    std::regex_match(test_str, results, r);

    auto prefix = results[1];
    auto suffix = results[2];

    assert(prefix.str() == "test");
    assert(suffix.str() == "x+2");


    constexpr auto equation = "x-32/1.8";
    std::regex eq_reg(R"([\-\+\*\/][0-9\.]+)");
    std::smatch eq_match;
    std::string eq_str = equation;
    while(std::regex_search(eq_str, eq_match, eq_reg))
    {
        std::cout << eq_match.str() << '\n';
        eq_str = eq_match.suffix();
    }
    return 0;
}
