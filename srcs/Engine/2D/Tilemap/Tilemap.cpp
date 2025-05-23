#include "Engine/2D/Tilemap/Tilemap.hpp"
#include "Engine/2D/TileDictionnary/TileDictionnary.hpp"
#include "Engine/2D/Renderers/SpriteRenderer/SpriteRenderer.hpp"
#include "Engine/2D/Renderers/LineRenderer2D/LineRenderer2D.hpp"
#include "Engine/2D/PhysicBody/PhysicBody.hpp"
#include "Engine/defines.hpp"
#include <array>
#include <map>
#include <vector>
#include <algorithm>

const std::array<ml::vec2, 4> directions {
    ml::vec2(0, -SPRITE_SIZE), // top
    ml::vec2(SPRITE_SIZE, 0),  // right
    ml::vec2(0, SPRITE_SIZE),  // bottom
    ml::vec2(-SPRITE_SIZE, 0)  // left
};

const std::array<std::pair<ml::vec2, ml::vec2>, 4> points {
    std::make_pair(ml::vec2(-SPRITE_SIZE, -SPRITE_SIZE) / 2.0f, ml::vec2(SPRITE_SIZE, -SPRITE_SIZE) / 2.0f),  // top
    std::make_pair(ml::vec2(SPRITE_SIZE, -SPRITE_SIZE) / 2.0f, ml::vec2(SPRITE_SIZE, SPRITE_SIZE) / 2.0f),    // right
    std::make_pair(ml::vec2(-SPRITE_SIZE, SPRITE_SIZE) / 2.0f, ml::vec2(SPRITE_SIZE, SPRITE_SIZE) / 2.0f),    // bottom
    std::make_pair(ml::vec2(-SPRITE_SIZE, -SPRITE_SIZE) / 2.0f, ml::vec2(-SPRITE_SIZE, SPRITE_SIZE) / 2.0f)   // left
};
        
Tilemap::Tilemap()
{
    buildCollision = false;
}

Tilemap::~Tilemap()
{

}

void Tilemap::AddTile(const ml::vec2 &position, size_t tileIndex)
{
    tiles[position] = tileIndex;
}

void Tilemap::SuppressTile(const ml::vec2 &position)
{
    auto it = tiles.find(position);
    if (it == tiles.end())
        return;

    tiles.erase(it);
}

bool Tilemap::TileExist(const ml::vec2 &position) const
{
    return (tiles.find(position) != tiles.end());
}

Tile Tilemap::GetTile(const ml::vec2 &position) const
{
    auto it = tiles.find(position);
    if (it == tiles.end())
        return (Tile::none);

    return (TileDictionnary::GetTile(it->second));
}

void Tilemap::Draw()
{
    for (auto it = tiles.begin(); it != tiles.end(); it++)
    {
        Tile tile = TileDictionnary::GetTile(it->second);
        SpriteRenderer::Draw(it->first - tile.spriteOffset, tile.sprite.size, 0, ml::vec3(1, 1, 1), tile.sprite, false, false, 1);
    }
}

void Tilemap::CreateCollision(b2WorldId worldId)
{
    if (!buildCollision)
        return;
    
    // store each line in a multimap, in both sense (A -> B, A <- B)
    std::multimap<ml::vec2, ml::vec2, Vec2Comparator> lines;
    for (auto it = tiles.begin(); it != tiles.end(); it++)
    {
        for (int i = 0; i < 4; i++)
        {
            if (tiles.find(it->first + directions[i]) == tiles.end())
            {
                lines.insert({it->first + points[i].first, it->first + points[i].second});
                lines.insert({it->first + points[i].second, it->first + points[i].first});
            }
        }
    }

    // create the path between all the points
    std::vector<std::vector<ml::vec2>> chains;
    while (lines.size() / 2 != 0)
        chains.push_back(DetermineChainPath(lines));

    // build chains
    for (size_t i = 0; i < chains.size(); i++)
        BuildChain(worldId, chains[i]);
}

std::vector<ml::vec2> Tilemap::DetermineChainPath(std::multimap<ml::vec2, ml::vec2, Vec2Comparator> &lines) const
{
    // determine path
    std::vector<ml::vec2> chainPoints;
    ml::vec2 point = lines.begin()->first;
    for (size_t i = 0; i < lines.size() / 2; i++)
    {
        chainPoints.push_back(point);

        bool pointChanged = false;
        for (auto[it, rangeEnd] = lines.equal_range(point); it != rangeEnd; ++it)
        {
            if (std::find(chainPoints.begin(), chainPoints.end(), it->second) == chainPoints.end())
            {
                point = it->second;
                pointChanged = true;
                break;
            }
        }

        if (!pointChanged)
            break;
    }

    // erase path from multimap
    size_t nbPoints = chainPoints.size();
    for (size_t i = 0; i < nbPoints; i++)
    {
        ml::vec2 p1 = chainPoints[i];
        ml::vec2 p2 = chainPoints[(i + 1) % nbPoints];
        for (auto[it, rangeEnd] = lines.equal_range(p1); it != rangeEnd;)
        {
            if (it->second == p2)
                it = lines.erase(it);
            else
                it++;
        }
        for (auto[it, rangeEnd] = lines.equal_range(p2); it != rangeEnd;)
        {
            if (it->second == p1)
                it = lines.erase(it);
            else
                it++;
        }
    }

    return (chainPoints);
}

void Tilemap::BuildChain(b2WorldId worldId, const std::vector<ml::vec2> &chain)
{
    std::vector<b2Vec2> b2Chain;
    for (size_t i = 0; i < chain.size(); i++)
        b2Chain.push_back({PhysicBody::PixelToWorld(chain[i].x), PhysicBody::PixelToWorld(chain[i].y)});

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    b2BodyId myBodyId = b2CreateBody(worldId, &bodyDef);

    b2ChainDef chainDef = b2DefaultChainDef();
    chainDef.points = b2Chain.data();
    chainDef.count = b2Chain.size();
    chainDef.isLoop = true;
    
    chainsId.push_back(b2CreateChain(myBodyId, &chainDef));
}

void Tilemap::UpdateCollision(b2WorldId worldId)
{
    DeleteCollision();
    CreateCollision(worldId);
}

void Tilemap::DeleteCollision()
{
    for (size_t i = 0; i < chainsId.size(); i++)
        b2DestroyChain(chainsId[i]);
    chainsId.clear();
}
