#include "searchsettings.h"

SearchSettings::SearchSettings()
{

}

bool SearchSettings::matchesSearch(ComparisonOperation operation, int value, int comparisonValue)
{
    if (operation == SearchSettings::ComparisonOperation::equals)
        return value == comparisonValue;
    else if (operation == SearchSettings::ComparisonOperation::greater)
        return value > comparisonValue;
    else if (operation == SearchSettings::ComparisonOperation::gEqual)
        return value >= comparisonValue;
    else if (operation == SearchSettings::ComparisonOperation::less)
        return value < comparisonValue;
    else if (operation == SearchSettings::ComparisonOperation::lEqual)
        return value <= comparisonValue;

    return false;
}
