#include "Engine/TilemapManager/TilemapManager.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <filesystem>
#include <set>
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/TileDictionnary/TileDictionnary.hpp"

std::map<std::string, Tilemap> TilemapManager::tilemaps;
std::vector<std::string> TilemapManager::tilemapOrder;

void TilemapManager::AddTile(const std::string &tilemapName, const glm::vec2 &position, const Tile &tile)
{
    auto it = tilemaps.find(tilemapName);
    if (it != tilemaps.end())
    {
        size_t index = TileDictionnary::AddTile(tile);
        it->second.AddTile(position, index);
    }
}

void TilemapManager::AddTile(const std::string &tilemapName, const glm::vec2 &position, const Sprite &sprite, const glm::vec2 &spriteOffset)
{
    Tile newTile;
    newTile.sprite = sprite;
    newTile.spriteOffset = spriteOffset;
    AddTile(tilemapName, position, newTile);
}

void TilemapManager::SuppressTile(const std::string &tilemapName, const glm::vec2 &position)
{
    auto it = tilemaps.find(tilemapName);
    if (it == tilemaps.end())
        return;

    it->second.SuppressTile(position);
}

bool TilemapManager::TileExist(const std::string &tilemapName, const glm::vec2 &position)
{
    auto it = tilemaps.find(tilemapName);
    if (it == tilemaps.end())
        return (false);

    return (it->second.TileExist(position));
}

Tile TilemapManager::GetTile(const std::string &tilemapName, const glm::vec2 &position)
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

void TilemapManager::Load()
{
    if (!std::filesystem::exists("saves/map.json")) // @todo: should be a parameter
        return;

    std::ifstream input("saves/map.json");
    nlohmann::ordered_json file =  nlohmann::ordered_json::parse(input);

    auto itTextures = file.find("textures"); //@todo error check
    for (auto it : *itTextures)
    {
        RessourceManager::AddTexture(it["name"], it["path"]);
    }

    auto itTiles = file.find("tiles"); //@todo error check
    for (auto it : *itTiles)
    {
        Tile tile;
        tile.sprite.textureName = it["sprite"]["texture"]["name"];
        tile.sprite.textureSize = glm::vec2(it["sprite"]["texture"]["size"][0], it["sprite"]["texture"]["size"][1]);
        tile.sprite.spriteCoords = glm::vec2(it["sprite"]["position"][0], it["sprite"]["position"][1]);
        tile.sprite.size = glm::vec2(it["sprite"]["size"][0], it["sprite"]["size"][1]);
        tile.spriteOffset = glm::vec2(it["sprite"]["offset"][0], it["sprite"]["offset"][1]);
        auto itBehaviors = it.find("behaviors");
        for (auto itBehavior : *itBehaviors)
            tile.behaviors.push_back(itBehavior);
        TileDictionnary::AddTile(tile);
    }

    auto itTilemaps = file.find("tilemaps"); //@todo error check
    for (auto itTilemap = itTilemaps->begin(); itTilemap != itTilemaps->end(); ++itTilemap)
    {
        std::string tilemapName = itTilemap.key();

        tilemaps[tilemapName] = Tilemap();
        tilemapOrder.push_back(tilemapName);

        tilemaps[tilemapName].SetBuildCollision((*itTilemap)["build collision"]);

        auto itTiles = itTilemap->find("tiles");
        for (auto it : *itTiles)
        {
            tilemaps[tilemapName].AddTile(glm::vec2(it["position"][0], it["position"][1]), it["index"]);
        }
    }
}

void TilemapManager::Save()
{
    nlohmann::ordered_json file;

    file["textures"] = {};
    std::set<std::string> textures;

    file["tiles"] = {};
    for (size_t i = 0; i < TileDictionnary::GetDictionnarySize(); i++)
    {
        Tile tile = TileDictionnary::GetTile(i);
        file["tiles"][i]["sprite"]["texture"]["name"] = tile.sprite.textureName;
        file["tiles"][i]["sprite"]["texture"]["size"] = {tile.sprite.textureSize.x, tile.sprite.textureSize.y};
        file["tiles"][i]["sprite"]["position"] = {tile.sprite.spriteCoords.x, tile.sprite.spriteCoords.y};
        file["tiles"][i]["sprite"]["size"] = {tile.sprite.size.x, tile.sprite.size.y};
        file["tiles"][i]["sprite"]["offset"] = {tile.spriteOffset.x, tile.spriteOffset.y};
        file["tiles"][i]["behaviors"] = {};
        for (size_t j = 0; j < tile.behaviors.size(); j++)
            file["tiles"][i]["behaviors"][j] = tile.behaviors[j];

        
        textures.insert(tile.sprite.textureName);
    }

    file["tilemaps"] = {};
    for (size_t i = 0; i < tilemapOrder.size(); i++ )
    {
        file["tilemaps"][tilemapOrder[i]]["tiles"] = {};
        file["tilemaps"][tilemapOrder[i]]["build collision"] = tilemaps[tilemapOrder[i]].GetBuildCollision();
        
        std::map<glm::vec2, size_t, Vec2Comparator> tiles = tilemaps[tilemapOrder[i]].GetTiles();
        int j = 0;
        for (auto it = tiles.begin(); it != tiles.end(); it++)
        {
            file["tilemaps"][tilemapOrder[i]]["tiles"][j]["position"] = {it->first.x, it->first.y};
            file["tilemaps"][tilemapOrder[i]]["tiles"][j]["index"] = it->second;

            j++;
        }
    }

    int i = 0;
    for (auto it = textures.begin(); it != textures.end(); it++)
    {
        file["textures"][i]["name"] = *it;
        file["textures"][i]["path"] = RessourceManager::GetTexture(*it)->getPath();

        i++;
    }
    std::ofstream o("saves/map.json");
    o << std::setw(4) << file << std::endl;
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