#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
//#include "GameWorld.cpp"

class Player {
public:
    Player(GameWorld& gameWorld);
    void handleKeyboardInput();
    void handleMouseInput(sf::RenderWindow& window);
    void updateAnimation(sf::Clock& time);
    void updateView(sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);
    bool checkCollision(sf::FloatRect bordure);

    const sf::Sprite& getPlayerSprite() { return sprite_perso; }
    sf::Vector2f getPlayerPosition() const { return sprite_perso.getPosition(); }
    

private:
    int speed;
    sf::Vector2i positionSouris;
    bool updateFps;
    sf::Texture perso;
    sf::Sprite sprite_perso;
    sf::Texture epee;
    sf::Sprite sprite_epee;
    enum Move { Down, Left, Right, Up };
    sf::Vector2i anim;
    sf::Vector2i anim_epee;
    int swordAnimationFrame;
    bool swordVisible;
    bool swordAnimating;
    sf::View view;
    int screenW, screenH, BlockSize;
    GameWorld& gameWorld;
};

#endif // PLAYER_H
