#include "Engine/2D/TilemapManager/TilemapManager.hpp"
#include <fstream>
#include <filesystem>
#include <set>
#include "Engine/RessourceManager/RessourceManager.hpp"
#include "Engine/2D/TileDictionnary/TileDictionnary.hpp"
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

void TilemapManager::Load()
{
    if (!std::filesystem::exists("saves/map.json")) // @todo: should be a parameter
        return;

    Json::Node file = Json::ParseFile("saves/map.json");

    if (file.KeyExist("textures") && file["textures"] != nullptr)
    {
        for (auto it : file["textures"])
        {
            RessourceManager::AddTexture(it["name"], it["path"]);
        }
    }

    if (file.KeyExist("tiles") && file["tiles"] != nullptr)
    {
        for (auto it : file["tiles"])
        {
            Tile tile;
            tile.sprite.textureName = std::string(it["sprite"]["texture"]["name"]);
            tile.sprite.textureSize = ml::vec2(it["sprite"]["texture"]["size"][0], it["sprite"]["texture"]["size"][1]);
            tile.sprite.spriteCoords = ml::vec2(it["sprite"]["position"][0], it["sprite"]["position"][1]);
            tile.sprite.size = ml::vec2(it["sprite"]["size"][0], it["sprite"]["size"][1]);
            tile.spriteOffset = ml::vec2(it["sprite"]["offset"][0], it["sprite"]["offset"][1]);
            if (it.KeyExist("behaviors") && it["behaviors"] != nullptr)
            {
                for (auto itBehavior : it["behaviors"])
                    tile.behaviors.push_back((TileBehaviorType)(int)itBehavior);
            }
            TileDictionnary::AddTile(tile);
        }
    }

    if (file.KeyExist("tilemaps") && file["tilemaps"] != nullptr)
    {
        Json::Node tilemapsNode = file["tilemaps"];
        for (auto itTilemap = tilemapsNode.begin(); itTilemap != tilemapsNode.end(); ++itTilemap)
        {
            std::string tilemapName = itTilemap.key();

            tilemaps[tilemapName] = Tilemap();
            tilemapOrder.push_back(tilemapName);

            Json::Node value = itTilemap.value();
            tilemaps[tilemapName].SetBuildCollision(value["build collision"]);

            for (auto it : value["tiles"])
            {
                tilemaps[tilemapName].AddTile(ml::vec2(it["position"][0], it["position"][1]), it["index"]);
            }
        }
    }
}

void TilemapManager::Save()
{
    Json::Node file;

    file["textures"] = {};
    std::set<std::string> textures;

    file["tiles"] = {};
    for (size_t i = 0; i < TileDictionnary::GetDictionnarySize(); i++)
    {
        Tile tile = TileDictionnary::GetTile(i);
        file["tiles"][i]["sprite"]["texture"]["name"] = tile.sprite.textureName;
        file["tiles"][i]["sprite"]["texture"]["size"][0] = tile.sprite.textureSize.x;
        file["tiles"][i]["sprite"]["texture"]["size"][1] = tile.sprite.textureSize.y;
        file["tiles"][i]["sprite"]["position"][0] = tile.sprite.spriteCoords.x;
        file["tiles"][i]["sprite"]["position"][1] = tile.sprite.spriteCoords.y;
        file["tiles"][i]["sprite"]["size"][0] = tile.sprite.size.x;
        file["tiles"][i]["sprite"]["size"][1] = tile.sprite.size.y;
        file["tiles"][i]["sprite"]["offset"][0] = tile.spriteOffset.x;
        file["tiles"][i]["sprite"]["offset"][1] = tile.spriteOffset.y;
        file["tiles"][i]["behaviors"] = {};
        for (size_t j = 0; j < tile.behaviors.size(); j++)
            file["tiles"][i]["behaviors"][j] = (int)tile.behaviors[j];

        textures.insert(tile.sprite.textureName);
    }

    file["tilemaps"] = {};
    for (size_t i = 0; i < tilemapOrder.size(); i++)
    {
        file["tilemaps"][tilemapOrder[i].c_str()]["tiles"] = {};
        file["tilemaps"][tilemapOrder[i].c_str()]["build collision"] = tilemaps[tilemapOrder[i]].GetBuildCollision();

        std::map<ml::vec2, size_t, Vec2Comparator> tiles = tilemaps[tilemapOrder[i]].GetTiles();
        int j = 0;
        for (auto it = tiles.begin(); it != tiles.end(); it++)
        {
            file["tilemaps"][tilemapOrder[i].c_str()]["tiles"][j]["position"][0] = it->first.x;
            file["tilemaps"][tilemapOrder[i].c_str()]["tiles"][j]["position"][1] = it->first.y;
            file["tilemaps"][tilemapOrder[i].c_str()]["tiles"][j]["index"] = (int)it->second;

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

    Json::WriteFile("saves/map.json", file);
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