#ifndef MATHUTIL_H
#define MATHUTIL_H

#include <cmath>
#include <iostream>

namespace CMPUT350 {

struct Point2D {
    float x, y;
    Point2D(float x = 0, float y = 0) : x(x), y(y) {}
    double Distance(const Point2D &other) const {
        // Using euclidean distance here
        return std::sqrt((other.x - x) * (other.x - x) + (other.y - y) * (other.y - y));
    }
    Point2D operator+(const Point2D &other) const {
        // Add two points together
        return Point2D(x + other.x, y + other.y);
    }
    Point2D operator+(const float &other) const {
        // Add uniformly by float
        return Point2D(x + other, y + other);
    }
    Point2D operator-(const Point2D &other) const {
        // Subtract another point from this one
        return Point2D(x - other.x, y - other.y);
    }
    Point2D operator-(const float &other) const {
        // Subtract uniformly by float
        return Point2D(x - other, y - other);
    }
    Point2D operator*(const float &scalar) const {
        // Multiply by scalar
        return Point2D(x * scalar, y * scalar);
    }
    Point2D &operator+=(const float &scalar) { //why is this one scalar
        // Change this point and add scalar
        x += scalar;
        y += scalar;
        return *this;
    }
    Point2D &operator+=(const Point2D &other) {
        // Add another point to this point
        x += other.x;
        y += other.y;
        return *this;
    }
    Point2D &operator-=(const Point2D &other) {
        // Subtract another point from this point
        x -= other.x;
        y -= other.y;
        return *this;
    }
    bool operator==(const Point2D &other) const {
        // Check if two points are equal
        return (x == other.x && y == other.y); //Might fail because float though, maybe come back
    }
    Point2D &operator*=(const float &scalar) {
        // Multiply this point by a scalar
        x *= scalar;
        y *= scalar;
        return *this;
    }
    Point2D &operator/=(const float &scalar) {
        // Divide this point by a scalar
        x /= scalar;
        y /= scalar;
        return *this;
    }
    float operator*(const Point2D &other) const {
        // This is the dot product of two points ( point * point )
        return (x * other.x) + (y * other.y);
    }
    float Dot(Point2D b) const {
        // This is the dot product of two points ( point.Dot(point) )
        return (x * b.x) + (y * b.y);
    }
    float Cross(Point2D b) const { // was missing from template but was in project description
        // Cross product between two points ( point.Cross(point) )
        return (x * b.y) - (y * b.x); // This formula with 2d points is just the determinant of a 2x2 matrix
    }
    static float Dot(Point2D a, Point2D b) {
        // This is the dot product of two points ( Point2D::Dot(point, point) )
        return (a.x * b.x) + (a.y * b.y);
    }
    static float Cross(Point2D a, Point2D b) {
        // Cross product between two points ( Point2D::Cross(point, point) )
        return (a.x * b.y) - (a.y * b.x);
    }
    void Normalize() {
        // Makes the magnitude of a point 1 (this is normalization)
        float temp = std::sqrt(x * x + y * y); // This is magnitude formula
        if (temp != 0) { //Guard against normalizing (0,0)
            x = x / temp;
            y = y / temp;
        }
    }
    Point2D Perpendicular() {
        // Returns the perpendicular version (counterclockwise)
        return Point2D(-y,x);
    }
};

static std::ostream &operator<<(std::ostream &os, const Point2D &p) {
    // Converts point into printable text (I chose (a,b) formatting)
    os << "(" << p.x << "," << p.y << ")";
    return os;
}

static Point2D operator*(float number, const Point2D &rhs) {
    // This one is the case where its float * point
    return Point2D(number * rhs.x, number * rhs.y);
}

struct Line {
    Point2D p1, p2;

    Line(Point2D p1 = {0, 0}, Point2D p2 = {0, 0}) : p1(p1), p2(p2) {}
    Line(float x1, float y1, float x2, float y2) : p1(x1, y1), p2(x2, y2) {}
    float Length() const {
        // Uses built in point function
        return p1.Distance(p2);
    }
    Point2D ClosestPoint(const Point2D &p) const {
        // Closest point calculation, first check if endpoints are closest, then circle x line calculation
        // MAKE SURE LINE IS NOT A POINT OR CALCULATION FAILS
        if (p1 == p2) {
            return p1;
        }
        Point2D ac = p - p1; // Quick translation a = p1, b = p2, c = p
        Point2D ab = p2 - p1;
        float projection = (ac * ab) / (ab * ab);
        if (projection < 0) {
            return p1;
        }
        if (projection > 1) { // in class mentioned recalculating with bc, but it's unneeded as if it's bigger than 1, then it's over b's edge
            return p2;
        }
        return p1 + (ab * projection); // Simplified version of the calculation in class
    }
    bool Crosses(Line other, Point2D &crossingPoint) const {
        // Check if two lines cross, and change crossing point
        Point2D ab = p2 - p1; // Translation a = p1, b = p2, x = other.p1, y = other.p2
        Point2D xy = other.p2 - other.p1;
        Point2D ax = other.p1 - p1;
        float ab_cross_xy = ab.Cross(xy);
        if (ab_cross_xy == 0) { // This means parallel
            if (ax.Cross(ab) == 0) { // Means collinear (fancy term for on the same infinite line)
                float t0 = (ax * ab) / (ab * ab);
                float t1 = ((other.p2 - p1) * ab) / (ab * ab);
                if (t0 > t1) { // Case where opposite directions
                    float temp = t0;
                    t0 = t1;
                    t1 = temp;
                }
                if (t0 <= 1 && t1 >= 0) {
                    crossingPoint = p1 + ab * t0;
                    return true;
                }
            }
            return false; // Parallel and not crossing
        }
        float t = ax.Cross(xy) / ab_cross_xy;
        float u = ax.Cross(ab) / ab_cross_xy;
        if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
            crossingPoint = p1 + ab * t;
            return true;
        }
        return false;
    }
};

static std::ostream &operator<<(std::ostream &os, const Line &l) {
    // Converts point into printable text (I chose (a,b)-(c,d) formatting)
    os << "(" << l.p1.x << "," << l.p1.y << ")-(" << l.p2.x << "," << l.p2.y << ")";
    return os;
}

struct Circle {
    Point2D center;
    float radius;

    Circle(Point2D c = {0, 0}, float r = 0) : center(c), radius(r) {}

    Circle(float x, float y, float r) : center(x, y), radius(r) {}
};

struct Rect {
    Point2D topLeft;
    float width, height;

    Rect(float left, float top, float width, float height)
        : topLeft(Point2D(left, top)), width(width), height(height) {} 

    Rect(Point2D tl = {0, 0}, int w = 0, int h = 0) : topLeft(tl), width(w), height(h) {}

    // Creates bounding box around p1 and p2 with positive width/height
    Rect(Point2D p1, Point2D p2)
        : topLeft(std::min(p1.x, p2.x), std::min(p1.y, p2.y)),
          width(fabs(p1.x - p2.x)),
          height(fabs(p1.y - p2.y)) {}

    Rect(Point2D center, float radius)
        : topLeft(center.x - radius, center.y - radius), width(2 * radius), height(2 * radius) {}

    Rect &operator|=(const Rect &other) {
        // Make this rectangle the minimum sized rectangle that contains both rectangles
        float left = std::min(topLeft.x, other.topLeft.x);
        float top = std::min(topLeft.y, other.topLeft.y);
        float right = std::max(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::max(topLeft.y + height, other.topLeft.y + other.height);
        topLeft.x = left;
        topLeft.y = top;
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator|=(const Point2D &other) {
        // Make this rectangle the minimum sized rectangle that contains the point and original rect
        float left = std::min(topLeft.x, other.x);
        float top = std::min(topLeft.y, other.y);
        float right = std::max(topLeft.x + width, other.x);
        float bottom = std::max(topLeft.y + height, other.y);
        topLeft.x = left;
        topLeft.y = top;
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator|=(const Line &other) {
        // Make this rectangle the minimum sized rectangle that contains the line and original rect
        float left = std::min(std::min(topLeft.x, other.p1.x), other.p2.x); //Double min/max is ugly, but we can't guarantee that line is ascending.
        float top = std::min(std::min(topLeft.y, other.p1.y), other.p2.y);
        float right = std::max(std::max(topLeft.x + width, other.p1.x), other.p2.x);
        float bottom = std::max(std::max(topLeft.y + height, other.p1.y), other.p2.y);
        topLeft.x = left;
        topLeft.y = top;
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator&=(const Rect &other) {
        // Make this rectangle the maximum sized rectangle that both rectangles overlap
        float left = std::max(topLeft.x, other.topLeft.x);
        float top = std::max(topLeft.y, other.topLeft.y);
        float right = std::min(topLeft.x + width, other.topLeft.x + other.width);
        float bottom = std::min(topLeft.y + height, other.topLeft.y + other.height);
        // NOTE: Originally had this function zero out the rectangle, it now just creates negative rectangles;
        // if wanting to change functionally, the check is (right < left || bottom < top)
        topLeft.x = left;
        topLeft.y = top;
        width = right - left;
        height = bottom - top;
        return *this;
    }
    Rect &operator+=(const Point2D &other) {
        // Add a point to this rectangle's position
        topLeft += other;
        return *this;
    }
    Rect operator+(const Point2D &other) const {
        // Add a point to this rectangle's position (but not changing this one)
        return Rect(topLeft + other, width, height);
    }
    void Inset(float inset) {
        // Moves all edges inwards by this amount
        topLeft += inset;
        height -= inset * 2;
        width -= inset * 2;
    }
    bool IsInside(const Point2D &p) const {
        // Checks if a point is inside a rect (including edges)
        return (topLeft.x <= p.x && topLeft.y <= p.y && topLeft.x + width >= p.x && topLeft.y + height >= p.y);
    }
    bool Intersects(Rect a, const Rect &b) {
        a &= b;
        return a.width >= 0 && a.height >= 0;
    }
};

static std::ostream &operator<<(std::ostream &os, const Rect &l) {
    // Converts point into printable text (I chose (a,b)-(width x height) formatting)
    os << "(" << l.topLeft.x << "," << l.topLeft.y << ")-(" << l.width << " x " << l.height << ")";
    return os;
}

}  // namespace CMPUT350

#endif  // MATHUTIL_H
