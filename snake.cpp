#include "snake.h"

field::field() {
    hasFood = 0;
    hasBarrier = 0;
    hasSnake = 0;
}








snakeBoard::snakeBoard(gameMode mode) : msmode(mode) {
    boardSize = BOARD_SIZE;
    boardArr.resize(BOARD_SIZE);
    for(int i=0;i<boardSize;i++) {
        boardArr[i].resize(BOARD_SIZE);
    }
    toggleBarrier(5,5);
    toggleFood(2,2);
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
    snakeBody.push_back(newElem);
    length++;
}

void snake::snakeInit() {
    snakeElem initElem;
    initElem.setPos(BOARD_SIZE/2,BOARD_SIZE/2);
    snakeBody.push_back(initElem);
    msboard.toggleSnake(BOARD_SIZE/2,BOARD_SIZE/2);
    initElem.setPos((BOARD_SIZE/2),(BOARD_SIZE/2)+1);
    snakeBody.push_back(initElem);
    msboard.toggleSnake((BOARD_SIZE/2),(BOARD_SIZE/2)+1);
    length = snakeBody.size()-1;
}

void snake::snakePop() {
    snakeBody.pop_back();
    length--;
}

void snake::updateSnake() {
    if(msboard.checkFood(snakeBody[0].getElemX(), snakeBody[0].getElemY())==1) {
        msboard.toggleFood(snakeBody[0].getElemX(), snakeBody[0].getElemY());
        snakePush();
    }
    if(msboard.checkBarrier(snakeBody[0].getElemX(), snakeBody[0].getElemY())==1) {
        snakeState=0;
    }
}

int snake::isLegal(dir direction) const {
    if((direction==TOP && snakeBody[0].getDirection()!=DOWN) || (direction==RIGHT && snakeBody[0].getDirection()!=LEFT) || (direction==DOWN && snakeBody[0].getDirection()!=TOP) || (direction==LEFT && snakeBody[0].getDirection()!=RIGHT)) {
        return 1;
    } else {
        return 0;
    }
}

void snake::move(dir direction) {
    msboard.toggleSnake(snakeBody.back().getElemX(),snakeBody.back().getElemY());
    for(int i=snakeBody.size()-1;i>0;i--) {
        snakeBody[i].setPos(snakeBody[i-1].getElemX(),snakeBody[i-1].getElemY());
        std::cout << "element " << i << "x " << snakeBody[i].getElemX() << "y " << snakeBody[i].getElemY() << std::endl;
    }
    switch(direction) {
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
    updateSnake();
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

void MSSFMLView::handleExit(sf::RenderWindow &window, sf::Event event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
}

void MSSFMLView::handleClick(sf::RenderWindow &window, sf::Event event) {
    if(event.type == sf::Event::KeyPressed) {
        if(event.key.code == sf::Keyboard::W) {
            if(mssnake.isLegal(TOP)==1) {
                mssnake.move(TOP);
            }
        }
        if(event.key.code == sf::Keyboard::S) {
            if(mssnake.isLegal(DOWN)==1) {
                mssnake.move(DOWN);
            }
        }
        if(event.key.code == sf::Keyboard::A) {
            if(mssnake.isLegal(LEFT)==1) {
                mssnake.move(LEFT);
            }
        }
        if(event.key.code == sf::Keyboard::D) {
            if(mssnake.isLegal(RIGHT)==1) {
                mssnake.move(RIGHT);
            }
        }
    }
}