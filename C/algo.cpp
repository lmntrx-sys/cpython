
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

// 1. Forward declarations so the classes know each other exist
class Circle;
class Square;
class Triangle;

// 2. Define the Visitor interface
class ShapeVisitor {
public:
    virtual void visit(Circle* circle) = 0;
    virtual void visit(Square* square) = 0;
    virtual void visit(Triangle* triangle) = 0;
    virtual ~ShapeVisitor() {} // Good practice to have a virtual destructor
};

// 3. Define the Base Shape class
class Shape {
public:
    virtual void accept(ShapeVisitor& visitor) = 0;
    virtual ~Shape() {}
};

// 4. Define Concrete Shapes
class Circle : public Shape {
public:
    void accept(ShapeVisitor& visitor) override {
        visitor.visit(this);
    }
};

class Square : public Shape {
public:
    void accept(ShapeVisitor& visitor) override {
        visitor.visit(this);
    }
};

class Triangle : public Shape {
public:
    void accept(ShapeVisitor& visitor) override {
        visitor.visit(this);
    }
};

// 5. Define the Concrete Visitor (AreaCalculator)
class AreaCalculator : public ShapeVisitor { 
private:
    double total_area = 0;
    
    // Hardcoded values for demonstration (usually these would be members of the shapes)
    double radius_of_circle = 15;
    double sideofSquare = 31;
    double baseOfTriangle = 34;
    double HeightOfTriangle = 45;

public: 
    void visit(Circle* circle) override {
        total_area += M_PI * pow(radius_of_circle, 2);
    }

    void visit(Square* square) override {
        total_area += pow(sideofSquare, 2);
    }

    void visit(Triangle* triangle) override {
        total_area += (baseOfTriangle * HeightOfTriangle) / 2.0;
    }

    double getTotalArea() {
        return total_area;
    }
};

// 6. Main execution
int main() {
    vector<Shape*> shapes;
    shapes.push_back(new Circle());
    shapes.push_back(new Square());
    shapes.push_back(new Triangle());

    AreaCalculator areaCalculator;
    for (Shape* shape : shapes) {
        shape->accept(areaCalculator);
    }

    cout << "Total area: " << areaCalculator.getTotalArea() << endl;

    // Clean up memory
    for (Shape* shape : shapes) {
        delete shape;
    }

    return 0;
}