#include "Tiles.h"

GameTile::GameTile(const std::string& textureName, int x, int y, bool passable) {
    if (!setUpSprite(textureName))
        return;
    pos = sf::Vector2f(x, y);
    sprite.setPosition(pos);
    isPassable = passable;

    if (passable==false || textureName=="./Texture/map/grass.png" || textureName=="./Texture/map/water.png" || textureName=="./Texture/map/pond.png" ||textureName=="./Texture/map/revpond.png")
        Profondeur=1;
    else
        Profondeur=3;
}

bool GameTile::setUpSprite(const std::string& textureName) {
    if (!texture.loadFromFile(textureName)) {
        return false;
    }
    texture.setSmooth(true);
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(0, 0, 50, 50));
    return true;
}
