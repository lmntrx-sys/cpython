/* 
The Visitor design pattern consists of several key components that work together to enable its functionality. Here’s a breakdown of these components

Visitor Interface: This interface declares a visit method for each type of element in the object structure. Each method is designed to handle a specific element type.

Concrete Visitor: This class implements the Visitor interface and provides the specific behavior for each visit method. It contains the logic for the operations that need to be performed on the elements.

Element Interface: This interface defines an accept method that takes a visitor as an argument. This method allows the visitor to visit the concrete elements.

Concrete Elements: These classes implement the Element interface and represent the various types of objects in the structure. Each concrete element defines how it accepts a visitor by calling the corresponding method on the visitor.

Object Structure: This is the collection of elements (the concrete elements) that the visitor will operate on. It often includes methods to add, remove, and retrieve elements.

source: https://www.geeksforgeeks.org/system-design/visitor-design-pattern/
*/

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

class ShapeVisitor{
    public:
        virtual void visit(Circle circle) = 0;
        virtual void visit(Square square) = 0;
        virtual void visit(Triangle triangle) = 0;
};

class ShapeVisitor;
class Shape {
    public:
        virtual void accept(ShapeVisitor& visitor) = 0;
};

class Circle : public Shape {
    void accept(ShapeVisitor* visitor) override {
        visitor->visit(this);
    }
};

class Square : public Shape {
    void accept(ShapeVisitor* visitor) override {
        visitor->visit(this);
    }
};

class Triangle : public Shape {
    void accept(ShapeVisitor* visitor) override {
        visitor->visit(this);
    }
};

class AreaCalculator : public ShapeVisitor { 
    private:
        double total_area = 0;
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
                total_area += (baseOfTriangle * HeightOfTriangle) / 2;
            }

            double getTotalArea(){
                return total_area;
            }
};


int main(){
    vector<Shape*> shapes;
    shapes.push_back(new Circle());
    shapes.push_back(new Square());
    shapes.push_back(new Triangle());

    AreaCalculator areaCalculator;
    for (Shape* shape : shapes) {
        shape->accept(&areaCalculator);
    }

    cout << "Total area: " << areaCalculator.getTotalArea() << endl;

    for (Shape* shape : shapes) {
        delete shape;
    }
    return 0;
}
