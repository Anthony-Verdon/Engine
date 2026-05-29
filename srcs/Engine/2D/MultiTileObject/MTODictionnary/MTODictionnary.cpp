#include "Engine/2D/MultiTileObject/MTODictionnary/MTODictionnary.hpp"
#include <algorithm>

std::vector<MTO> MTODictionnary::MTOs;

size_t MTODictionnary::AddMTO(const MTO &mto)
{
    auto it = std::find(MTOs.begin(), MTOs.end(), mto);
    if (it == MTOs.end())
    {
        MTOs.push_back(mto);
        return (MTOs.size() - 1);
    }

    return (std::distance(MTOs.begin(), it));
}

MTO MTODictionnary::GetMTO(size_t index)
{
    if (index >= MTOs.size())
        return MTO::none;
    return (MTOs[index]);
}