#include "base/pt.h"

const Pt::ValueType Pt::MAX_VALUE = UINT32_MAX;
const Pt::ValueType Pt::EMPTY_DIST = 999999;

Pt::Pt() {}

Pt::~Pt() {}

Pt::Type Pt::getType() const {
    return type;
}

void Pt::setType(Type type_) {
    type = type_;
}

void Pt::setParent(const P &p_) {
    parent = p_;
}

void Pt::setVisit(const bool v) {
    visit = v;
}

void Pt::setDist(const ValueType dist_) {
    dist = dist_;
}

void Pt::setIdx(const ValueType index_) {
    idx = index_;
}

P Pt::getParent() const {
    return parent;
}

bool Pt::isVisit() const {
    return visit;
}

Pt::ValueType Pt::getDist() const {
    return dist;
}

Pt::ValueType Pt::getIdx() const {
    return idx;
}
