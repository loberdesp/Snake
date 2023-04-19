#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#define TILE_SIZE 16
#define BOARD_SIZE 15
#define WINDOW_SIZE 800

struct field {
    bool hasFood;
    bool hasBarrier;
    bool hasSnake;

    public:
        field();
};

enum gameMode {DEBUG, EASY, MEDIUM, HARD};
enum gameState {RUNNING, WIN, LOSS};
enum dir{TOP, RIGHT, DOWN, LEFT};

class snakeBoard {
    int boardSize;
    std::vector< std::vector <field> > boardArr;
    gameMode msmode;

    public:
        snakeBoard(gameMode mode);
        int getFieldInfo(int x, int y) const;
        void setFood(int x, int y);
        void setBarrier(int x, int y);
        void setSnake(int x, int y);
};

class snakeElem {
    int posX, posY;
    dir direction;

    public:
        snakeElem();
        void setPos(int x, int y);
        void setDir(dir direction); 
        int getElemX() const;
        int getElemY() const;
        int getDirection() const;
};

class snake {
    int length;
    int speed;
    bool snakeState;
    std::vector<snakeElem> snakeBody;
    snakeBoard &msboard;

    public:
        snake(snakeBoard &board);
        int getSnakeState() const;
        int getSnakeHeadX() const;
        int getSnakeHeadY() const;
        int getSpeed() const;
        int getLength() const;
        void snakePush();
        void snakeInit();
        void snakePop();
        void move(dir direction);

};

class MSSFMLView {
    double scale, spriteSize;
    snakeBoard &mssnakeBoard;
    snake &mssnake;
    sf::Texture txtVec[10];
    sf::Sprite sprite[5];
    

    public:
        MSSFMLView(snakeBoard &snakeBoard, snake &snakeVar);
        void draw(sf::RenderWindow &window);
        void drawField(sf::RenderWindow &window, int j, int i);
        void handleExit(sf::RenderWindow &window, sf::Event event);
        void handleClick(sf::RenderWindow &window, sf::Event event);
};
