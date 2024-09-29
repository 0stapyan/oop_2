#include <iostream>
#include <vector>
using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Board{
private:
    vector<vector<char>> grid;

public:
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

};

class Shape{
public:
    virtual ~Shape() {}
};

class Triangle : public Shape{
private:
    int x, y, height;

public:
    Triangle(int x, int y, int height) : x(x), y(y), height(height) {}
};

class Circle : public Shape{
private:
    int x, y, radius;

public:
    Circle(int x, int y, int radius) : x(x), y(y), radius(radius) {}
};

class Rectangle : public Shape{
private:
    int x, y, width, height;

public:
    Rectangle(int x, int y, int width, int height) : x(x), y(y), width(width), height(height) {}
};

class UserInterface{
private:
    Board board;

public:
    void run(){

    }

private:
    void listOfShapes(){

    }

    void availableShapes(){

    }

    void add(){

    }

    void undo(){

    }

    void save(){

    }

    void load(){

    }

};

int main() {
    Board board;
    return 0;
}


//    void print() {
//        for (auto &row: grid) {
//            for (char c: row) {
//                std::cout << c;
//            }
//            std::cout << "\n";
//        }
//    }
//    void drawTriangle(int x, int y, int height) {
//        if (height <= 0) return;
//
//        for (int i = 0; i < height; i++) {
//            int leftMost = x - i;
//            int rightMost = x + i;
//            int posY = y + i;
//
//            if (posY < BOARD_HEIGHT) {
//                if (leftMost >= 0 && leftMost < BOARD_WIDTH) {
//                    grid[posY][leftMost] = '*';
//                }
//                if (rightMost >= 0 && leftMost < BOARD_WIDTH && leftMost != rightMost){
//                    grid[posY][rightMost] = '*';
//                }
//            }
//        }
//        for (int j = 0; j < 2 * height - 1; j++){
//            int baseX = x - height + 1 + j;
//            int baseY = y + height - 1;
//            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT){
//                grid[baseY][baseX] = '*';
//            }
//        }
//    }
//};
//
//int main() {
//    Board board;
//    board.drawTriangle(10, 1, 5);
//    board.print();
//    return 0;
//}

