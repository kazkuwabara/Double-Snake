#include "base/grid.h"
#include "utility/utility.h"

using std::string;
using std::list;
using std::vector;

Grid::Grid(const SizeType rowCnt_, const SizeType colCnt_)
    : content(rowCnt_, vector<Pt>(colCnt_)) {
    size = (rowCnt_ - 2) * (colCnt_ - 2);
    // Add boundary walls
    SizeType row = getRowCount(), col = getColCount();
    for (SizeType i = 0; i < row; ++i) {
        if (i == 0 || i == row - 1) {  // The first and last row
            for (SizeType j = 0; j < col; ++j) {
                content[i][j].setType(Pt::Type::WALL);
            }
        } else {  // Rows in the middle
            content[i][0].setType(Pt::Type::WALL);
            content[i][col - 1].setType(Pt::Type::WALL);
        }
    }
}

Grid::~Grid() {}

Pt& Grid::getPoint(const P &p) {
    return content[p.getX()][p.getY()];
}

const Pt& Grid::getPoint(const P &p) const {
    return content[p.getX()][p.getY()];
}

Grid::SizeType Grid::getRowCount() const {
    return content.size();
}

Grid::SizeType Grid::getColCount() const {
    return content[0].size();
}

Grid::SizeType Grid::getSize() const {
    return size;
}

bool Grid::isInside(const P &p) const {
    return p.getX() > 0 && p.getY() > 0
        && p.getX() < getRowCount() - 1
        && p.getY() < getColCount() - 1;
}

bool Grid::isEmpty(const P &p) const {
    return isInside(p) && getPoint(p).getType() == Pt::Type::EMPTY;
}

bool Grid::isEmptyNotVisit(const P &p) const {
    return isEmpty(p) && !getPoint(p).isVisit();
}

bool Grid::isSafe(const P &p) const {
    const Pt &Pt = getPoint(p);
    return isInside(p) && (Pt.getType() == Pt::Type::EMPTY 
                           || Pt.getType() == Pt::Type::FOOD);
}

bool Grid::isAllBody() const {
    SizeType row = getRowCount(), col = getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            Pt::Type type = content[i][j].getType();
            if (!(type == Pt::Type::hebi_HEAD
                || type == Pt::Type::hebi_BODY
                || type == Pt::Type::hebi_TAIL)) {
                return false;
            }
        }
    }
    return true;
}

void Grid::createRandFood() {
    auto random = utility::Random<>::getInstance();
    vector<P> emptyPoints = getEmptyPoints();
    if (!emptyPoints.empty()) {
        SizeType i = random->nextInt((SizeType)0, emptyPoints.size() - 1);
        createFood(emptyPoints[i]);
    }
}

void Grid::createFood(const P &p) {
    food = p;
    content[food.getX()][food.getY()].setType(Pt::Type::FOOD);
}

void Grid::removeFood() {
    if (hasFood()) {
        content[food.getX()][food.getY()].setType(Pt::Type::EMPTY);
    }
}

bool Grid::hasFood() const {
    return isInside(food) && getPoint(food).getType() == Pt::Type::FOOD;
}

const P& Grid::getFood() const {
    return food;
}

Grid::SizeType Grid::distance(const P &from, const P &to) {
    SizeType fromX = from.getX(), toX = to.getX();
    SizeType fromY = from.getY(), toY = to.getY();
    SizeType dx = fromX > toX ? fromX - toX : toX - fromX;
    SizeType dy = fromY > toY ? fromY - toY : toY - fromY;
    return dx + dy;
}

void Grid::setPointTypeWithDelay(const P &p, const Pt::Type type) {
    getPoint(p).setType(type);
    utility::sleep(20);
}

vector<P> Grid::getEmptyPoints() const {
    vector<P> points;
    SizeType row = getRowCount(), col = getColCount();
    for (SizeType i = 1; i < row - 1; ++i) {
        for (SizeType j = 1; j < col - 1; ++j) {
            if (content[i][j].getType() == Pt::Type::EMPTY) {
                points.push_back(P(i, j));
            }
        }
    }
    return points;
}
