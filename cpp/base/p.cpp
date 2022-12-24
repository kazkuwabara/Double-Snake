#include "base/p.h"
#include "utility/utility.h"

P::P(const SizeType x_, const SizeType y_) : x(x_), y(y_) {}

P::~P() {}

P::SizeType P::getX() const {
    return x;
}

P::SizeType P::getY() const {
    return y;
}

void P::setX(const SizeType x_) {
    x = x_;
}

void P::setY(const SizeType y_) {
    y = y_;
}

P operator+(const P &a, const P &b) {
    return P(a.x + b.x, a.y + b.y);
}

bool operator==(const P &a, const P &b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(const P &a, const P &b) {
    return !(a == b);
}

std::string P::toString() const {
    return "(" + utility::toString(x) + "," + utility::toString(y) + ")";
}

Dir P::getDirectionTo(const P &p) const {
    if (x == p.x) {
        if (p.y - y == 1) {
            return RIGHT;
        } else if (y - p.y == 1) {
            return LEFT;
        }
    } else if (y == p.y) {
        if (p.x - x == 1) {
            return DOWN;
        } else if (x - p.x == 1) {
            return UP;
        }
    }
    return NONE;
}

P P::getAdj(const Dir d) const {
    switch (d) {
        case LEFT:
            if (y) {
                return P(x, y - 1);
            }
            break;
        case UP:
            if (x) {
                return P(x - 1, y);
            }
            break;
        case RIGHT:
            return P(x, y + 1);
        case DOWN:
            return P(x + 1, y);
        default:
            break;
    }
    return *this;
}

std::vector<P> P::getAllAdj() const {
    std::vector<P> adj;
    for (int i = 1; i <= 4; ++i) {
        P p = getAdj((Dir)i);
        if (p != *this) {
            adj.push_back(p);
        }
    }
    return adj;
}
