#include "base/hebi.h"
#include "utility/utility.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <queue>

using std::list;
using std::queue;
using std::vector;
using utility::Random;
using namespace std;

Hebi::Hebi() {}

Hebi::~Hebi() {}

void Hebi::setDirection(const Dir &d) {
    direc = d;
}

void Hebi::setGrid(Grid *const m) {
    grid = m;
}

Dir Hebi::getDirection() const {
    return direc;
}

bool Hebi::isDead() const {
    return dead;
}

void Hebi::addBody(const P &p) {
    if (bodies.size() == 0) {  // Insert a head
        grid->getPoint(p).setType(Pt::Type::hebi_HEAD);
    } else {  // Insert a body
        if (bodies.size() > 1) {
            const P &oldTail = getTail();
            grid->getPoint(oldTail).setType(Pt::Type::hebi_BODY);
        }
        grid->getPoint(p).setType(Pt::Type::hebi_TAIL);
    }
    bodies.push_back(p);
}

void Hebi::move(bool &bo) {
    if (isDead() || direc == NONE) {
        return;
    }
    grid->getPoint(getHead()).setType(Pt::Type::hebi_BODY);
    P newHead = getHead().getAdj(direc);

    bodies.push_front(newHead);
    if (!grid->isSafe(newHead)) {
        dead = true;
    } else {
        if (grid->getPoint(newHead).getType() != Pt::Type::FOOD) {
            removeTail();
        } else {
            bo = true;
            return;
        }
    }
    grid->getPoint(newHead).setType(Pt::Type::hebi_HEAD);
}

void Hebi::move() {
    if (isDead() || direc == NONE) {
        return;
    }
    
    grid->getPoint(getHead()).setType(Pt::Type::hebi_BODY);
    P newHead = getHead().getAdj(direc);
    bodies.push_front(newHead);
    if (!grid->isSafe(newHead)) {
        dead = true;
    } else {
        if (grid->getPoint(newHead).getType() != Pt::Type::FOOD) {
            removeTail();
        } else {
            return;
        }
    }
    grid->getPoint(newHead).setType(Pt::Type::hebi_HEAD);
}


void Hebi::move(const std::list<Dir> &path) {
    for (const Dir &d : path) {
        setDirection(d);
        move();
    }
}

void Hebi::decideNext() {
    if (isDead()) {
        return;
    } else if (!grid->hasFood()) {
        direc = NONE;
        return;
    }

    list<Dir> pathToFood, pathToTail;
    // Create a virtual snake
    Grid tmpGrid = *grid;
    Hebi tmphebi(*this);
    tmphebi.setGrid(&tmpGrid);
    // build the path to objective
    tmphebi.findMinPathToFood(pathToFood);
    if (!pathToFood.empty()) {
        // move virtual hebi along path
        tmphebi.move(pathToFood);
        if (tmpGrid.isAllBody()) {
            this->setDirection(*(pathToFood.begin()));
            return;
        } else {
            // virtual hebi head->tail
            tmphebi.findMaxPathToTail(pathToTail);
            if (pathToTail.size() > 1) {
                this->setDirection(*(pathToFood.begin()));
                return;
            }
        }
    }
    // real hebi head->tail
    this->findMaxPathToTail(pathToTail);
    if (pathToTail.size() > 1) {
        this->setDirection(*(pathToTail.begin()));
        return;
    }
    // furthest direction from food, idea being to traverse this path until min path can be found
    direc = Dir::DOWN;  // A default direction
    SizeType max = 0;
    P head = getHead();
    vector<P> adjPositions = head.getAllAdj();
    for (const P &adjPos : adjPositions) {
        if (grid->isSafe(adjPos)) {
            SizeType dist = Grid::distance(adjPos, grid->getFood());
            if (dist >= max) {
                max = dist;
                direc = head.getDirectionTo(adjPos);
            }
        }

    }
}

const P& Hebi::getHead() const {
    return *bodies.begin();
}

const P& Hebi::getTail() const {
    return *bodies.rbegin();
}

void Hebi::removeTail() {
    grid->getPoint(getTail()).setType(Pt::Type::EMPTY);
    bodies.pop_back();
    if (bodies.size() > 1) {
        grid->getPoint(getTail()).setType(Pt::Type::hebi_TAIL);
    }
}

void Hebi::findMinPathToFood(list<Dir> &path) {
    findPathTo(0, grid->getFood(), path);
}

void Hebi::findMaxPathToTail(list<Dir> &path) {
    findPathTo(1, getTail(), path);
}

void Hebi::findPathTo(const int pathType, const P &goal, list<Dir> &path) {
    Pt::Type oriType = grid->getPoint(goal).getType();
    grid->getPoint(goal).setType(Pt::Type::EMPTY);
    if (pathType == 0) {
        findMinPath(getHead(), goal, path);
    } else if (pathType == 1) {
        findMaxPath(getHead(), goal, path);
    }
    grid->getPoint(goal).setType(oriType);
}

void Hebi::findMinPath(const P &from, const P &to, list<Dir> &path) {
    // Init
    SizeType row = grid->getRowCount(), col = grid->getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            grid->getPoint(P(i, j)).setDist(Pt::MAX_VALUE);
        }
    }
    path.clear();
    grid->getPoint(from).setDist(0);
    queue<P> openList;
    openList.push(from);
    // BFS
    while (!openList.empty()) {
        P curPos = openList.front();
        const Pt &curPoint = grid->getPoint(curPos);
        openList.pop();
        if (curPos == to) {
            buildPath(from, to, path);
            break;
        }
        vector<P> adjPositions = curPos.getAllAdj();
        Random<>::getInstance()->shuffle(adjPositions.begin(), adjPositions.end());
        // Arrange the order of traversing to make the result path as straight as possible
        Dir bestDirec = (curPos == from ? direc : curPoint.getParent().getDirectionTo(curPos));
        for (SizeType i = 0; i < adjPositions.size(); ++i) {
            if (bestDirec == curPos.getDirectionTo(adjPositions[i])) {
                utility::swap(adjPositions[0], adjPositions[i]);
                break;
            }
        }
        // Traverse the adjacent positions
        for (const P &adjPos : adjPositions) {
            Pt &adjPoint = grid->getPoint(adjPos);
            if (grid->isEmpty(adjPos) && adjPoint.getDist() == Pt::MAX_VALUE) {
                adjPoint.setParent(curPos);
                adjPoint.setDist(curPoint.getDist() + 1);
                openList.push(adjPos);
            }
        }
    }
}

void Hebi::findMaxPath(const P &from, const P &to, list<Dir> &path) {
    // Get the shortest path
    findMinPath(from, to, path);
    // Init
    SizeType row = grid->getRowCount(), col = grid->getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            grid->getPoint(P(i, j)).setVisit(false);
        }
    }
    // Make all points on the path visited
    P cur = from;
    for (const Dir d : path) {
        grid->getPoint(cur).setVisit(true);
        cur = cur.getAdj(d);
    }
    grid->getPoint(cur).setVisit(true);
    // Extend the path between each pair of the points
    for (auto it = path.begin(); it != path.end();) {
        if (it == path.begin()) {
            cur = from;
        }
        bool extended = false;
        Dir curDirec = *it;
        P next = cur.getAdj(curDirec);
        switch (curDirec) {
            case LEFT:
            case RIGHT: {
                P curUp = cur.getAdj(UP);
                P nextUp = next.getAdj(UP);
                // Check two points above
                if (grid->isEmptyNotVisit(curUp) && grid->isEmptyNotVisit(nextUp)) {
                    grid->getPoint(curUp).setVisit(true);
                    grid->getPoint(nextUp).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, DOWN);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, UP);
                    it = path.begin();
                    extended = true;
                } else {
                    P curDown = cur.getAdj(DOWN);
                    P nextDown = next.getAdj(DOWN);
                    // Check two points below
                    if (grid->isEmptyNotVisit(curDown) && grid->isEmptyNotVisit(nextDown)) {
                        grid->getPoint(curDown).setVisit(true);
                        grid->getPoint(nextDown).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, UP);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, DOWN);
                        it = path.begin();
                        extended = true;
                    }
                }
                break;
            }
            case UP:
            case DOWN: {
                P curLeft = cur.getAdj(LEFT);
                P nextLeft = next.getAdj(LEFT);
                // Check two points on the left
                if (grid->isEmptyNotVisit(curLeft) && grid->isEmptyNotVisit(nextLeft)) {
                    grid->getPoint(curLeft).setVisit(true);
                    grid->getPoint(nextLeft).setVisit(true);
                    it = path.erase(it);
                    it = path.insert(it, RIGHT);
                    it = path.insert(it, curDirec);
                    it = path.insert(it, LEFT);
                    it = path.begin();
                    extended = true;
                } else {
                    P curRight = cur.getAdj(RIGHT);
                    P nextRight = next.getAdj(RIGHT);
                    // Check two points on the right
                    if (grid->isEmptyNotVisit(curRight) && grid->isEmptyNotVisit(nextRight)) {
                        grid->getPoint(curRight).setVisit(true);
                        grid->getPoint(nextRight).setVisit(true);
                        it = path.erase(it);
                        it = path.insert(it, LEFT);
                        it = path.insert(it, curDirec);
                        it = path.insert(it, RIGHT);
                        it = path.begin();
                        extended = true;
                    }
                }
                break;
            }
            default:
                break;
        }
        if (!extended) {
            ++it;
            cur = next;
        }
    }
}

void Hebi::buildPath(const P &from, const P &to, list<Dir> &path) const {
    P tmp = to, parent;
    while (tmp != from) {
        parent = grid->getPoint(tmp).getParent();
        path.push_front(parent.getDirectionTo(tmp));
        tmp = parent;
    }
}
