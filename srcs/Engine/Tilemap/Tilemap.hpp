#pragma once

#include <glm/glm.hpp>
#include <map>
#include "Engine/Tile/Tile.hpp"
#include <box2d/box2d.h>
#include <vector>

struct Vec2Comparator {
    bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const {
        if (lhs.x != rhs.x) {
            return lhs.x < rhs.x;
        }
        return lhs.y < rhs.y;
    }
};

class Tilemap
{
    private:
        std::map<glm::vec2, size_t, Vec2Comparator> tiles;
        std::vector<b2ChainId> chainsId;
        bool buildCollision;

        std::vector<glm::vec2> DetermineChainPath(std::multimap<glm::vec2, glm::vec2, Vec2Comparator> &lines) const;
        void BuildChain(b2WorldId worldId, const std::vector<glm::vec2> &points);

        void DeleteCollision();
    public:
        Tilemap();
        ~Tilemap();

        void AddTile(const glm::vec2 &position, size_t tileIndex);
        void SuppressTile(const glm::vec2 &position);

        bool TileExist(const glm::vec2 &position) const;
        Tile GetTile(const glm::vec2 &position) const;

        bool GetBuildCollision() const { return (buildCollision); }
        void SetBuildCollision(bool buildCollision) {this->buildCollision = buildCollision; }
        
        void Draw();

        void CreateCollision(b2WorldId worldId);
        void UpdateCollision(b2WorldId worldId);

        const std::map<glm::vec2, size_t, Vec2Comparator>& GetTiles() const { return (tiles); }
};