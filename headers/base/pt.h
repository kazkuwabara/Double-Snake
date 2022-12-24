#ifndef hebi_PT_H
#define hebi_PT_H

#include "base/p.h"
#include <cstdint>

/*
Point on the game map.
*/
class Pt {
public:
    typedef uint32_t ValueType;

    static const ValueType MAX_VALUE;
    static const ValueType EMPTY_DIST;

    enum Type {
        EMPTY,
        WALL,
        FOOD,
        hebi_BODY,
        hebi_HEAD,
        hebi_TAIL
    };

public:
    Pt();
    ~Pt();

    void setType(Type type_);
    void setParent(const P &p_);
    void setVisit(const bool v);
    void setDist(const ValueType dist_);
    void setIdx(const ValueType index_);
    Type getType() const;
    P getParent() const;
    bool isVisit() const;
    ValueType getDist() const;
    ValueType getIdx() const;

private:
    Type type = EMPTY;
    P parent;
    bool visit;
    ValueType dist;
    ValueType idx;
};

#endif
