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
    string getId() const { return id; }
    virtual bool containsPoint(int x, int y) const = 0;

};

class Board{
private:
    vector<vector<char>> grid;
    vector<shared_ptr<Shape>> shapes;
    shared_ptr<Shape> selectedShape = nullptr;

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

    void selectByCoord(int x, int y) {
        for (const auto& shape : shapes) {
            if (shape->containsPoint(x, y)) {
                selectedShape = shape;
                cout << "Shape selected: " << selectedShape->getDescription() << endl;
                return;
            }
        }
        cout << "No shape found." << endl;
    }

    void selectById(const string& id) {
        auto it = find_if(shapes.begin(), shapes.end(), [&](shared_ptr<Shape>shape) {
            return shape->getId() == id;
        });

        if (it != shapes.end()) {
            selectedShape = *it;
            cout << "Shape selected: " << selectedShape->getDescription() << endl;
        }
        else {
            cout << "Shape not found." << endl;
        }
    }

    shared_ptr<Shape> getSelectedShape() const {
        return selectedShape;
    }
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

    bool containsPoint(int px, int py) const override {
        if (py < y || py >= y + height) return false;
        int dx = py - y;
        return px >= x - dx && px <= x + dx;
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
        return id + " circle " + to_string(radius) + " " + to_string(x) + " " + to_string(y) + " ";
    }

    bool containsPoint(int px, int py) const override {
        int dx = px - x;
        int dy = py - y;
        return dx * dx + dy * dy <= radius * radius;
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
        return id + " rectangle " + to_string(width) + " " + to_string(height) + " " + to_string(x) + " " + to_string(y) + " ";
    }

    bool containsPoint(int px, int py) const override {
        return px >= x && px < x + width && py >= y && py < y + height;
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

    bool containsPoint(int px, int py) const override {
        return px >= x && px < x + sideLength && py >= y && py < y + sideLength;
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
                "9. select\n"
                "10. exit\n""" << endl;

        while (true) {
            cout << ">";
            getline(cin, command);

            stringstream ss(command);
            string cmd;
            ss >> cmd;

            if (cmd == "draw") {
                drawBoard();
            } else if (cmd == "list") {
                listOfShapes();
            } else if (cmd == "shapes") {
                availableShapes();
            } else if (cmd == "add") {
                add(ss);
            } else if (cmd == "undo") {
                undo();
            } else if (cmd == "clear") {
                clear();
            } else if (cmd == "save") {
                string filename;
                ss >> filename;
                save(filename);
            } else if (cmd == "load") {
                string filename;
                ss >> filename;
                load(filename);
            } else if (cmd == "select") {
                select(ss);
            } else if (cmd == "exit") {
                return;
            } else {
                cout << "Invalid command!" << endl;
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
        cout << "4. Square - Parameters: x y sideLength\n";
    }

    void add(stringstream &ss) {
        string shapeType;
        static int shapeCounter = 1;
        ss >> shapeType;

        string id = "Shape" + to_string(shapeCounter++);
        bool isDuplicate = false;

        if (shapeType == "triangle") {
            int x, y, height;
            ss >> x >> y >> height;

            if (!isDuplicate) {
                auto triangle = make_shared<Triangle>(id, x, y, height);
                board.addShape(triangle);
                cout << "Triangle added." << endl;
            }
        } else if (shapeType == "circle") {
            int x, y, radius;
            ss >> x >> y >> radius;

            if (!isDuplicate) {
                auto circle = make_shared<Circle>(id, x, y, radius);
                board.addShape(circle);
                cout << "Circle added." << endl;
            }
        } else if (shapeType == "rectangle") {
            int x, y, width, height;
            ss >> x >> y >> width >> height;

            if (!isDuplicate) {
                auto rectangle = make_shared<Rectangle>(id, x, y, width, height);
                board.addShape(rectangle);
                cout << "Rectangle added." << endl;
            }
        } else if (shapeType == "square") {
            int x, y, sideLength;
            ss >> x >> y >> sideLength;

            if (!isDuplicate) {
                auto square = make_shared<Square>(id, x, y, sideLength);
                board.addShape(square);
                cout << "Square added." << endl;
            }
        } else {
            cout << "Invalid shape type!" << endl;
        }
    }

    void undo() {
        if (!board.getShapes().empty()) {
            board.getShapes().pop_back();
            cout << "Last added shape removed." << endl;
        } else {
            cout << "No shapes to undo." << endl;
        }
    }

    void clear() {
        board.getShapes().clear();
        cout << "Board cleared." << endl;
    }

    void save(const string &filename) {
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

    void load(const string &filename) {
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
                } else if (shapeType == "circle") {
                    iss >> x >> y >> size1;
                    if (!iss.fail()) {
                        auto circle = make_shared<Circle>(id, x, y, size1);
                        board.addShape(circle);
                    } else {
                        cout << "Error parsing circle: " << line << endl;
                    }
                } else if (shapeType == "rectangle") {
                    iss >> x >> y >> size1 >> size2;
                    if (!iss.fail()) {
                        auto rectangle = make_shared<Rectangle>(id, x, y, size1, size2);
                        board.addShape(rectangle);
                    } else {
                        cout << "Error parsing rectangle: " << line << endl;
                    }
                } else if (shapeType == "square") {
                    iss >> x >> y >> size1;
                    if (!iss.fail()) {
                        auto square = make_shared<Square>(id, x, y, size1);
                        board.addShape(square);
                    } else {
                        cout << "Error parsing square: " << line << endl;
                    }
                } else {
                    cout << "Unknown shape type: " << shapeType << endl;
                }
            }

            inFile.close();
            cout << "Board loaded from " << filename << endl;
        } else {
            cout << "Error opening file for loading." << endl;
        }
    }

private:
    void select(stringstream &ss) {
        string idOrCoord;
        ss >> idOrCoord;
        if (isdigit(idOrCoord[0])) {
            int x = stoi(idOrCoord);
            int y;
            ss >> y;
            board.selectByCoord(x, y);
        }
        else {
            board.selectById(idOrCoord);
        }
    }
};

int main() {
    UserInterface ui;
    ui.run();
    return 0;
}


