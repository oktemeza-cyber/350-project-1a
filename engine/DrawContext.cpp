#include "DrawContext.h"

namespace CMPUT350 {

DrawContext::DrawContext(std::shared_ptr<sf::RenderWindow> window, std::shared_ptr<sf::Font> font)
    : mWindow(window), mFont(font) {}

void DrawContext::DrawCenteredText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {}

void DrawContext::DrawText(const std::string &text, int pixelSize, Point2D p, RGBColor c) {}

void DrawContext::DrawCircle(Point2D p, float radius, RGBColor c) {
    sf::CircleShape shape(radius);
    shape.setPosition(sf::Vector2f(p.x - radius, p.y - radius));
    shape.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);

}

void DrawContext::DrawRect(Rect r, RGBColor c) {
    sf::RectangleShape shape(sf::Vector2f(r.width, r.height));
    shape.setPosition(sf::Vector2f(r.topLeft.x, r.topLeft.y));
    shape.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(shape);
}

void DrawContext::FrameRect(Rect r, float width, RGBColor c) {
    // this system breaks if width is larger than the size of the rectangle, but I thought it'd look nicer
    Point2D topOffset(0,width/2);
    Point2D sideOffset(width/2,0);

    Point2D topLeft = r.topLeft;
    Point2D topRight = topLeft + Point2D(r.width, 0);
    Point2D bottomLeft = topLeft + Point2D(0,r.height);
    Point2D bottomRight = topRight + Point2D(0,r.height);

    DrawLine(topLeft + topOffset, topRight + topOffset, width, c); //Top line
    DrawLine(topLeft + sideOffset, bottomLeft + sideOffset, width, c); //Left line
    DrawLine(topRight - sideOffset, bottomRight - sideOffset, width, c); //Right line
    DrawLine(bottomLeft - topOffset, bottomRight - topOffset, width, c); //Bottom line
}

/**
 * @brief Draws a line between two points with a specified width and color.
 *
 * @param from The starting point of the line (Point2D).
 * @param to The ending point of the line (Point2D).
 * @param width The width of the line in pixels.
 * @param c The color of the line, specified as an RGBColor object.
 *
 * This function calculates the distance and angle between the two points
 * and uses a polygone shape to represent the line. The line is drawn
 * relative to the world offset and rendered onto the associated window.
 */
void DrawContext::DrawLine(Point2D from, Point2D to, float width, RGBColor c) {
    Point2D end = (from - to).Perpendicular();
    end.Normalize();
    end *= width / 2;

    //Is there a better way to do this? Probably. Does this match the class definition? Yes, so I'm doing this.
    sf::ConvexShape line;
    line.setPointCount(4);
    line.setPoint(0, {(from + end).x, (from + end).y});
    line.setPoint(1, {(to + end).x, (to + end).y});
    line.setPoint(2, {(to - end).x, (to - end).y});
    line.setPoint(3, {(from - end).x, (from - end).y});

    line.setFillColor(sf::Color(c.r, c.g, c.b));
    mWindow->draw(line);
}

int DrawContext::GetWindowWidth() { return mWindow->getSize().x; }

int DrawContext::GetWindowHeight() { return mWindow->getSize().y; }

}  // namespace CMPUT350
