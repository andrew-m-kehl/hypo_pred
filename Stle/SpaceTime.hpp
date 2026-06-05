#ifndef STLE_SPACE_TIME_HPP
#define STLE_SPACE_TIME_HPP

#include "Api.hpp"

#include "Double.hpp"
#include "Float.hpp"

namespace Stle {

typedef Double Time;

// Having Float for Space is only marginally faster, about 10% faster with pragma pack 1.
// Having Float for Time as well is about 25% faster.
typedef Double Space;

struct STLE_API SpaceMake0 {
    Space operator()() const {
        return Space(0);
    }
};

struct STLE_API SpaceMake1 {
    Space operator()() const {
        return Space(1);
    }
};

struct STLE_API SpaceMakeInf {
    Space operator()() const {
        return Space::inf();
    }
};

struct STLE_API SpaceMakeMinusInf {
    Space operator()() const {
        return Space::minusInf();
    }
};

STLE_API inline Space spaceTo01(Space v) {
    return v == Space(0) ? v : Space(1);
}

struct STLE_API SpaceTo01 {
    Space operator()(Space v) {
        return spaceTo01(v);
    }
};

STLE_API inline bool spaceToBool(Space v) {
    return v != Space(0);
}

STLE_API inline Space spaceFromBool(bool b) {
    return Space(static_cast<int>(b));
}

template<typename TOp, typename TMakeDefault, typename TIt>
Space spaceLeftAssocR(TIt begin, TIt end) {
    if (begin == end)
        return TMakeDefault{}();
    TOp op;
    Space result = *begin;
    for (TIt it = std::next(begin); it != end; ++it)
        result = op(result, *it);
    return result;
}

template <typename TOp, typename TMakeDefault, typename TIt>
Space spaceRightAssocR(TIt begin, TIt end) {
    if (begin == end)
        return TMakeDefault{}();
    TOp op;
    TIt it = std::prev(end);
    Space result = *it;
    while(true) {
        if (it == begin)
            break;
        --it;
        result = op(*it, result);
    }
    return result;
}

template<typename TOp, typename TIt>
Space spaceChainableWithAndR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count < 2)
        throw std::runtime_error("Operation should have two or more arguments.");
    TOp op;
    Space v1(*begin);    
    for (TIt it2 = std::next(begin); it2 != end; ++it2) {
        Space v2 = *it2;
        if (!spaceToBool(op(v1, v2)))
            return spaceFromBool(false);        
        v1 = v2;
    }
    return spaceFromBool(true);
}

template<typename TOp, typename TChainOp, typename TIt>
Space spaceChainableWithOpR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count < 2)
        throw std::runtime_error("Operation should have two or more arguments.");
    TOp op;
    TChainOp chainOp;
    Space v1(*begin);
    TIt it2 = std::next(begin);
    Space v2(*it2);
    Space result = op(v1, v2);
    ++it2;
    for (; it2 != end; ++it2) {
        v1 = v2;
        v2 = *it2;
        result = chainOp(result, op(v1, v2));
    }
    return result;
}

STLE_API inline Space spaceMin(Space v1, Space v2) {
    return (v1 <= v2) ? v1 : v2;
}

struct STLE_API SpaceMin {
    Space operator()(Space v1, Space v2) {
        return spaceMin(v1, v2);
    }
};

template <typename TIt>
Space spaceMinR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceMin, SpaceMakeInf>(begin, end);
}

struct STLE_API SpaceMinR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceMinR(begin, end);
    }
};

STLE_API inline Space spaceMax(Space v1, Space v2) {
    return (v1 >= v2) ? v1 : v2;
}

struct STLE_API SpaceMax {
    Space operator()(Space v1, Space v2) {
        return spaceMax(v1, v2);
    }
};

template <typename TIt>
Space spaceMaxR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceMax, SpaceMakeMinusInf>(begin, end);
}

struct STLE_API SpaceMaxR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceMaxR(begin, end);
    }
};

inline Space spacePlus(Space v1, Space v2) {
    return v1 + v2;
}

struct STLE_API SpacePlus {
    Space operator()(Space v1, Space v2) {
        return spacePlus(v1, v2);
    }
};

template <typename TIt>
Space spacePlusR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpacePlus, SpaceMake0>(begin, end);
}

struct STLE_API SpacePlusR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spacePlusR(begin, end);
    }
};

STLE_API inline Space spaceMinus1(Space v) {
    return -v;    
}

STLE_API inline Space spaceMinus2(Space v1, Space v2) {
    return v1 - v2;
}

template <typename TIt>
Space spaceMinusR(TIt begin, TIt end) {
    if (begin == end)
        throw std::runtime_error("Minus should have at least one argument.");        
    TIt second = std::next(begin);
    if (second == end)
        return spaceMinus1(*begin);
    Space result = *begin;
    for (TIt it = std::next(begin); it != end; ++it)
        result = spaceMinus2(result, *it);
    return result;
}

struct STLE_API SpaceMinus1 {
    Space operator()(Space v) {
        return spaceMinus1(v);
    }
};

struct STLE_API SpaceMinus2 {
    Space operator()(Space v1, Space v2) {
        return spaceMinus2(v1, v2);
    }
};

struct STLE_API SpaceMinusR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceMinR(begin, end);
    }
};

STLE_API inline Space spaceMul(Space v1, Space v2) {
    return v1 * v2;
}

struct STLE_API SpaceMul {
    Space operator()(Space v1, Space v2) {
        return spaceMul(v1, v2);
    }
};

template <typename TIt>
Space spaceMulR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceMul, SpaceMake1>(begin, end);
}

struct STLE_API SpaceMulR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceMulR(begin, end);
    }
};

STLE_API inline Space spaceDiv(Space v1, Space v2) {
    return v1 / v2;
}

template <typename TIt>
Space spaceDivR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count != 2)
        throw std::runtime_error("Division should 2 arguments.");    
    return *begin / *std::next(begin);
}

struct STLE_API SpaceDiv {
    Space operator()(Space v1, Space v2) {
        return spaceDiv(v1, v2);
    }
};

struct STLE_API SpaceDivR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceDivR(begin, end);
    }
};

struct STLE_API SpaceDer {
    Time step = Space(0);
    Space previous_v = Space(0);

    Space operator()(Space v) {
        Space diff = spaceMinus2(v, previous_v);
        previous_v = v;
        return spaceDiv(diff, step);
    }
};

template <typename TIt>
Space spaceDerR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceDer, SpaceMake0>(begin, end);
}

struct STLE_API SpaceDerR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceDerR(begin, end);
    }
};

struct STLE_API SpaceIntegral {
    Space step = Space(0);
    Space acc = Space(0);

    Space operator()(Space v) {
        acc = spacePlus(acc, v * step);
        return acc;
    }
};

struct STLE_API SpaceAccum {
    Space acc = Space(0);

    Space operator()(Space v) {
        // static Space acc = Space(0);
        acc = spacePlus(acc, v);
        return acc;
    }
};

template <typename TIt>
Space spaceAccumR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpacePlus, SpaceMake0>(begin, end);
}

STLE_API inline Space spaceAnd(Space v1, Space v2) {
    return (v1 == Space(0) || v2 == Space(0)) ? Space(0) : Space(1);
}

struct STLE_API SpaceAnd {
    Space operator()(Space v1, Space v2) {
        return spaceAnd(v1, v2);
    }
};

template <typename TIt>
Space spaceAndR(TIt begin, TIt end) {
    for (TIt it = begin; it != end; ++it) {
        if (!spaceToBool(*it))
            return spaceFromBool(false);
    }
    return spaceFromBool(true);
}

struct STLE_API SpaceAndR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceAndR(begin, end);
    }
};

STLE_API inline Space spaceOr(Space v1, Space v2) {
    return (v1 == Space(1) || v2 == Space(1)) ? Space(1) : Space(0);
}

struct STLE_API SpaceOr {
    Space operator()(Space v1, Space v2) {
        return spaceOr(v1, v2);
    }
};

template <typename TIt>
Space spaceOrR(TIt begin, TIt end) {    
    for (TIt it = begin; it != end; ++it) {
        if (spaceToBool(*it))
            return spaceFromBool(true);
    }
    return spaceFromBool(false);
}

struct STLE_API SpaceOrR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceOrR(begin, end);
    }
};

STLE_API inline Space spaceNot(Space v) {
    return (v == Space(0)) ? Space(1) : Space(0);
}

struct STLE_API SpaceNot {
    Space operator()(Space v) {
        return spaceNot(v);
    }
};

STLE_API inline Space spaceImplies(Space v1, Space v2) {
    return spaceOr(spaceNot(v1), v2);
}

template <typename TIt>
Space spaceImpliesR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count < 2)
        throw std::runtime_error("Implication should have 2 or more arguments.");
    TIt it = std::prev(end);
    Space result = *it;
    while(true) {
        --it;
        result = spaceImplies(*it, result);
        if (it == begin)
            break;
    }
    return result;
}

struct STLE_API SpaceImplies {
    Space operator()(Space v1, Space v2) {
        return spaceImplies(v1, v2);
    }
};

struct STLE_API SpaceImpliesR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceImpliesR(begin, end);
    }
};

STLE_API inline Space spaceAbs(Space v) {
    return (v >= Space(0)) ? v : -v;
}

struct STLE_API SpaceAbs {
    Space operator()(Space v) {
        return spaceAbs(v);
    }
};

STLE_API inline Space spaceIte(Space condV, Space thenV, Space elseV) {
    return (spaceToBool(condV)) ? thenV : elseV;
}

template <typename TIt>
Space spaceIteR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count != 3)
        throw std::runtime_error("Ite should have 3 arguments.");
    TIt second = std::next(begin);
    TIt third = std::next(second);
    return spaceIte(*begin, *second, *third);
}

struct STLE_API SpaceIte {
    Space operator()(Space condV, Space thenV, Space elseV) {
        return spaceIte(condV, thenV, elseV);
    }
};

struct STLE_API SpaceIteR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceIteR(begin, end);
    }
};

STLE_API inline Space spaceLt(Space v1, Space v2) {
    return spaceFromBool(v1 < v2);
}

struct STLE_API SpaceLt {
    Space operator()(Space v1, Space v2) {
        return spaceLt(v1, v2);
    }
};

template <typename TIt>
Space spaceLtR(TIt begin, TIt end) {
    return spaceChainableWithAndR<SpaceLt>(begin, end);
}

struct STLE_API SpaceLtR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceLtR(begin, end);
    }
};

STLE_API inline Space spaceLeq(Space v1, Space v2) {
    return spaceFromBool(v1 <= v2);
}

struct STLE_API SpaceLeq {
    Space operator()(Space v1, Space v2) {
        return spaceLeq(v1, v2);
    }
};

template <typename TIt>
Space spaceLeqR(TIt begin, TIt end) {
    return spaceChainableWithAndR<SpaceLeq>(begin, end);
}

struct STLE_API SpaceLeqR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceLeqR(begin, end);
    }
};

STLE_API inline Space spaceGt(Space v1, Space v2) {
    return spaceFromBool(v1 > v2);
}

struct STLE_API SpaceGt {
    Space operator()(Space v1, Space v2) {
        return spaceGt(v1, v2);
    }
};

template <typename TIt>
Space spaceGtR(TIt begin, TIt end) {
    return spaceChainableWithAndR<SpaceGt>(begin, end);
}

struct STLE_API SpaceGtR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceGtR(begin, end);
    }
};

inline Space spaceGeq(Space v1, Space v2) {
    return spaceFromBool(v1 >= v2);
}

struct STLE_API SpaceGeq {
    Space operator()(Space v1, Space v2) {
        return spaceGeq(v1, v2);
    }
};

template <typename TIt>
Space spaceGeqR(TIt begin, TIt end) {
    return spaceChainableWithAndR<SpaceGeq>(begin, end);    
}

struct STLE_API SpaceGeqR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceGeqR(begin, end);
    }
};

STLE_API inline Space spaceFirst(Space v1, Space) {
    return v1;
}

struct STLE_API SpaceFirst {
    Space operator()(Space v1, Space v2) {
        return spaceFirst(v1, v2);
    }
};

STLE_API inline Space spaceId(Space v) {
    return v;
}

struct STLE_API SpaceId {
    Space operator()(Space v) {
        return spaceId(v);
    }
};

}

#endif
