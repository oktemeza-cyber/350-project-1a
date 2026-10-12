#include "Bezier.h"

namespace CMPUT350 {

Bezier::Bezier(const std::vector<Point2D> &controlPoints) : mControlPoints(controlPoints) {}

Point2D Bezier::GetPoint(float t) const {
    return GetPoint(mControlPoints, t);
}

Point2D Bezier::GetSlope(float t) const {
    return GetSlope(mControlPoints, t);
}

Point2D Bezier::GetPoint(const std::vector<Point2D> &pts, float t) {
    if (pts.size() < 4) return Point2D(0, 0); // Not enough points to define a cubic Bezier curve

    const int k = (pts.size() - 1) / 3;

    if (t <= 0.0f) return pts.front();
    if (t >= static_cast<float>(k)) return pts[k*3];

    const float segmentT = t - std::floor(t);
    const int startIndex = static_cast<int>(t) * 3;

    const float u = 1.0f - segmentT;
    return (u*u*u*pts[startIndex]) + (3*u*u*segmentT*pts[startIndex + 1]) + (3*u*segmentT*segmentT*pts[startIndex + 2]) + (segmentT*segmentT*segmentT*pts[startIndex + 3]);
}

Point2D Bezier::GetSlope(const std::vector<Point2D> &pts, float t) {
    if (pts.size() < 4) return Point2D(0, 0); // Not enough points to define a cubic Bezier curve

    const int k = (pts.size() - 1) / 3;

    if (t <= 0.0f) return 3*(pts[1] - pts[0]);
    if (t >= static_cast<float>(k)) return 3*(pts[k*3] - pts[k*3 - 1]);

    const float segmentT = t - std::floor(t);
    const int startIndex = static_cast<int>(t) * 3;

    const float u = 1.0f - segmentT;
    return (3.0f*u*u*(pts[startIndex + 1] - pts[startIndex])) + (6.0f*u*segmentT*(pts[startIndex + 2] - pts[startIndex + 1])) + (3.0f*segmentT*segmentT*(pts[startIndex + 3] - pts[startIndex + 2]));
}

} // namespace CMPUT350