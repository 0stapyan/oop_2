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
        for (auto &row : grid) {
            fill(row.begin(), row.end(), ' ');
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
        if (height <= 0) return;

        for (int i = 0; i < height; i++) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;

            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH) {
                    grid[posY][leftMost] = '*';
                }
                if (rightMost >=0 && rightMost < BOARD_WIDTH && leftMost != rightMost) {
                    grid[posY][rightMost] = '*';
                }
            }
        }

        for (int j = 0; j < 2 * height - 1; j++) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {
                grid[baseY][baseX] = '*';
            }
        }

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
        int x0 = x;
        int y0 = y;

        int x = radius;
        int y = 0;
        int decisionOver2 = 1 - x;

        while (x >= y) {
            if (x0 + x >= 0 && x0 + x < BOARD_WIDTH && y0 + y >= 0 && y0 + y < BOARD_HEIGHT)
                grid[y0 + y][x0 + x] = '*';
            if (x0 + y >= 0 && x0 + y < BOARD_WIDTH && y0 + x >= 0 && y0 + x < BOARD_HEIGHT)
                grid[y0 + x][x0 + y] = '*';
            if (x0 - y >= 0 && x0 - y < BOARD_WIDTH && y0 + x >= 0 && y0 + x < BOARD_HEIGHT)
                grid[y0 + x][x0 - y] = '*';
            if (x0 - x >= 0 && x0 - x < BOARD_WIDTH && y0 + y >= 0 && y0 + y < BOARD_HEIGHT)
                grid[y0 + y][x0 - x] = '*';
            if (x0 - x >= 0 && x0 - x < BOARD_WIDTH && y0 - y >= 0 && y0 - y < BOARD_HEIGHT)
                grid[y0 - y][x0 - x] = '*';
            if (x0 - y >= 0 && x0 - y < BOARD_WIDTH && y0 - x >= 0 && y0 - x < BOARD_HEIGHT)
                grid[y0 - x][x0 - y] = '*';
            if (x0 + y >= 0 && x0 + y < BOARD_WIDTH && y0 - x >= 0 && y0 - x < BOARD_HEIGHT)
                grid[y0 - x][x0 + y] = '*';
            if (x0 + x >= 0 && x0 + x < BOARD_WIDTH && y0 - y >= 0 && y0 - y < BOARD_HEIGHT)
                grid[y0 - y][x0 + x] = '*';

            y++;
            if (decisionOver2 <= 0) {
                decisionOver2 += 2 * y + 1;
            } else {
                x--;
                decisionOver2 += 2 * (y - x) + 1;
            }
        }
    }

    string getDescription() const override {
        return id + " circle radius=" + to_string(radius) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
    }
};

class Rectangle : public Shape{
private:
    int x, y, width, height;

public:
    Rectangle(string id, int x, int y, int width, int height) : Shape(id), x(x), y(y), width(width), height(height) {}

    void draw(vector<vector<char>>& grid) const override {
        for (int i = 0; i < width; i++) {
            int topX = x + i;
            int topY = y;
            int bottomY = y + height - 1;
            if (topX >= 0 && topX < BOARD_WIDTH) {
                if (topY >= 0 && topY < BOARD_HEIGHT) {
                    grid[topY][topX] = '*';
                }
                if (bottomY >= 0 && bottomY < BOARD_HEIGHT) {
                    grid[bottomY][topX] = '*';
                }
            }
        }

        for (int j = 0; j < height; j++) {
            int leftX = x;
            int rightX = x + width - 1;
            int drawY = y + j;
            if (drawY >= 0 && drawY < BOARD_HEIGHT) {
                if (leftX >= 0 && leftX < BOARD_WIDTH) {
                    grid[drawY][leftX] = '*';
                }
                if (rightX >= 0 && rightX < BOARD_WIDTH) {
                    grid[drawY][rightX] = '*';
                }
            }
        }

    }

    string getDescription() const override {
        return id + " rectangle width=" + to_string(width) + " height=" + to_string(height) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
    }

};

class Square : public Shape {
private:
    int x, y, sideLength;

public:
    Square(string id, int x, int y, int sideLength) : Shape(id), x(x), y(y), sideLength(sideLength) {}

    void draw(vector<vector<char>>& grid) const override {
        for (int i = 0; i < sideLength; ++i) {
            int topX = x + i;
            int topY = y;
            int bottomY = y + sideLength - 1;

            if (topX >= 0 && topX < BOARD_WIDTH && topY >= 0 && topY < BOARD_HEIGHT) {
                grid[topY][topX] = '*';
            }

            if (topX >= 0 && topX < BOARD_WIDTH && bottomY >= 0 && bottomY < BOARD_HEIGHT) {
                grid[bottomY][topX] = '*';
            }
        }

        for (int j = 0; j < sideLength; ++j) {
            int leftX = x;
            int rightX = x + sideLength - 1;
            int drawY = y + j;

            if (leftX >= 0 && leftX < BOARD_WIDTH && drawY >= 0 && drawY < BOARD_HEIGHT) {
                grid[drawY][leftX] = '*';
            }

            if (rightX >= 0 && rightX < BOARD_WIDTH && drawY >= 0 && drawY < BOARD_HEIGHT) {
                grid[drawY][rightX] = '*';
            }
        }
    }

    string getDescription() const override {
        return id + " square sideLength=" + to_string(sideLength) + " position=(" + to_string(x) + ", " + to_string(y) + ")";
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
        cout << "4. Square - Parameters: x y sideLength\\n";
    }

    void add(){
        string shapeType;
        static int shapeCounter = 1;
        cout << "Enter shape type (triangle, circle, rectangle, square): " << endl;
        cin >> shapeType;

        string id = "Shape" + to_string(shapeCounter++);

        if (shapeType == "triangle") {
            int x, y, height;
            cout << "Enter x, y, height: ";
            cin >> x >> y >> height;

            if (x - height < 0 || x + height >= BOARD_WIDTH || y + height >= BOARD_HEIGHT) {
                cout << "Triangle is out of bounds! Please enter valid coordinates and height." << endl;
                return;
            }

            auto triangle = make_shared<Triangle>(id, x, y, height);
            board.addShape(triangle);
            cout << "Triangle added." << endl;
        }
        else if (shapeType == "circle") {
            int x, y, radius;
            cout << "Enter x, y, radius: ";
            cin >> x >> y >> radius;

            if (x - radius < 0 || x + radius >= BOARD_WIDTH || y - radius < 0 || y + radius >= BOARD_HEIGHT) {
                cout << "Circle is out of bounds! Please enter valid coordinates and radius." << endl;
                return;
            }

            auto circle = make_shared<Circle>(id, x, y, radius);
            board.addShape(circle);
            cout << "Circle added." << endl;
        }
        else if (shapeType == "rectangle") {
            int x, y, width, height;
            cout << "Enter x, y, width, height: ";
            cin >> x >> y >> width >> height;

            if (x < 0 || x + width >= BOARD_WIDTH || y < 0 || y + height >= BOARD_HEIGHT) {
                cout << "Rectangle is out of bounds! Please enter valid coordinates and dimensions." << endl;
                return;
            }

            auto rectangle = make_shared<Rectangle>(id, x, y, width, height);
            board.addShape(rectangle);
            cout << "Rectangle added." << endl;
        }
        else if (shapeType == "square") {
            int x, y, sideLength;
            cout << "Enter x, y, sideLength: ";
            cin >> x >> y >> sideLength;

            if (x < 0 || x + sideLength >= BOARD_WIDTH || y < 0 || y + sideLength >= BOARD_HEIGHT) {
                cout << "Square is out of bounds! Please enter valid coordinates and side length." << endl;
                return;
            }

            auto square = make_shared<Square>(id, x, y, sideLength);
            board.addShape(square);
            cout << "Square added." << endl;
        }
        else {
            cout << "Invalid shape type" << endl;
        }
    }

    void undo(){
        if (!board.getShapes().empty()) {
            board.getShapes().pop_back();
            cout << "Last added shape removed." << endl;
        }
        else {
            cout << "No shapesto undo." << endl;
        }
    }

    void clear() {
        board.getShapes().clear();
        cout << "Board cleared." << endl;
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



