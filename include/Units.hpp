#pragma once
#include <fstream>
#include <vector>
#include <tuple>
#include <memory>
#include "Operations.hpp"
#include <unordered_map>
#include <string_view>

class UnitBase
{
    friend class PrimaryUnit;
    friend class SecondaryUnit;
protected:
    bool is_primary;
    UnitBase(bool b);

public:
    virtual ~UnitBase();
    virtual double convert_to(const UnitBase& target, double value) const = 0;
};

//all units are converted to or from the primary unit
class PrimaryUnit: public UnitBase
{
public:
    PrimaryUnit();
    virtual ~PrimaryUnit();
    double convert_to(const UnitBase& target, double value) const override;
};

//all units must be convertable to primary unit
class SecondaryUnit: public UnitBase
{
public:
    std::vector<BakedOperation> conversion_data;
    SecondaryUnit();
    SecondaryUnit(std::vector<BakedOperation> ops);
    virtual ~SecondaryUnit();
    double convert_to(const UnitBase& target, double value) const override;
};

typedef std::unordered_map<std::string, std::unique_ptr<UnitBase>> UnitMap;
typedef std::unordered_map<std::string, UnitMap> CategoryMap;
CategoryMap read_units_file(std::istream& input);