#ifndef GAMEWORLD_H
#define GAMEWORLD_H

#include <SFML/Graphics.hpp>
#include "Tiles.h"
#include <vector>

class GameWorld {
    
public:
    bool checkCollision(const sf::FloatRect& bordure) const;
    void setUpTiles();
    void setUpDeco();
    std::vector<GameTile*> deco;
    std::vector<std::vector<GameTile*>> tiles;
    int mapX;
    int mapY;
    GameWorld();
    ~GameWorld(); // Ajoutez un destructeur pour nettoyer les pointeurs
};

#endif // GAMEWORLD_H
