#include <bits/stdc++.h>
using namespace std;
namespace Geometry {
    using ld = long double;
    const ld EPS = 1e-9;
    const ld PI = acosl(-1.0L);

    inline int sgn(ld x) {
        if (fabsl(x) < EPS) return 0;
        return x < 0 ? -1 : 1;
    }
    inline bool eq(ld a, ld b) { return sgn(a - b) == 0; }

    struct Point {
        ld x, y;
        Point() : x(0), y(0) {}
        Point(ld _x, ld _y) : x(_x), y(_y) {}

        Point operator+(const Point &p) const { return {x + p.x, y + p.y}; }
        Point operator-(const Point &p) const { return {x - p.x, y - p.y}; }
        Point operator*(ld k) const { return {x * k, y * k}; }
        Point operator/(ld k) const { return {x / k, y / k}; }
        Point operator-() const { return {-x, -y}; }

        bool operator==(const Point &p) const { return sgn(x - p.x) == 0 && sgn(y - p.y) == 0; }
        bool operator!=(const Point &p) const { return !(*this == p); }
        bool operator<(const Point &p) const {
            if (sgn(x - p.x) != 0) return x < p.x;
            return sgn(y - p.y) < 0;
        }
    };
    inline std::istream &operator>>(std::istream &is, Point &p) { return is >> p.x >> p.y; }
    inline std::ostream &operator<<(std::ostream &os, const Point &p) { return os << '(' << p.x << ", " << p.y << ')'; }

    inline ld dot(Point a, Point b) { return a.x * b.x + a.y * b.y; }
    inline ld cross(Point a, Point b) { return a.x * b.y - a.y * b.x; }
    inline ld cross(Point a, Point b, Point c) { return cross(b - a, c - a); }
    inline ld norm2(Point a) { return dot(a, a); }
    inline ld norm(Point a) { return sqrtl(norm2(a)); }
    inline ld dist(Point a, Point b) { return norm(a - b); }
    inline ld dist2(Point a, Point b) { return norm2(a - b); }

    inline Point perp(Point p) { return {-p.y, p.x}; }
    inline Point unit(Point p) { ld n = norm(p); return sgn(n) == 0 ? p : p / n; }
    inline ld angle(Point p) { return atan2l(p.y, p.x); }
    inline Point fromPolar(ld r, ld theta) { return {r * cosl(theta), r * sinl(theta)}; }

    inline Point rotate(Point p, ld ang) {
        return {p.x * cosl(ang) - p.y * sinl(ang), p.x * sinl(ang) + p.y * cosl(ang)};
    }
    inline Point rotateAbout(Point p, Point pivot, ld ang) { return pivot + rotate(p - pivot, ang); }
    inline Point midpoint(Point a, Point b) { return (a + b) / 2; }

    inline ld angleBetween(Point a, Point b) {
        ld c = dot(a, b) / (norm(a) * norm(b));
        c = std::max((ld)-1.0, std::min((ld)1.0, c));
        return acosl(c);
    }

    inline bool polarCmp(const Point &a, const Point &b, const Point &pivot = Point(0, 0)) {
        Point pa = a - pivot, pb = b - pivot;
        bool ha = sgn(pa.y) > 0 || (sgn(pa.y) == 0 && sgn(pa.x) > 0);
        bool hb = sgn(pb.y) > 0 || (sgn(pb.y) == 0 && sgn(pb.x) > 0);
        if (ha != hb) return ha > hb;
        ld c = cross(pa, pb);
        if (sgn(c) != 0) return c > 0;
        return norm2(pa) < norm2(pb);
    }

    struct Line {
        ld a, b, c;
        Line() : a(0), b(0), c(0) {}
        Line(ld A, ld B, ld C) : a(A), b(B), c(C) {}
        Line(Point p1, Point p2) {
            a = p2.y - p1.y;
            b = p1.x - p2.x;
            c = a * p1.x + b * p1.y;
        }
        Point direction() const { return {-b, a}; }
        Point normalVec() const { return {a, b}; }
    };

    inline bool parallel(Line l1, Line l2) { return sgn(l1.a * l2.b - l2.a * l1.b) == 0; }
    inline bool perpendicular(Line l1, Line l2) { return sgn(l1.a * l2.a + l1.b * l2.b) == 0; }
    inline bool same(Line l1, Line l2) {
        return parallel(l1, l2) && sgn(l1.a * l2.c - l2.a * l1.c) == 0 && sgn(l1.b * l2.c - l2.b * l1.c) == 0;
    }
    inline bool intersect(Line l1, Line l2, Point &p) {
        ld d = l1.a * l2.b - l2.a * l1.b;
        if (sgn(d) == 0) return false;
        p.x = (l1.c * l2.b - l2.c * l1.b) / d;
        p.y = (l1.a * l2.c - l2.a * l1.c) / d;
        return true;
    }
    inline Line perpendicularBisector(Point a, Point b) {
        Point mid = midpoint(a, b);
        Point v = perp(b - a);
        return Line(mid, mid + v);
    }
    inline Line lineThrough(Point p, Point dir_unit_or_any) { return Line(p, p + dir_unit_or_any); }

    inline ld distancePointLine(Point p, Line l) {
        return fabsl(l.a * p.x + l.b * p.y - l.c) / sqrtl(l.a * l.a + l.b * l.b);
    }
    inline Point projectPointOnLine(Point p, Line l) {
        ld d2 = l.a * l.a + l.b * l.b;
        ld t = (l.a * p.x + l.b * p.y - l.c) / d2;
        return {p.x - l.a * t, p.y - l.b * t};
    }
    inline Point reflectPointOverLine(Point p, Line l) {
        Point proj = projectPointOnLine(p, l);
        return proj * 2 - p;
    }
    inline bool onLine(Point p, Line l) { return sgn(l.a * p.x + l.b * p.y - l.c) == 0; }

    inline bool onSegment(Point p, Point a, Point b) {
        return sgn(cross(a, b, p)) == 0 && sgn(dot(p - a, p - b)) <= 0;
    }
    inline int orientation(Point a, Point b, Point c) { return sgn(cross(a, b, c)); }

    inline bool segmentIntersect(Point a, Point b, Point c, Point d) {
        int o1 = orientation(a, b, c);
        int o2 = orientation(a, b, d);
        int o3 = orientation(c, d, a);
        int o4 = orientation(c, d, b);
        if (o1 * o2 < 0 && o3 * o4 < 0) return true;
        if (o1 == 0 && onSegment(c, a, b)) return true;
        if (o2 == 0 && onSegment(d, a, b)) return true;
        if (o3 == 0 && onSegment(a, c, d)) return true;
        if (o4 == 0 && onSegment(b, c, d)) return true;
        return false;
    }

    inline Point closestPointOnSegment(Point p, Point a, Point b) {
        if (a == b) return a;
        ld t = dot(p - a, b - a) / norm2(b - a);
        t = std::max((ld)0.0, std::min((ld)1.0, t));
        return a + (b - a) * t;
    }
    inline ld distPointSegment(Point p, Point a, Point b) { return dist(p, closestPointOnSegment(p, a, b)); }

    inline ld distSegmentSegment(Point a, Point b, Point c, Point d) {
        if (segmentIntersect(a, b, c, d)) return 0;
        return std::min({distPointSegment(a, c, d), distPointSegment(b, c, d),
                         distPointSegment(c, a, b), distPointSegment(d, a, b)});
    }

    inline ld signedArea(const std::vector<Point> &poly) {
        ld area = 0;
        int n = (int)poly.size();
        for (int i = 0; i < n; i++) area += cross(poly[i], poly[(i + 1) % n]);
        return area / 2;
    }
    inline ld area(const std::vector<Point> &poly) { return fabsl(signedArea(poly)); }

    inline ld perimeter(const std::vector<Point> &poly) {
        ld per = 0;
        int n = (int)poly.size();
        for (int i = 0; i < n; i++) per += dist(poly[i], poly[(i + 1) % n]);
        return per;
    }

    inline Point centroid(const std::vector<Point> &poly) {
        ld cx = 0, cy = 0, A = signedArea(poly);
        int n = (int)poly.size();
        for (int i = 0; i < n; i++) {
            Point p1 = poly[i], p2 = poly[(i + 1) % n];
            ld cr = cross(p1, p2);
            cx += (p1.x + p2.x) * cr;
            cy += (p1.y + p2.y) * cr;
        }
        return {cx / (6 * A), cy / (6 * A)};
    }

    inline bool isConvex(const std::vector<Point> &poly) {
        int n = (int)poly.size();
        if (n < 3) return false;
        int dir = 0;
        for (int i = 0; i < n; i++) {
            int o = orientation(poly[i], poly[(i + 1) % n], poly[(i + 2) % n]);
            if (o == 0) continue;
            if (dir == 0) dir = o;
            else if (o != dir) return false;
        }
        return true;
    }

    inline int pointInPolygon(Point p, const std::vector<Point> &poly) {
        int n = (int)poly.size();
        bool inside = false;
        for (int i = 0; i < n; i++) {
            Point a = poly[i], b = poly[(i + 1) % n];
            if (onSegment(p, a, b)) return 2;
            if ((sgn(a.y - p.y) > 0) != (sgn(b.y - p.y) > 0)) {
                ld xin = (b.x - a.x) * (p.y - a.y) / (b.y - a.y) + a.x;
                if (sgn(p.x - xin) < 0) inside = !inside;
            }
        }
        return inside ? 1 : 0;
    }

    inline std::vector<Point> convexHull(std::vector<Point> pts) {
        std::sort(pts.begin(), pts.end());
        pts.erase(std::unique(pts.begin(), pts.end()), pts.end());
        int n = (int)pts.size();
        if (n <= 2) return pts;
        std::vector<Point> hull(2 * n);
        int k = 0;
        for (int i = 0; i < n; i++) {
            while (k >= 2 && orientation(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
            hull[k++] = pts[i];
        }
        int lower = k + 1;
        for (int i = n - 2; i >= 0; i--) {
            while (k >= lower && orientation(hull[k - 2], hull[k - 1], pts[i]) <= 0) k--;
            hull[k++] = pts[i];
        }
        hull.resize(k - 1);
        return hull;
    }
    struct Circle {
        Point o;
        ld r;
        Circle() : r(0) {}
        Circle(Point O, ld R) : o(O), r(R) {}
        bool contains(Point p) const { return sgn(dist(o, p) - r) <= 0; }
        bool onCircle(Point p) const { return sgn(dist(o, p) - r) == 0; }
        ld area() const { return (ld)PI * r * r; }
        ld circumference() const { return 2 * (ld)PI * r; }
    };

    inline bool circumcenter(Point a, Point b, Point c, Point &out) {
        if (orientation(a, b, c) == 0) return false;
        Line l1 = perpendicularBisector(a, b);
        Line l2 = perpendicularBisector(a, c);
        return intersect(l1, l2, out);
    }
    inline bool circumcircle(Point a, Point b, Point c, Circle &out) {
        Point o;
        if (!circumcenter(a, b, c, o)) return false;
        out = Circle(o, dist(o, a));
        return true;
    }

    inline Point incenter(Point a, Point b, Point c) {
        ld A = dist(b, c), B = dist(a, c), C = dist(a, b);
        ld s = A + B + C;
        return (a * A + b * B + c * C) / s;
    }
    inline Circle incircle(Point a, Point b, Point c) {
        Point i = incenter(a, b, c);
        ld s = (dist(a, b) + dist(b, c) + dist(c, a)) / 2;
        ld K = fabsl(cross(a, b, c)) / 2;
        return Circle(i, K / s);
    }
    inline int lineCircleIntersect(Line l, Circle circ, Point &p1, Point &p2) {
        Point foot = projectPointOnLine(circ.o, l);
        ld d2 = dist2(foot, circ.o);
        ld r2 = circ.r * circ.r;
        if (sgn(d2 - r2) > 0) return 0;
        Point dir = unit(l.direction());
        if (sgn(d2 - r2) == 0) { p1 = foot; return 1; }
        ld h = sqrtl(std::max((ld)0.0, r2 - d2));
        p1 = foot + dir * h;
        p2 = foot - dir * h;
        return 2;
    }
    inline int circleCircleIntersect(Circle c1, Circle c2, Point &p1, Point &p2) {
        ld d = dist(c1.o, c2.o);
        if (sgn(d) == 0 && sgn(c1.r - c2.r) == 0) return -1;
        if (sgn(d - (c1.r + c2.r)) > 0) return 0;
        if (sgn(d - fabsl(c1.r - c2.r)) < 0) return 0;
        ld a = (c1.r * c1.r - c2.r * c2.r + d * d) / (2 * d);
        ld h2 = c1.r * c1.r - a * a;
        ld h = sqrtl(std::max((ld)0.0, h2));
        Point mid = c1.o + (c2.o - c1.o) * (a / d);
        Point offset = perp(unit(c2.o - c1.o)) * h;
        p1 = mid + offset;
        p2 = mid - offset;
        return sgn(h) == 0 ? 1 : 2;
    }

    inline int tangentPointsFromPoint(Point p, Circle circ, Point &t1, Point &t2) {
        ld d2 = dist2(p, circ.o);
        ld r2 = circ.r * circ.r;
        if (sgn(d2 - r2) < 0) return 0;
        if (sgn(d2 - r2) == 0) { t1 = p; return 1; }
        ld len = sqrtl(d2 - r2);
        ld ang = asinl(circ.r / sqrtl(d2));
        Point dir = unit(circ.o - p);
        t1 = p + rotate(dir, ang) * len;
        t2 = p + rotate(dir, -ang) * len;
        return 2;
    }

}using namespace Geometry;