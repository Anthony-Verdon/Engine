#pragma once

#include "Matrix/Matrix.hpp"
#include <map>
#include "Engine/2D/Tile/Tile.hpp"
#include <box2d/box2d.h>
#include <vector>

struct Vec2Comparator {
    bool operator()(const ml::vec2& lhs, const ml::vec2& rhs) const {
        if (lhs.x != rhs.x) {
            return lhs.x < rhs.x;
        }
        return lhs.y < rhs.y;
    }
};

class Tilemap
{
    private:
        std::map<ml::vec2, size_t, Vec2Comparator> tiles;
        std::vector<b2ChainId> chainsId;
        bool buildCollision;

        std::vector<ml::vec2> DetermineChainPath(std::multimap<ml::vec2, ml::vec2, Vec2Comparator> &lines) const;
        void BuildChain(b2WorldId worldId, const std::vector<ml::vec2> &points);

        void DeleteCollision();
    public:
        Tilemap();
        ~Tilemap();

        void AddTile(const ml::vec2 &position, size_t tileIndex);
        void SuppressTile(const ml::vec2 &position);

        bool TileExist(const ml::vec2 &position) const;
        Tile GetTile(const ml::vec2 &position) const;

        bool GetBuildCollision() const { return (buildCollision); }
        void SetBuildCollision(bool buildCollision) {this->buildCollision = buildCollision; }
        
        void Draw();

        void CreateCollision(b2WorldId worldId);
        void UpdateCollision(b2WorldId worldId);

        const std::map<ml::vec2, size_t, Vec2Comparator>& GetTiles() const { return (tiles); }
};