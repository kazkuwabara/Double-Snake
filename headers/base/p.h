#ifndef hebi_P_H
#define hebi_P_H

#include "base/dir.h"
#include <vector>
#include <string>
#include <cstddef>

/*
Integer coordinate in 2D plane.
*/
class P {
public:
    typedef std::size_t SizeType;

public:
    P(const SizeType x_ = 0, const SizeType y_ = 0);
    ~P();

    SizeType getX() const;
    SizeType getY() const;
    void setX(const SizeType x_);
    void setY(const SizeType y_);

    friend P operator+(const P &a, const P &b);
    friend bool operator==(const P &a, const P &b);
    friend bool operator!=(const P &a, const P &b);

    std::string toString() const;

    Dir getDirectionTo(const P &p) const;

    /*
    Return the adjacent pition at a given direction. If there
    is no valid pition at the given direction, return current pition.

    @param d The given direction
    */

    P getAdj(const Dir d) const;

    /*
    Return all adjacent pitions.
    */
    std::vector<P> getAllAdj() const;

private:
    SizeType x;
    SizeType y;
};

#endif
