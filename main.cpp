#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include "GameWorld.cpp"
//#include "Tiles.cpp"
#include "mob.cpp"

sf::RenderWindow window;
sf::RectangleShape rect;
sf::RectangleShape rect2;
int speed = 5;
sf::Vector2i positionSouris;
bool updateFps = true;
sf::Texture perso;
sf::Sprite sprite_perso;
sf::Texture epee;
sf::Sprite sprite_epee;
enum Move { Down, Left, Right, Up };
sf::Vector2i anim(1, Down);
sf::Vector2i anim_epee(0, 0);
int swordAnimationFrame = 0;
bool swordVisible = false;
bool swordAnimating = false;
sf::View view;
int screenW = 800, screenH = 600;
int BlockSize = 48;
GameWorld gameWorld;

Mob mob(200,200,gameWorld);
//Mob mob1(1000,1000,gameWorld);
//Mob mob2(1500,50,gameWorld);

void init();
bool checkCollision(sf::FloatRect bordure);
void handleKeyboardInput();
void handleMouseInput();
void updateAnimation(sf::Clock& time);
void updateSwordAnimation();
void updateView();
void drawGame();
//std::vector<std::vector<int>> generateMapArray();


int main() {
    init();

    sf::Clock time;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
                updateFps = true;
            else {
                anim.x = 1;
                updateFps = false;
            }
        }
        
        handleKeyboardInput();

        mob.setMaze(gameWorld,sprite_perso);
        mob.mapmob(sprite_perso.getPosition().x/50,sprite_perso.getPosition().y/50, 200/50,200/50);

        //mob1.setMaze(gameWorld,sprite_perso);
        //mob1.mapmob(sprite_perso.getPosition().x/50,sprite_perso.getPosition().y/50, 1000/50,1000/50);

        //mob2.setMaze(gameWorld,sprite_perso);
        // mob2.mapmob(sprite_perso.getPosition().x/50,sprite_perso.getPosition().y/50, 1500/50,50/50);
        //printMapArray(mob.maze);
        
        updateAnimation(time);
        
        //updateSwordAnimation();
        updateView();
        
        drawGame();
        
    }
    return 0;
}

void init() {
    window.create(sf::VideoMode(screenW, screenH), "RPG");
    window.setPosition(sf::Vector2i(192, 0));
    window.setFramerateLimit(60);

    rect.setPosition(100, 100);
    rect.setSize(sf::Vector2f(50, 50));
    rect.setFillColor(sf::Color(255, 0, 0));

    rect2.setPosition(1000, 1000);
    rect2.setSize(sf::Vector2f(50, 50));
    rect2.setFillColor(sf::Color(255, 0, 0));

    if (!perso.loadFromFile("./Texture/Perso/Pnj48.png")) {
        std::cout << "Erreur chargement pnj.png" << std::endl;
    }
    perso.setSmooth(true);
    sprite_perso.setTexture(perso);

    if (!epee.loadFromFile("./Texture/Perso/epee5.png")) {
        std::cout << "Erreur chargement epee.png" << std::endl;
    }
    epee.setSmooth(true);
    sprite_epee.setTexture(epee);
}

bool checkCollision(sf::FloatRect bordure) {
    for (int i = 0; i < gameWorld.mapY; i++) {
        for (int j = 0; j < gameWorld.mapX; j++) {
            if (!gameWorld.tiles[i][j]->isPassable) {
                if (bordure.intersects(gameWorld.tiles[i][j]->sprite.getGlobalBounds())) {
                    return true;
                }
            }
        }
    }

    for (GameTile* decoElement : gameWorld.deco) {
        if (!decoElement->isPassable) {
            if (bordure.intersects(decoElement->sprite.getGlobalBounds())) {
                return true;
            }
        }
    }
    return false;
}

void handleKeyboardInput() {
    sf::Vector2f move(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        anim.y = Up;
        move.y = -speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        anim.y = Down;
        move.y = speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        anim.y = Right;
        move.x = speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        anim.y = Left;
        move.x = -speed;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        window.close();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (!swordAnimating) {
            swordVisible = true;
            swordAnimating = true;
            swordAnimationFrame = 0;
            anim_epee.x = 0;
        }
    }

    sf::FloatRect hitbox = sprite_perso.getGlobalBounds();
    hitbox.top += 20;
    hitbox.height -= 20;
    hitbox.left += 10;
    hitbox.width -= 20;

    sf::FloatRect futureBordure = hitbox;
    futureBordure.left += move.x;
    futureBordure.top += move.y;

    if (!checkCollision(futureBordure)) {
        sprite_perso.move(move);
    }

    if (sprite_perso.getPosition().x <= 0)
        sprite_perso.setPosition(sf::Vector2f(0, sprite_perso.getPosition().y));

    if (sprite_perso.getPosition().y <= 0)
        sprite_perso.setPosition(sf::Vector2f(sprite_perso.getPosition().x, 0));

    if (sprite_perso.getPosition().x > (gameWorld.mapX - 1) * 50)
        sprite_perso.setPosition(sf::Vector2f((gameWorld.mapX - 1) * 50, sprite_perso.getPosition().y));

    if (sprite_perso.getPosition().y > (gameWorld.mapY - 1) * 50)
        sprite_perso.setPosition(sf::Vector2f(sprite_perso.getPosition().x, (gameWorld.mapY - 1) * 50));
}

void handleMouseInput() {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        positionSouris = sf::Mouse::getPosition(window);
    }
}

void updateAnimation(sf::Clock& time) {
    if (time.getElapsedTime().asMilliseconds() >= 150) {
        if (updateFps) {
            anim.x++;
            if (anim.x * BlockSize >= 96)
                anim.x = 0;
        }

        if (swordAnimating) {
            swordAnimationFrame++;
            if (swordAnimationFrame >= 5) { // Assuming there are 3 frames for the sword animation
                swordVisible = false;
                swordAnimating = false;
            }
        }

        if (swordVisible) {
            anim_epee.x++;
            if (anim_epee.x * BlockSize >= 288)
                anim_epee.x = 0;
        }

        time.restart();
    }
}

void updateView() {
    view.reset(sf::FloatRect(0, 0, screenW, screenH));
    sf::Vector2f position(screenW / 2, screenH / 2);
    position.x = sprite_perso.getPosition().x + 16 - (screenW / 2);
    position.y = sprite_perso.getPosition().y + 16 - (screenH / 2);

    if (position.x < 0)
        position.x = 0;
    if (position.y < 0)
        position.y = 0;
    if (position.x > screenW)
        position.x = screenW;
    if (position.y > screenH)
        position.y = screenH;

    view.reset(sf::FloatRect(position.x, position.y, screenW, screenH));
    window.setView(view);
}

void drawGame() {
    sprite_perso.setTextureRect(sf::IntRect(anim.x * BlockSize, anim.y * BlockSize, BlockSize, BlockSize));
    sprite_epee.setTextureRect(sf::IntRect(anim_epee.x * BlockSize, anim.y * BlockSize, BlockSize, BlockSize));

    for (int p = 1; p < 4; p++) {
        for (int i = 0; i < gameWorld.mapY; i++) {
            for (int j = 0; j < gameWorld.mapX; j++) {
                if (gameWorld.tiles[i][j]->Profondeur == p)
                    window.draw(gameWorld.tiles[i][j]->sprite);
            }
        }
        if (p == 2) {
            window.draw(rect);
            window.draw(rect2);
            window.draw(sprite_perso);
            mob.draw(window);
            //mob1.draw(window);
            if (swordVisible) {
                if (anim.y == Up)
                    sprite_epee.setPosition(sprite_perso.getPosition().x, sprite_perso.getPosition().y - 32);
                else if (anim.y == Right)
                    sprite_epee.setPosition(sprite_perso.getPosition().x + 32, sprite_perso.getPosition().y);
                else if (anim.y == Left)
                    sprite_epee.setPosition(sprite_perso.getPosition().x - 32, sprite_perso.getPosition().y);
                else if (anim.y == Down)
                    sprite_epee.setPosition(sprite_perso.getPosition().x, sprite_perso.getPosition().y + 32);
                window.draw(sprite_epee);
            }
            handleMouseInput();
        }

        for (int i = 0; i < gameWorld.deco.size(); i++) {
            if (gameWorld.deco[i]->Profondeur == p)
                window.draw(gameWorld.deco[i]->sprite);
        }
    }

    window.display();
    window.clear();
}

