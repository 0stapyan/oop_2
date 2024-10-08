#include <iostream>
#include <fstream>
#include <sstream>
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

        cout << "+";
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            cout << "-";
        }
        cout << "+" << endl;

        for (int row = 0; row < BOARD_HEIGHT; ++row) {
            cout << "|";
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                cout << grid[row][col];
            }
            cout << "|" << endl;
        }

        cout << "+";
        for (int i = 0; i < BOARD_WIDTH; ++i) {
            cout << "-";
        }
        cout << "+" << endl;
    }

    void addShape(shared_ptr<Shape> shape) {
        shapes.push_back(shape);
    }

};

class Triangle : public Shape{
private:
    int x, y, height;

public:
    Triangle(string id, int x, int y, int height) : Shape(id), x(x), y(y), height(height) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getHeight() const { return height; }

    void draw(vector<vector<char>>& grid) const override {
        if (height <= 0) return;

        for (int i = 0; i < height; i++) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i;

            if (posY < BOARD_HEIGHT && posY >= 0) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH) {
                    grid[posY][leftMost] = '*';
                }
                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost) {  // Crop right side
                    grid[posY][rightMost] = '*';
                }
            }
        }

        for (int j = 0; j < 2 * height - 1; j++) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY >= 0 && baseY < BOARD_HEIGHT) {  // Crop the base
                grid[baseY][baseX] = '*';
            }
        }
    }

    string getDescription() const override {
        return id + " triangle " + to_string(height) + " " + to_string(x) + " " + to_string(y) + " ";
    }
};

class Circle : public Shape{
private:
    int x, y, radius;

public:
    Circle(string id, int x, int y, int radius) : Shape(id), x(x), y(y), radius(radius) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getRadius() const { return radius; }

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
        return id + " circle " + to_string(radius) + " " + to_string(x) + " " + to_string(y) + " ";
    }
};

class Rectangle : public Shape{
private:
    int x, y, width, height;

public:
    Rectangle(string id, int x, int y, int width, int height) : Shape(id), x(x), y(y), width(width), height(height) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

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
        return id + " rectangle " + to_string(width) + " " + to_string(height) + " " + to_string(x) + " " + to_string(y) + " ";
    }

};

class Square : public Shape {
private:
    int x, y, sideLength;

public:
    Square(string id, int x, int y, int sideLength) : Shape(id), x(x), y(y), sideLength(sideLength) {}

    int getX() const { return x; }
    int getY() const { return y; }
    int getSideLength() const { return sideLength; }

    void draw(vector<vector<char>>& grid) const override {
        for (int i = 0; i < sideLength; ++i) {
            int topX = x + i;
            int topY = y;
            int bottomY = y + sideLength - 1;

            if (topX >= 0 && topX < BOARD_WIDTH) {
                if (topY >= 0 && topY < BOARD_HEIGHT) {
                    grid[topY][topX] = '*';
                }
                if (bottomY >= 0 && bottomY < BOARD_HEIGHT) {
                    grid[bottomY][topX] = '*';
                }
            }
        }

        for (int j = 0; j < sideLength; ++j) {
            int leftX = x;
            int rightX = x + sideLength - 1;
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
        return id + " square " + to_string(sideLength) + " " + to_string(x) + " " + to_string(y) + " ";
    }
};

class UserInterface {
private:
    Board board;

public:
    void run() {
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

        while (true) {
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
                    return;
            }
        }

    }

private:
    void drawBoard() {
        board.drawBoard();
    }


    void listOfShapes() {
        cout << "List of shapes:" << endl;
        for (const auto &shape: board.getShapes()) {
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

    void add() {
        string shapeType;
        static int shapeCounter = 1;
        cout << "Enter shape type (triangle, circle, rectangle, square): " << endl;
        cin >> shapeType;

        string id = "Shape" + to_string(shapeCounter++);
        bool isDuplicate = false;

        if (shapeType == "triangle") {
            int x, y, height;
            cout << "Enter x, y, height: ";
            cin >> x >> y >> height;

            // Check for duplicates
            for (const auto &shape: board.getShapes()) {
                auto triangle = dynamic_pointer_cast<Triangle>(shape);
                if (triangle && triangle->getX() == x && triangle->getY() == y && triangle->getHeight() == height) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                auto triangle = make_shared<Triangle>(id, x, y, height);
                board.addShape(triangle);
                cout << "Triangle added." << endl;
            } else {
                cout << "A triangle with the same parameters already exists at this position!" << endl;
            }
        } else if (shapeType == "circle") {
            int x, y, radius;
            cout << "Enter x, y, radius: ";
            cin >> x >> y >> radius;

            // Check for duplicates
            for (const auto &shape: board.getShapes()) {
                auto circle = dynamic_pointer_cast<Circle>(shape);
                if (circle && circle->getX() == x && circle->getY() == y && circle->getRadius() == radius) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                auto circle = make_shared<Circle>(id, x, y, radius);
                board.addShape(circle);
                cout << "Circle added." << endl;
            } else {
                cout << "A circle with the same parameters already exists at this position!" << endl;
            }
        } else if (shapeType == "rectangle") {
            int x, y, width, height;
            cout << "Enter x, y, width, height: ";
            cin >> x >> y >> width >> height;

            // Check for duplicates
            for (const auto &shape: board.getShapes()) {
                auto rectangle = dynamic_pointer_cast<Rectangle>(shape);
                if (rectangle && rectangle->getX() == x && rectangle->getY() == y && rectangle->getWidth() == width &&
                    rectangle->getHeight() == height) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                auto rectangle = make_shared<Rectangle>(id, x, y, width, height);
                board.addShape(rectangle);
                cout << "Rectangle added." << endl;
            } else {
                cout << "A rectangle with the same parameters already exists at this position!" << endl;
            }
        } else if (shapeType == "square") {
            int x, y, sideLength;
            cout << "Enter x, y, sideLength: ";
            cin >> x >> y >> sideLength;

            // Check for duplicates
            for (const auto &shape: board.getShapes()) {
                auto square = dynamic_pointer_cast<Square>(shape);
                if (square && square->getX() == x && square->getY() == y && square->getSideLength() == sideLength) {
                    isDuplicate = true;
                    break;
                }
            }

            if (!isDuplicate) {
                auto square = make_shared<Square>(id, x, y, sideLength);
                board.addShape(square);
                cout << "Square added." << endl;
            } else {
                cout << "A square with the same parameters already exists at this position!" << endl;
            }
        }
    }
    void undo() {
        if (!board.getShapes().empty()) {
            board.getShapes().pop_back();
            cout << "Last added shape removed." << endl;
        } else {
            cout << "No shapesto undo." << endl;
        }
    }

    void clear() {
        board.getShapes().clear();
        cout << "Board cleared." << endl;
    }

    void save() {
        string filename;
        cout << "Enter filename to save: ";
        cin >> filename;

        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (const auto &shape: board.getShapes()) {
                outFile << shape->getDescription() << endl;
            }

            outFile.close();
            cout << "Board saved to " << filename << endl;
        } else {
            cout << "Error opening file for saving." << endl;
        }
    }

    void load() {
        string filename;
        cout << "Enter filename to load: ";
        cin >> filename;

        ifstream inFile(filename);
        if (inFile.is_open()) {
            board.getShapes().clear();
            string line;

            while (getline(inFile, line)) {
                istringstream iss(line);
                string shapeType, id;
                int x, y, size1, size2;

                iss >> id >> shapeType;

                if (shapeType == "triangle") {
                    iss >> x >> y >> size1;
                    if (!iss.fail()) {
                        auto triangle = make_shared<Triangle>(id, x, y, size1);
                        board.addShape(triangle);
                    } else {
                        cout << "Error parsing triangle: " << line << endl;
                    }
                }
                else if (shapeType == "circle") {
                    iss >> x >> y >> size1;
                    if (!iss.fail()) {
                        auto circle = make_shared<Circle>(id, x, y, size1);
                        board.addShape(circle);
                    } else {
                        cout << "Error parsing circle: " << line << endl;
                    }
                }
                else if (shapeType == "rectangle") {
                    iss >> x >> y >> size1 >> size2;
                    if (!iss.fail()) {
                        auto rectangle = make_shared<Rectangle>(id, x, y, size1, size2);
                        board.addShape(rectangle);
                    } else {
                        cout << "Error parsing rectangle: " << line << endl;
                    }
                }
                else if (shapeType == "square") {
                    iss >> x >> y >> size1;
                    if (!iss.fail()) {
                        auto square = make_shared<Square>(id, x, y, size1);
                        board.addShape(square);
                    } else {
                        cout << "Error parsing square: " << line << endl;
                    }
                }
                else {
                    cout << "Unknown shape type: " << shapeType << endl;
                }
            }

            inFile.close();
            cout << "Board loaded from " << filename << endl;

            board.drawBoard();
        } else {
            cout << "Error opening file for loading." << endl;
        }
    }

};

int main() {
    UserInterface ui;
    ui.run();
    return 0;
}


