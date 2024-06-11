#include "mob.h"


Mob::Mob(int x, int y,GameWorld& gameWorld) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);

    int randomValue = dis(gen); // Génère une fois le nombre aléatoire
    std::cout << randomValue << std::endl;

    if (randomValue == 0) {
        std::cout << "zombie" << std::endl;
        if (!texture.loadFromFile("./Texture/Perso/zombie.png")) {
            std::cout << "Erreur chargement zombie.png" << std::endl;
        }
        speed=1;
        pv=2;
    } else if (randomValue == 1) {
        std::cout << "goblin" << std::endl;
        if (!texture.loadFromFile("./Texture/Perso/goblin.png")) {
            std::cout << "Erreur chargement goblin.png" << std::endl;
        }
        pv=1;
        speed=2;
    } else if (randomValue == 2) {
        std::cout << "hobgoblin" << std::endl;
        if (!texture.loadFromFile("./Texture/Perso/hobgoblin.png")) {
            std::cout << "Erreur chargement hobgoblin.png" << std::endl;
        }
        speed=1;
        pv=5;
    }
    gameWorld=gameWorld;
    mobSize = 48;
    timer = 0;
    animy=0;
    
    sprite.setTexture(texture);
    setTextureRect();

    // Définir l'origine du sprite au centre
    //sprite.setOrigin(mobSize / 2, mobSize / 2);
    sprite.setOrigin(0,0);

    sprite.setPosition(x, y);
}



void Mob::move(float dx, float dy) {
    sprite.move(dx, dy);
}

void Mob::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Mob::draw(sf::RenderWindow& window) {
    std::vector<sf::Vector2i> moves = mobMove();

    // Ajuster la hitbox pour les collisions
    sf::FloatRect hitbox = sprite.getGlobalBounds();
    hitbox.top += 20;
    hitbox.height -= 20;
    hitbox.left += 10;
    hitbox.width -= 20;

    for (const auto& move : moves) {
        // Calculer la nouvelle position en pixels
        float newX = sprite.getPosition().x + move.x;
        float newY = sprite.getPosition().y + move.y;

        sf::FloatRect futureBordure = hitbox;
        futureBordure.left += move.x * mobSize;
        futureBordure.top += move.y * mobSize;

        // Vérifiez les collisions avant de déplacer le mob
        if (!gameWorld.checkCollision(futureBordure)) {
            // Mettre à jour l'orientation du mob en fonction de la direction
            if (move.x > 0) {
                animy = 2; // Vers la droite
            } else if (move.x < 0) {
                animy = 1; // Vers la gauche
            } else if (move.y > 0) {
                animy = 0; // Vers le bas
            } else if (move.y < 0) {
                animy = 3; // Vers le haut
            }
            setTextureRect();
            sprite.setPosition(newX, newY);
            break; // Sortir de la boucle dès qu'un mouvement valide est trouvé
        }
    }

    window.draw(sprite);
}

void Mob::setTextureRect() {
    timer++;
    if (timer >= 60)
        timer = 0;
    sprite.setTextureRect(sf::IntRect((timer / 20) * 48, animy * 48, 48, 48));
}


void Mob::setMaze(GameWorld& gameWorld, sf::Sprite& sprite_perso)
{
    this->maze=generateMapArray(gameWorld,sprite_perso,mobSize);
}

void printMapArray(const std::vector<std::vector<int>>& mapArray) {
    for (const auto& row : mapArray) {
        for (int cell : row) {
            std::cout << cell << " ";
        }
        std::cout << std::endl;
    }
    std::cout <<""<< std::endl;
}

std::vector<std::vector<int>> generateMapArray(GameWorld& gameWorld,sf::Sprite sprite_perso,int BlockSize) 
{
    std::vector<std::vector<int>> mapArray;
    
    if (gameWorld.mapY <= 0 || gameWorld.mapX <= 0) {
        std::cerr << "Taille de la carte invalide." << std::endl;
        return mapArray;
    }

    mapArray.resize(gameWorld.mapY, std::vector<int>(gameWorld.mapX, 0));

    // Populate the mapArray based on tiles
    for (int i = 0; i < gameWorld.mapY; i++) {
        
        for (int j = 0; j < gameWorld.mapX; j++) {
            
            if (!gameWorld.tiles[i][j]->isPassable) {
                mapArray[i][j] = -1;
            }
        }
    }
    

    // Populate the mapArray based on deco
    for (int k=0; k<gameWorld.deco.size(); k++) {
        int i = gameWorld.deco[k]->pos.y/50;
        int j = gameWorld.deco[k]->pos.x/50;
        // Vérifier si les coordonnées sont valides
        if (i >= 0 && i < mapArray.size() && j >= 0 && j < mapArray[0].size()) {
            
            if (!gameWorld.deco[k]->isPassable) {
                mapArray[i][j] = -1; // Set to 0 if not passable
            }
        }
    }
    // Set the player position in the mapArray
    int playerX = sprite_perso.getPosition().x / BlockSize;
    int playerY = sprite_perso.getPosition().y / BlockSize;
    if (playerX >= 0 && playerX < gameWorld.mapX && playerY >= 0 && playerY < gameWorld.mapY) {
        mapArray[playerY][playerX] = 0;
    }
    return mapArray;
}



void Mob::mapmob(int x_player, int y_player, int x_mob, int y_mob)
{
    std::queue<std::pair<int, int>> queue;
    queue.push(std::make_pair(x_player, y_player));
    maze[y_player][x_player] = 0; // La case du joueur a une distance de zéro

    while (!queue.empty())
    {
        int x = queue.front().first;
        int y = queue.front().second;
        queue.pop();

        // Les déplacements possibles : haut, bas, gauche, droite
        int dx[4] = {0, 0, -1, 1};
        int dy[4] = {-1, 1, 0, 0};

        for (int i = 0; i < 4; ++i)
        {
            int new_x = x + dx[i];
            int new_y = y + dy[i];

            if (new_x >= 0 && new_x < maze[0].size() &&
                new_y >= 0 && new_y < maze.size() &&
                maze[new_y][new_x] == 0) // Seules les cases non visitées sont explorées
            {
                // Mettre à jour la distance et ajouter à la file d'attente
                maze[new_y][new_x] = maze[y][x] + 1;
                queue.push(std::make_pair(new_x, new_y));
            }
        }
    }
    maze[y_player][x_player]=0;
}

std::vector<sf::Vector2i> Mob::mobMove() {
    std::vector<sf::Vector2i> moves;

    int x = sprite.getPosition().x / mobSize;
    int y = sprite.getPosition().y / mobSize;

    int up = 200, down = 200, right = 200, left = 200;

    if (y - 1 >= 0 && maze[y - 1][x] != -1)
        up = maze[y - 1][x];
    if (y + 1 < maze.size() && maze[y + 1][x] != -1)
        down = maze[y + 1][x];
    if (x - 1 >= 0 && maze[y][x - 1] != -1)
        left = maze[y][x - 1];
    if (x + 1 < maze[0].size() && maze[y][x + 1] != -1)
        right = maze[y][x + 1];

    int minDistance = std::min({up, down, left, right});

    if (up == minDistance)
        moves.push_back({0, -speed});
    if (down == minDistance)
        moves.push_back({0, speed});
    if (left == minDistance)
        moves.push_back({-speed, 0});
    if (right == minDistance)
        moves.push_back({speed, 0});

    return moves;
}
