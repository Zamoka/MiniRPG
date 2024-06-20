#ifndef TILES_H
#define TILES_H

#include <SFML/Graphics.hpp>

class GameTile {
public:
    bool isPassable;
    int Profondeur;
    sf::Vector2f pos;
    sf::Texture texture;
    sf::Sprite sprite;
    GameTile(const std::string&, int, int, bool);
    bool setUpSprite(const std::string&);
};

#endif // TILES_H