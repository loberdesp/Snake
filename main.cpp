#include "snake.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Snake");
    window.setFramerateLimit(60);
    snakeBoard board(MENU);
    snake waz(board);
    MSSFMLView view(board, waz);
    while (window.isOpen()) {
        waz.zegarUpdate();
            sf::Event event;
            while (window.pollEvent(event)) {
                view.exit(window, event);
                view.controls(event);
            }
    window.clear();
    view.draw(window);
    window.display();
    } 
}