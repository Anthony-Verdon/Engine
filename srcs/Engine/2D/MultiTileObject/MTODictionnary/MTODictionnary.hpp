#pragma once

#include "Engine/2D/MultiTileObject/MTO.hpp"
#include <vector>

class MTODictionnary
{
  private:
    static std::vector<MTO> MTOs;

    MTODictionnary() = delete;
    ~MTODictionnary() = delete;

  public:
    static size_t AddMTO(const MTO &mto);
    static MTO GetMTO(size_t index);
    static void UpdateMTO(size_t index, const MTO &mto);
};