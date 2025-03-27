#include "Engine/2D/TilemapManager/TilemapManager.hpp"
#include <fstream>
#include <filesystem>
#include <set>
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/2D/TileDictionnary/TileDictionnary.hpp"

std::map<std::string, Tilemap> TilemapManager::tilemaps;
std::vector<std::string> TilemapManager::tilemapOrder;

void TilemapManager::AddTile(const std::string &tilemapName, const ml::vec2 &position, const Tile &tile)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
    {
        size_t index = TileDictionnary::AddTile(tile);
        it->second.AddTile(position, index);
    }
}

void TilemapManager::AddTile(const std::string &tilemapName, const ml::vec2 &position, const Sprite &sprite, const ml::vec2 &spriteOffset)
{
    Tile newTile;
    newTile.sprite = sprite;
    newTile.spriteOffset = spriteOffset;
    AddTile(tilemapName, position, newTile);
}

void TilemapManager::SuppressTile(const std::string &tilemapName, const ml::vec2 &position)
{
    auto it = tilemaps.find(tilemapName);
    if (it == tilemaps.end())
        return;

    it->second.SuppressTile(position);
}

bool TilemapManager::TileExist(const std::string &tilemapName, const ml::vec2 &position)
{
    auto it = tilemaps.find(tilemapName);
    if (it == tilemaps.end())
        return (false);

    return (it->second.TileExist(position));
}

Tile TilemapManager::GetTile(const std::string &tilemapName, const ml::vec2 &position)
{
    auto it = tilemaps.find(tilemapName);
    if (it == tilemaps.end())
        return (Tile::none);

    return (it->second.GetTile(position));
}

bool TilemapManager::GetBuildCollision(const std::string &tilemapName)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        return (it->second.GetBuildCollision());

    return (false);
}

void TilemapManager::SetBuildCollision(const std::string &tilemapName, bool buildCollision)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.SetBuildCollision(buildCollision);
}

void TilemapManager::AddTilemap(const std::string &name, const Tilemap &tilemap)
{
    tilemaps[name] = tilemap;
    tilemapOrder.push_back(name);
}

void TilemapManager::Draw()
{
    for (size_t i = 0; i < tilemapOrder.size(); i++)
        tilemaps[tilemapOrder[i]].Draw();
}

void TilemapManager::AddCollisions(b2WorldId worldId)
{
    for (size_t i = 0; i < tilemapOrder.size(); i++)
        tilemaps[tilemapOrder[i]].CreateCollision(worldId);
}

void TilemapManager::UpdateCollision(const std::string &tilemapName, b2WorldId worldId)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.UpdateCollision(worldId);
}