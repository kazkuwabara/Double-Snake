#ifndef hebi_GRID_H
#define hebi_GRID_H

#include "base/pt.h"
#include <list>

class Grid {
public:
    typedef P::SizeType SizeType;

public:
    Grid(const SizeType rowCnt_, const SizeType colCnt_);
    ~Grid();

    Pt& getPoint(const P &p);
    const Pt& getPoint(const P &p) const;

    SizeType getRowCount() const;
    SizeType getColCount() const;

    bool isInside(const P &p) const;
    bool isEmpty(const P &p) const;
    bool isEmptyNotVisit(const P &p) const;
    bool isSafe(const P &p) const;
    bool isAllBody() const;

    void createRandFood();
    void createFood(const P &p);
    void removeFood();
    bool hasFood() const;
    const P& getFood() const;

    /*
    Return the number of points in the grid excluding the boundaries.
    */
    SizeType getSize() const;

    /*
    Return the manhatten distance between two pitions.

    @param from The starting pition
    @param to   The ending pition
    @return     The manhatten distance
    */
    static SizeType distance(const P &from, const P &to);

private:
    std::vector<P> getEmptyPoints() const;

    void setPointTypeWithDelay(const P &p, const Pt::Type type);

private:
    P food;
    SizeType size;
    std::vector<std::vector<Pt>> content;
};

#endif
