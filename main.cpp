#include "snake.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Minesweeper");
    window.setVerticalSyncEnabled(false);
    window.setFramerateLimit(60);
    snakeBoard board(EASY);
    snake wonsz(board);
    MSSFMLView view(board, wonsz);
    while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                view.handleExit(window, event);
                view.handleClick(window, event);
            }
    window.clear();
    view.draw(window);
    window.display();
    } 
}