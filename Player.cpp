#include "Player.h"

Player::Player(GameWorld& gameWorld) 
    : gameWorld(gameWorld), speed(5), updateFps(true), anim(1, Down), 
      anim_epee(0, 0), swordAnimationFrame(0), swordVisible(false), 
      swordAnimating(false), screenW(800), screenH(600), BlockSize(48) {

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

bool Player::checkCollision(sf::FloatRect bordure) {
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

void Player::handleKeyboardInput() {
    sf::Vector2f move(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        anim.y = Up;
        move.y = -speed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        anim.y = Down;
        move.y = speed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        anim.y = Right;
        move.x = speed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        anim.y = Left;
        move.x = -speed;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
        exit(0); // or other way to close window
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

void Player::handleMouseInput(sf::RenderWindow& window) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        positionSouris = sf::Mouse::getPosition(window);
    }
}

void Player::updateAnimation(sf::Clock& time) {
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

void Player::updateView(sf::RenderWindow& window) {
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

void Player::draw(sf::RenderWindow& window) {
    sprite_perso.setTextureRect(sf::IntRect(anim.x * BlockSize, anim.y * BlockSize, BlockSize, BlockSize));
    sprite_epee.setTextureRect(sf::IntRect(anim_epee.x * BlockSize, anim.y * BlockSize, BlockSize, BlockSize));

    window.draw(sprite_perso);
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
}
