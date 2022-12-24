#ifndef hebi_HEBI_H
#define hebi_HEBI_H

#include "base/grid.h"

class Hebi {
public:
    typedef Grid::SizeType SizeType;

public:
    Hebi();
    ~Hebi();


    const P& getHead() const;
    const P& getTail() const;

    bool isDead() const;

    void setGrid(Grid *const m);

    void setDirection(const Dir &d);
    Dir getDirection() const;

    void addBody(const P &p);

    void move();
    void move(bool &b);

    void move(const std::list<Dir> &path);

    void decideNext();

private:
    void removeTail();

    void findMinPathToFood(std::list<Dir> &path);
    void findMaxPathToTail(std::list<Dir> &path);

    void findPathTo(const int type, const P &to, std::list<Dir> &path);

    void findMinPath(const P &from, const P &to, std::list<Dir> &path);

    void findMaxPath(const P &from, const P &to, std::list<Dir> &path);

    void buildPath(const P &from, const P &to, std::list<Dir> &path) const;

private:
    Grid *grid = nullptr;
    std::list<P> bodies;
    Dir direc = NONE;
    bool dead = false;
};

#endif
