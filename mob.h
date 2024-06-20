#ifndef MOB_H
#define MOB_H

#include <SFML/Graphics.hpp>
#include <string>
#include <queue>
#include <random>
#include "GameWorld.h"

class Mob {
public:
    std::vector<std::vector<int>> maze;
    Mob(int x, int y,GameWorld& gameWorld);
    void move(float dx, float dy);
    void setPosition(float x, float y);
    sf::Vector2f getPosition(); 
    void draw(sf::RenderWindow& window);
    void setTextureRect();
    void setMaze(GameWorld& gameWorld,sf::Sprite& sprite_perso);
    void mapmob(int x_player, int y_player, int x_mob, int y_mob);
    sf::Vector2f mobMove();
    void mobSetPv(int hp) { pv=hp;}
    int mobGetPv() {return pv;}
    int mobGetSpeed() {return speed;}
    void setMobimun(int t) {mobimun=t;}
    int getMobimun() {return mobimun;}

private:
    int mobimun;
    sf::Vector2f movedir;
    int movephase;
    int pv;
    int speed;
    int animy;
    GameWorld gameWorld;
    sf::Texture texture;
    sf::Sprite sprite;
    int timer;
    int mobSize;
    int haut, bas, gauche, droite;
};

void printMapArray(const std::vector<std::vector<int>>&);
std::vector<std::vector<int>> generateMapArray(GameWorld&,sf::Sprite,int);

#endif // MOB_H