#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Structure de données pour représenter un niveau de Sokoban
struct Level {
  std::vector<std::string> map;  // La carte du niveau, sous forme de chaîne de caractères
  int player_x;  // La position x (colonne) du joueur
  int player_y;  // La position y (ligne) du joueur
};

// Lit un niveau à partir d'un fichier et le retourne sous forme de structure de données
Level read_level(const std::string& filename) {
  Level level;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Erreur lors de l'ouverture du fichier " << filename << std::endl;
    return level;
  }

  std::string line;
  while (std::getline(file, line)) {
    level.map.push_back(line);
    for (int i = 0; i < line.size(); i++) {
      if (line[i] == '@') {
        level.player_x = i;
        level.player_y = level.map.size() - 1;
      }
    }
  }

  return level;
}

// Déplace le joueur dans la direction indiquée et met à jour la carte en conséquence
// Retourne true si le déplacement a été effectué, false sinon
#include <SFML/Graphics.hpp>
// ...
int boxSize = 32;
float speed = 0.1f; 
void drawMap(const Level& level, sf::RenderWindow& window) {
  for (int y = 0; y < level.map.size(); y++) {
    for (int x = 0; x < level.map[y].size(); x++) {
      sf::RectangleShape rect;
      rect.setSize(sf::Vector2f(boxSize, boxSize));
      rect.setPosition(x * boxSize, y * boxSize);
      if (level.map[y][x] == '#') {
        rect.setFillColor(sf::Color::Black);
      } else if (level.map[y][x] == '@') {
        rect.setFillColor(sf::Color::White);
      } else if (level.map[y][x] == '.') {
        rect.setFillColor(sf::Color::Yellow);
      } else if (level.map[y][x] == '$') {
        rect.setFillColor(sf::Color::Blue);
      }
      window.draw(rect);
    }
  }
}

bool move_player(Level& level, sf::RenderWindow& window) {
  char c = getchar();
  int dx = 0, dy = 0;
  if (c == 'w' || c == 'W') {
    dy = -1;
  } else if (c == 'a' || c == 'A') {
    dx = -1;
  } else if (c == 's' || c == 'S') {
    dy = 1;
  } else if (c == 'd' || c == 'D') {
    dx = 1;
  } else {
    return false;
  }

  int new_x = level.player_x + dx;
  int new_y = level.player_y + dy;

  // Check if the movement is within the bounds of the map
  if (new_x < 0 || new_x >= level.map[0].size() || new_y < 0 || new_y >= level.map.size()) {
    return false;
  }

  // Check if the destination cell is empty or a target
  if (level.map[new_y][new_x] == ' ' || level.map[new_y][new_x] == '.') {
    level.map[level.player_y][level.player_x] = ' ';

    // Animation
    sf::RectangleShape player;
    player.setSize(sf::Vector2f(boxSize, boxSize));
    player.setFillColor(sf::Color::White);
    player.setPosition(level.player_x*boxSize, level.player_y*boxSize);

    // Move the player sprite
    for(int i = 0; i < boxSize; i++)
    {
        player.move(dx*speed, dy*speed);
        window.clear();
        drawMap(level, window);
        window.draw(player);
        window.display();
    }
    level.player_x = new_x;
    level.player_y = new_y;
    level.map[level.player_y][level.player_x] = '@';
    return true;
  }

  // Check if the destination cell is a box
  if (level.map[new_y][new_x] == '$') {
    int box_x = new_x + dx;
    int box_y = new_y + dy;
    if (box_x < 0 || box_x >= level.map[0].size() || box_y < 0 || box_y >= level.map.size()) {
      return false;
    }
    if (level.map[box_y][box_x] != ' ' && level.map[box_y][box_x] != '.') {
      return false;
    }
    level.map[level.player_y][level.player_x] = ' ';
    level.map[new_y][new_x] = '@';
    // Animation
 sf::RectangleShape player;
 player.setSize(sf::Vector2f(boxSize, boxSize));
 player.setFillColor(sf::Color::White);
 player.setPosition(new_x*boxSize, new_y*boxSize);
 sf::RectangleShape box;
 box.setSize(sf::Vector2f(boxSize, boxSize));
 box.setFillColor(sf::Color::Blue);
 box.setPosition(new_x*boxSize+dx*boxSize, new_y*boxSize+dy*boxSize);

 // Move the player and box sprites
 for(int i = 0; i < boxSize; i++)
 {
     player.move(dx*speed, dy*speed);
     box.move(dx*speed, dy*speed);
     window.clear();
     drawMap(level, window);
     window.draw(player);
     window.draw(box);
     window.display();
 }
 level.map[box_y][box_x] = '$';
 level.player_x = new_x;
level.player_y = new_y;
return true;
}

return false;
}






    // Vérifie si le niveau est résolu, c'est-à-dire si toutes les boîtes sont sur des cibles
    bool is_solved(const Level& level) {
    for (int y = 0; y < level.map.size(); y++) {
    for (int x = 0; x < level.map[y].size(); x++) {
    if (level.map[y][x] == '$') {
    return false;
    }
    }
    }
    return true;
    }

    // Dessine le niveau à l'écran
    void draw_level(const Level& level) {
    for (int y = 0; y < level.map.size(); y++) {
    for (int x = 0; x < level.map[y].size(); x++) {
    std::cout << level.map[y][x];
    }
    std::cout << std::endl;
    }
    }

    int main() {
      Level level = read_level("level1.txt");
      sf::RenderWindow window(sf::VideoMode(level.map[0].size()*boxSize, level.map.size()*boxSize), "Sokoban");
      while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
          if (event.type == sf::Event::Closed) {
            window.close();
          }
        }
        window.clear();
        drawMap(level, window);
        move_player(level, window);
        window.display();
      }
      return 0;
    }
