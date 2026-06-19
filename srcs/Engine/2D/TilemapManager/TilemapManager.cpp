#include "Engine/2D/TilemapManager/TilemapManager.hpp"
#include <fstream>
#include <filesystem>
#include <set>
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/2D/TileDictionnary/TileDictionnary.hpp"
#include "Engine/2D/MultiTileObject/MTODictionnary/MTODictionnary.hpp"
#include "Json/Json.hpp"

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

void TilemapManager::AddTile(const std::string &tilemapName, const ml::vec2 &position, size_t index)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.AddTile(position, index);
}

void TilemapManager::AddTile(const std::string &tilemapName, const ml::vec2 &position, const Sprite &sprite, const ml::vec2 &spriteOffset, const ml::vec2 &boundingBox)
{
    Tile newTile;
    newTile.sprite = sprite;
    newTile.spriteOffset = spriteOffset;
    newTile.boundingBox = boundingBox;
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

std::map<ml::vec2, size_t, Vec2Comparator> TilemapManager::GetTiles(const std::string &tilemapName)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        return (it->second.GetTiles());

    return (std::map<ml::vec2, size_t, Vec2Comparator>{});
}

int TilemapManager::GetTilemapIndex(const std::string &tilemapName)
{
    for (size_t i = 0; i < tilemapOrder.size(); i++)
    {
        if (tilemapOrder[i] == tilemapName)
            return ((int)i);
    }

    return (-1);
}

void TilemapManager::AddMTO(const std::string &tilemapName, const ml::vec2 &position, const MTO &mto)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
    {
        size_t index = MTODictionnary::AddMTO(mto);
        MTOInstance mtoInstance;
        mtoInstance.index = index;
        mtoInstance.position = position;
        for (size_t i = 0; i < mto.boundingBoxs.size(); i++)
        {
            mtoInstance.behaviors.push_back({});
            mtoInstance.behaviors[i].type = 0;
        }
        it->second.AddMTO(mtoInstance);
    }
}

void TilemapManager::AddMTO(const std::string &tilemapName, const MTOInstance &instance)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.AddMTO(instance);
}

void TilemapManager::UpdateMTO(const std::string &tilemapName, size_t index, const MTOInstance &instance)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.UpdateMTO(index, instance);
}

std::vector<MTOInstance> TilemapManager::GetMTOs(const std::string &tilemapName)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        return (it->second.GetMTOs());

    return (std::vector<MTOInstance>{});
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
        tilemaps[tilemapOrder[i]].Draw(i);
}

void TilemapManager::AddCollisions(b2WorldId worldId)
{
    for (auto it = tilemaps.begin(); it != tilemaps.end(); it++)
        it->second.CreateCollision(worldId);
}

void TilemapManager::UpdateCollision(const std::string &tilemapName, b2WorldId worldId)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
        it->second.UpdateCollision(worldId);
}