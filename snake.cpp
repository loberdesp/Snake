#include "snake.h"

field::field() {
    hasFood = 0;
    hasBarrier = 0;
    hasSnake = 0;
}








snakeBoard::snakeBoard(gameMode mode) : msmode(mode) {
    boardArr.resize(BOARD_SIZE);
    for(int i=0;i<BOARD_SIZE;i++) {
        boardArr[i].resize(BOARD_SIZE);
    }
}


int snakeBoard::getDif() const {
    return msmode;
}

int snakeBoard::getFieldInfo(int x, int y) const {
    if(boardArr[y][x].hasBarrier==1) {
        return 'b';
    }
    if(boardArr[y][x].hasFood==1) {
        return 'f';
    }
    if(boardArr[y][x].hasSnake==1) {
        return 's';
    }
    return 0;
}

void snakeBoard::toggleFood(int x, int y) {
    if(boardArr[y][x].hasFood==0) {
        boardArr[y][x].hasFood = 1;
    } else {
        boardArr[y][x].hasFood = 0;
    }
}

void snakeBoard::toggleBarrier(int x, int y) {
    if(boardArr[y][x].hasBarrier==0) {
        boardArr[y][x].hasBarrier = 1;
    } else {
        boardArr[y][x].hasBarrier = 0;
    }
}

void snakeBoard::toggleSnake(int x, int y) {
    if(boardArr[y][x].hasSnake==0) {
        boardArr[y][x].hasSnake = 1;
    } else {
        boardArr[y][x].hasSnake = 0;
    }
}

int snakeBoard::checkBarrier(int x, int y) const {
    if(boardArr[y][x].hasBarrier==1) {
        return 1;
    } else {
        return 0;
    }
}

int snakeBoard::checkFood(int x, int y) const {
    if(boardArr[y][x].hasFood==1) {
        return 1;
    } else {
        return 0;
    }
}

int snakeBoard::checkSnake(int x, int y) const {
    if(boardArr[y][x].hasSnake==1) {
        return 1;
    } else {
        return 0;
    }
}

int snakeBoard::rngCoord() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,BOARD_SIZE-1);
    int rng = dist(mt);
    return rng;
}

void snakeBoard::rngFood() {
    int rngHeight = rngCoord();
    int rngWidth = rngCoord();
    while(checkSnake(rngWidth, rngHeight)==1 || checkBarrier(rngWidth, rngHeight)==1) {
        rngHeight = rngCoord();
        rngWidth = rngCoord();
    }
    toggleFood(rngWidth, rngHeight);
}

void snakeBoard::rngBarrier(gameMode mode) {
    double barRate;
    int size = BOARD_SIZE*BOARD_SIZE;
    switch(mode) {
        case EASY:
            barRate = (double)30/1;
        break;
        case MEDIUM:
            barRate = (double)30/2;
        break;
        case HARD:
            barRate = (double)30/3;
        break;
        case MENU:
        break;
    }
    int numOfBars = std::ceil(size/barRate);
    for(int i=0; i<numOfBars;i++) {
        int rngHeight = rngCoord();
        int rngWidth = rngCoord();
        while(rngHeight<1 && rngHeight>BOARD_SIZE-1) {
            rngHeight = rngCoord();
            rngWidth = rngCoord();
        }
        toggleBarrier(rngWidth, rngHeight);
    }
}

void snakeBoard::setDif(gameMode mode) {
    msmode=mode;
    rngBarrier(mode);
    rngFood();
}















snakeElem::snakeElem(): posX(0), posY(0), direction(TOP) {
}

void snakeElem::setPos(int x, int y) {
    posX = x;
    posY = y;
}

void snakeElem::setDir(dir snakeDir) {
    direction = snakeDir;
}

int snakeElem::getElemX() const {
    return posX;
}

int snakeElem::getElemY() const {
    return posY;
}

int snakeElem::getDirection() const {
    return direction;
}


snake::snake(snakeBoard &board) : length(1), speed(1), msboard(board) {
    snakeInit();
}

int snake::getSnakeState() const {
    return snakeState;
}

int snake::getSpeed() const {
    return speed;
}


int snake::getLength() const {
    return length;
}

int snake::getSnakeHeadX() const {
    return snakeBody.front().getElemX();
}

int snake::getSnakeHeadY() const {
    return snakeBody.front().getElemY();
}

void snake::snakePush() {
    snakeElem newElem;
    newElem.setPos(snakeBody.back().getElemX(), snakeBody.back().getElemY());
    snakeBody.push_back(newElem);
    length++;
}

void snake::snakeInit() {
    snakeElem initElem;
    initElem.setDir(TOP);
    initElem.setPos(BOARD_SIZE/2,BOARD_SIZE/2);
    snakeBody.push_back(initElem);
    msboard.toggleSnake(BOARD_SIZE/2,BOARD_SIZE/2);
    initElem.setPos((BOARD_SIZE/2),(BOARD_SIZE/2)+1);
    snakeBody.push_back(initElem);
    msboard.toggleSnake((BOARD_SIZE/2),(BOARD_SIZE/2)+1);
    initElem.setPos((BOARD_SIZE/2),(BOARD_SIZE/2)+2);
    snakeBody.push_back(initElem);
    msboard.toggleSnake((BOARD_SIZE/2),(BOARD_SIZE/2)+2);
    length = snakeBody.size()-1;
}


int snake::updateSnake() {
    if(msboard.checkFood(getSnakeHeadX(), getSnakeHeadY())==1) {
        msboard.toggleFood(getSnakeHeadX(), getSnakeHeadY());
        snakePush();
        msboard.rngFood();
        std::cout << "nowy element, dlugosc: " << length << std::endl;
        return 1;
    } else if(msboard.checkBarrier(getSnakeHeadX(), getSnakeHeadY())==1) {
        std::cout << "przegrales" << std::endl;
        exit (0);
    }
    return 0;
}

int snake::isLegal(dir direction) const {
    if((direction==TOP && snakeBody[0].getDirection()!=DOWN) || (direction==RIGHT && snakeBody[0].getDirection()!=LEFT) || (direction==DOWN && snakeBody[0].getDirection()!=TOP) || (direction==LEFT && snakeBody[0].getDirection()!=RIGHT)) {
        return 1;
    } else {
        std::cout << "cos sie stalo niedobrego" << std::endl;
        return 0;
    }
}

void snake::checkBounds() {
    switch(snakeBody[0].getDirection()) {
        case TOP:
        if(getSnakeHeadY()==0) {
            std::cout << "przegrales" << std::endl;
            exit(0);
        }
        break;
        case DOWN:
            if(getSnakeHeadY()==BOARD_SIZE-1) {
                std::cout << "przegrales" << std::endl;
                exit(0);
            }
        break;
        case RIGHT:
            if(getSnakeHeadX()==BOARD_SIZE-1) {
                std::cout << "przegrales" << std::endl;
                exit(0);
            }
        break;
        case LEFT:
            if(getSnakeHeadX()==0) {
                std::cout << "przegrales" << std::endl;
                exit(0);
            }
        break;
    }
}

void snake::move() {
    checkBounds();
    int tmpX = getSnakeHeadX();
    int tmpY = getSnakeHeadY();
    switch(snakeBody[0].getDirection()) {
        case TOP:
                snakeBody[0].setDir(TOP);
                snakeBody[0].setPos(getSnakeHeadX(),getSnakeHeadY()-1);
                msboard.toggleSnake(getSnakeHeadX(),getSnakeHeadY());
        break;
        case RIGHT:
                snakeBody[0].setDir(RIGHT);
                snakeBody[0].setPos(getSnakeHeadX()+1,getSnakeHeadY());
                msboard.toggleSnake(getSnakeHeadX(),getSnakeHeadY());
        break;
        case DOWN:
                snakeBody[0].setDir(DOWN);
                snakeBody[0].setPos(getSnakeHeadX(),getSnakeHeadY()+1);
                msboard.toggleSnake(getSnakeHeadX(),getSnakeHeadY());
        break;
        case LEFT:
                snakeBody[0].setDir(LEFT);
                snakeBody[0].setPos(getSnakeHeadX()-1,getSnakeHeadY());
                msboard.toggleSnake(getSnakeHeadX(),getSnakeHeadY());
        break;
    }
    if(updateSnake()==0) {
        msboard.toggleSnake(snakeBody.back().getElemX(), snakeBody.back().getElemY());
    }
    for(int i=snakeBody.size();i>0;i--) {
        if(i==1) {
            snakeBody[i].setPos(tmpX, tmpY);
        } else {
            snakeBody[i].setPos(snakeBody[i-1].getElemX(), snakeBody[i-1].getElemY());
        }
    }
}

void snake::zegarUpdate() {
    if(msboard.getDif()!=MENU) {
        sf::Time time1 = clock.getElapsedTime();
        if(time1.asMilliseconds()>=500) {
            clock.restart();
            move();
            updateSnake();
        }
    }
}

void snake::changeDir(dir direction) {
    switch(direction) {
        case TOP:
            snakeBody[0].setDir(TOP);
        break;
        case DOWN:
            snakeBody[0].setDir(DOWN);
        break;
        case RIGHT:
            snakeBody[0].setDir(RIGHT);
        break;
        case LEFT:
            snakeBody[0].setDir(LEFT);
        break;
    }
}











MSSFMLView::MSSFMLView(snakeBoard &board, snake &snakeVar) : mssnakeBoard(board), mssnake(snakeVar) {
    scale = WINDOW_SIZE / double((BOARD_SIZE*TILE_SIZE));
    spriteSize = TILE_SIZE*scale;
    sprite[0].setScale(scale,scale);
    sprite[1].setScale(scale,scale);
    txtVec[0].loadFromFile("txt.jpg", sf::IntRect(0, 0, 16, 16));
    txtVec[1].loadFromFile("txt.jpg", sf::IntRect(16, 0, 16, 16));
    txtVec[2].loadFromFile("head.png", sf::IntRect(0, 0, 16, 16));
    txtVec[3].loadFromFile("body.png", sf::IntRect(0, 0, 16, 16));
    txtVec[4].loadFromFile("barrier.png", sf::IntRect(0, 0, 16, 16));
    txtVec[5].loadFromFile("apple.png", sf::IntRect(0, 0, 16, 16));
    txtVec[6].loadFromFile("logo.png");
    txtVec[7].loadFromFile("dif.png");
    txtVec[8].loadFromFile("easy.png");
    txtVec[9].loadFromFile("medium.png");
    txtVec[10].loadFromFile("hard.png");
}

void MSSFMLView::drawField(sf::RenderWindow &window, int i, int j) {
    if(i%2==0) {
        if(j%2==0) {
            sprite[0].setTexture(txtVec[0]);
        } else {
            sprite[0].setTexture(txtVec[1]);
        }
    } else {
        if(j%2==0) {
            sprite[0].setTexture(txtVec[1]);
        } else {
            sprite[0].setTexture(txtVec[0]);
        }
    }
    sprite[0].setPosition(j*spriteSize, i*spriteSize);
    window.draw(sprite[0]);
}

void MSSFMLView::draw(sf::RenderWindow &window) {
    if(mssnakeBoard.getDif()==MENU) {
        window.clear(sf::Color::Green);
        sprite[2].setTexture(txtVec[6]);
        sprite[2].setPosition(131, 50);
        window.draw(sprite[2]);
        sprite[3].setTexture(txtVec[7]);
        sprite[3].setPosition(202,250);
        window.draw(sprite[3]);
        sprite[4].setTexture(txtVec[8]);
        sprite[4].setPosition(320, 350);
        window.draw(sprite[4]);
        sprite[5].setTexture(txtVec[9]);
        sprite[5].setPosition(320, 400);
        window.draw(sprite[5]);
        sprite[6].setTexture(txtVec[10]);
        sprite[6].setPosition(320, 450);
        window.draw(sprite[6]);
    } else {
        for(int i=0;i<BOARD_SIZE;i++) {
            for(int j=0;j<BOARD_SIZE;j++) {
                drawField(window,i,j);
                switch(mssnakeBoard.getFieldInfo(j,i)) {
                    case 'b':
                        sprite[1].setTexture(txtVec[4]);
                        sprite[1].setPosition(j*spriteSize,i*spriteSize);
                        window.draw(sprite[1]);
                    break;
                    case 'f':
                        sprite[1].setTexture(txtVec[5]);
                        sprite[1].setPosition(j*spriteSize,i*spriteSize);
                        window.draw(sprite[1]);
                    break;
                    case 's':
                        if(mssnake.getSnakeHeadX()==j && mssnake.getSnakeHeadY()==i) {
                            sprite[1].setTexture(txtVec[2]);
                            sprite[1].setPosition(j*spriteSize,i*spriteSize);
                            window.draw(sprite[1]);
                        } else {
                            sprite[1].setTexture(txtVec[3]);
                            sprite[1].setPosition(j*spriteSize,i*spriteSize);
                            window.draw(sprite[1]);
                        }
                    break;
                }
            }
        }
    }
}

void MSSFMLView::exit(sf::RenderWindow &window, sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
}

void MSSFMLView::controls(sf::Event event) {
    if(mssnakeBoard.getDif()==MENU) {
        if(event.key.code == sf::Keyboard::Num3) {
            mssnakeBoard.setDif(HARD);
        }
        if(event.key.code == sf::Keyboard::Num2) {
            mssnakeBoard.setDif(MEDIUM);
        }
        if(event.key.code == sf::Keyboard::Num1) {
            mssnakeBoard.setDif(EASY);
        }
    } else {
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::W) {
                if(mssnake.isLegal(TOP)==1) {
                    mssnake.changeDir(TOP);
                }
            }
            if(event.key.code == sf::Keyboard::S) {
                if(mssnake.isLegal(DOWN)==1) {
                    mssnake.changeDir(DOWN);
                }
            }
            if(event.key.code == sf::Keyboard::A) {
                if(mssnake.isLegal(LEFT)==1) {
                    mssnake.changeDir(LEFT);
                }
            }
            if(event.key.code == sf::Keyboard::D) {
                if(mssnake.isLegal(RIGHT)==1) {
                    mssnake.changeDir(RIGHT);
                }
            }
        }
    }
}