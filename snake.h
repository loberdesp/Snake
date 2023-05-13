#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

#define TILE_SIZE 16
#define BOARD_SIZE 16
#define WINDOW_SIZE 800

struct field {
    bool hasFood;
    bool hasBarrier;
    bool hasSnake;

    public:
        field();
};

enum gameMode {MENU, EASY, MEDIUM, HARD};
enum gameState {RUNNING, WIN, LOSS};
enum dir{TOP, RIGHT, DOWN, LEFT};

class snakeBoard {
    std::vector< std::vector <field> > boardArr;
    gameMode msmode;

    public:
        snakeBoard(gameMode mode);
        int getDif() const;
        int getFieldInfo(int x, int y) const;
        void toggleFood(int x, int y);
        void toggleBarrier(int x, int y);
        void toggleSnake(int x, int y);
        int checkFood(int x, int y) const;
        int checkBarrier(int x, int y) const;
        int checkSnake(int x, int y) const;
        void setDif(gameMode mode);
        int rngCoord();
        void rngFood();
        void rngBarrier(gameMode mode);
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
    sf::Clock clock;

    public:
        snake(snakeBoard &board);
        int getSnakeState() const;
        int getSnakeHeadX() const;
        int getSnakeHeadY() const;
        int getSpeed() const;
        int getLength() const;
        void snakePush();
        void snakeInit();
        int isLegal(dir direction) const;
        void move();
        int updateSnake();
        void zegarUpdate();
        void changeDir(dir direction);
        void checkBounds();
        void 

};

class MSSFMLView {
    double scale, spriteSize;
    snakeBoard &mssnakeBoard;
    snake &mssnake;
    sf::Texture txtVec[15];
    sf::Sprite sprite[10];
    

    public:
        MSSFMLView(snakeBoard &snakeBoard, snake &snakeVar);
        void draw(sf::RenderWindow &window);
        void drawField(sf::RenderWindow &window, int j, int i);
        void exit(sf::RenderWindow &window, sf::Event event);
        void controls(sf::Event event);
};
