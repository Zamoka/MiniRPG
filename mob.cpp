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
        speed=2;
        pv=2;
    } else if (randomValue == 1) {
        std::cout << "goblin" << std::endl;
        if (!texture.loadFromFile("./Texture/Perso/goblin.png")) {
            std::cout << "Erreur chargement goblin.png" << std::endl;
        }
        pv=1;
        speed=3;
    } else if (randomValue == 2) {
        std::cout << "hobgoblin" << std::endl;
        if (!texture.loadFromFile("./Texture/Perso/hobgoblin.png")) {
            std::cout << "Erreur chargement hobgoblin.png" << std::endl;
        }
        speed=1;
        pv=5;
    }
    movedir.x=-1;
    movedir.y=0;
    movephase=0;
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

void Mob::setPosition(float x, float y) {
    sprite.setPosition(x, y);
}

void Mob::draw(sf::RenderWindow& window) {

    sf::Vector2f moves;
    if (movephase < 50) {
        moves = sf::Vector2f(movedir.x , movedir.y );
        movephase += speed;
    } else {
        sf::Vector2f newMoveDir = mobMove();
        moves = sf::Vector2f(newMoveDir.x , newMoveDir.y );
        movedir = sf::Vector2f(newMoveDir.x / speed, newMoveDir.y / speed);
        movephase = speed;
    }

    if (moves.x != 0) {
        moves.x = (moves.x > 0) ? speed : -speed;
    }
    if (moves.y != 0) {
        moves.y = (moves.y > 0) ? speed : -speed;
    }

    //std::cout<< sprite.getPosition().x << " "<< sprite.getPosition().y << std::endl;
    // Calculer la nouvelle position en pixels
    int X = sprite.getPosition().x + moves.x;
    int Y = sprite.getPosition().y + moves.y;
    if (moves.x > 0) {
        animy = 2; // Vers la droite
    } else if (moves.x < 0) {
        animy = 1; // Vers la gauche
    } else if (moves.y > 0) {
        animy = 0; // Vers le bas
    } else if (moves.y < 0) {
        animy = 3; // Vers le haut
    }
    setTextureRect();
    sprite.setPosition(X, Y);    

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

sf::Vector2f Mob::getPosition() 
{
    sf::Vector2f coord(0, 0);
    coord.x = sprite.getPosition().x;
    coord.y = sprite.getPosition().y;
    return coord;
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

sf::Vector2f Mob::mobMove() {
    sf::Vector2f move(0, 0);
    int up = 200, down = 200, right = 200, left = 200; // Initialisez à une valeur élevée
    int x = sprite.getPosition().x / mobSize; // Convertissez les coordonnées du sprite en indices de grille
    int y = sprite.getPosition().y / mobSize;

    // Vérifiez les limites avant d'accéder aux éléments du tableau
    if (y - 1 >= 0 && maze[y - 1][x] != -1)
        up = maze[y - 1][x];
    if (y + 1 < maze.size() && maze[y + 1][x] != -1)
        down = maze[y + 1][x];
    if (x - 1 >= 0 && maze[y][x - 1] != -1)
        left = maze[y][x - 1];
    if (x + 1 < maze[0].size() && maze[y][x + 1] != -1)
        right = maze[y][x + 1];

    // Comparez les valeurs et déterminez la direction du mouvement

    if (up <= down && up <= left && up <= right)
        move.y = -speed;
    else if (down <= up && down <= left && down <= right)
        move.y = speed;
    else if (left <= down && left <= up && left <= right)
        move.x = -speed;
    else if (right <= down && right <= left && right <= up)
        move.x = speed;

    return move;
}