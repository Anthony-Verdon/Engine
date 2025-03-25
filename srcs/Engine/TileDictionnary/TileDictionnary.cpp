#include "Engine/TileDictionnary/TileDictionnary.hpp"
#include <algorithm>

std::vector<Tile> TileDictionnary::tiles;

size_t TileDictionnary::AddTile(const Tile &tile)
{
    auto it = std::find(tiles.begin(), tiles.end(), tile);
    if (it == tiles.end())
    {
        tiles.push_back(tile);
        return (tiles.size() - 1);
    }

    return (std::distance(tiles.begin(), it));
}

void TileDictionnary::AddTileBehavior(size_t index, TileBehaviorType behaviorType)
{
    if (index >= tiles.size())
        return;

    tiles[index].behaviors.push_back(behaviorType);
}

void TileDictionnary::RemoveTileBehavior(size_t index, TileBehaviorType behaviorType)
{
    if (index >= tiles.size())
        return;

    auto it = std::find(tiles[index].behaviors.begin(), tiles[index].behaviors.end(), behaviorType);
    if (it != tiles[index].behaviors.end())
        tiles[index].behaviors.erase(it);
}

Tile TileDictionnary::GetTile(size_t index)
{
    if (index >= tiles.size())
        return Tile::none;
    return (tiles[index]);
}

size_t TileDictionnary::GetTileIndex(const Tile &tile)
{
    return (std::distance(tiles.begin(), std::find(tiles.begin(), tiles.end(), tile)));
}

size_t TileDictionnary::GetDictionnarySize()
{
    return (tiles.size());
}