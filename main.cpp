#include <iostream>
#include <vector>
using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Shape{
protected:
    string id;

public:
    Shape(string id) : id(id) {}
    virtual ~Shape() {}
    virtual void draw(vector<vector<char>>& grid) const = 0;
    virtual string getDescription() const = 0;
};

class Board{
private:
    vector<vector<char>> grid;
    vector<shared_ptr<Shape>> shapes;

public:
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

    vector<shared_ptr<Shape>>& getShapes() {
        return shapes;
    }

    void drawBoard() {
        for (auto& row : grid) {
            for (char c : row) {
                std::cout << c;
            }
            std::cout << "\n";
        }

        for (const auto &shape : shapes) {
            shape->draw(grid);
        }

        for (const auto &row : grid) {
            for (char c : row) {
                cout << c;
            }
            cout << "\n";
        }
    }

    void addShape(shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }

    void clearBoard() {}

};

class Triangle : public Shape{
private:
    int x, y, height;

public:
    Triangle(string id, int x, int y, int height) : Shape(id), x(x), y(y), height(height) {}

    void draw(vector<vector<char>>& grid) const override {

    }

    string getDescription() const override {
        return id + " triangle height=" + to_string(height) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
    }
};

class Circle : public Shape{
private:
    int x, y, radius;

public:
    Circle(string id, int x, int y, int radius) : Shape(id), x(x), y(y), radius(radius) {}

    void draw(vector<vector<char>>& grid) const override {

    }

    string getDescription() const override {
        return id + "circle radius=" + to_string(radius) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
    }
};

class Rectangle : public Shape{
private:
    int x, y, width, height;

public:
    Rectangle(string id, int x, int y, int width, int height) : Shape(id), x(x), y(y), width(width), height(height) {}

    void draw(vector<vector<char>>& grid) const override {

    }

    string getDescription() const override {
        return id + " rectangle width=" + to_string(width) + " height=" + to_string(height) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
    }

};

class Line: public Shape {
private:
    int x1, y1, x2, y2;

public:
    Line(string id, int x1, int y1, int x2, int y2) : Shape(id), x1(x1), y1(y1), x2(x2), y2(y2) {}

    void draw(vector<vector<char>>& grid) const override {

    }

    string getDescription() const override {
        return id + " line from (" + to_string(x1) + ", " + to_string(y1) + ") to (" + to_string(x2) + ", " + to_string(y2) + ")";
    }
};

class UserInterface{
private:
    Board board;

public:
    void run(){
        string command;
        cout << """Welcome to the Shapes Blackboard. Choose the command from the list:\n"
                  "1. draw\n"
                  "2. list\n"
                  "3. shapes\n"
                  "4. add\n"
                  "5. undo\n"
                  "6. clear\n"
                  "7. save\n"
                  "8. load\n"
                  "9. exit\n""" << endl;

        while (true){
            cout << ">" << endl;
            getline(cin, command);

            int commandNumber = -1;
            if (command == "draw") commandNumber = 1;
            else if (command == "list") commandNumber = 2;
            else if (command == "shapes") commandNumber = 3;
            else if (command == "add") commandNumber = 4;
            else if (command == "undo") commandNumber = 5;
            else if (command == "clear") commandNumber = 6;
            else if (command == "save") commandNumber = 7;
            else if (command == "load") commandNumber = 8;
            else if (command == "exit") commandNumber = 9;

            switch (commandNumber) {
                case 1:
                    drawBoard();
                    break;
                case 2:
                    listOfShapes();
                    break;
                case 3:
                    availableShapes();
                    break;
                case 4:
                    add();
                    break;
                case 5:
                    undo();
                    break;
                case 6:
                    clear();
                    break;
                case 7:
                    save();
                    break;
                case 8:
                    load();
                    break;
                case 9:
                    break;
            }
        }

    }

private:
    void drawBoard() {
        board.drawBoard();
    }


    void listOfShapes(){
        cout << "List of shapes:" << endl;
        for (const auto& shape : board.getShapes()) {
            cout << shape->getDescription() << endl;
        }
    }

    void availableShapes() {
        cout << "Available shapes:\n";
        cout << "1. Triangle - Parameters: x y height\n";
        cout << "2. Circle - Parameters: x y radius\n";
        cout << "3. Rectangle - Parameters: x y width height\n";
        cout << "4. Line - Parameters: x1 y1 x2 y2\n";
    }

    void add(){
        string shapeType;
        static int shapeCounter = 0;
        cout << "Enter shape type (triangle, circle, rectangle, line): " << endl;
        cin >> shapeType;

        string id = "Shape" + to_string(shapeCounter++);

    }

    void undo(){

    }

    void clear() {

    }

    void save(){

    }

    void load(){

    }

};

int main() {
    UserInterface ui;
    ui.run();
    return 0;
}



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



