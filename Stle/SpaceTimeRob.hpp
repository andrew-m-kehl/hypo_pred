#ifndef STLE_SPACE_TIME_ROB_HPP
#define STLE_SPACE_TIME_ROB_HPP

#include "SpaceTime.hpp"

namespace Stle {

STLE_API inline Space spaceRobFromBool(bool b) {
    return b ? Space::inf() : Space::minusInf();
}

STLE_API inline Space spaceLtRob(Space v1, Space v2) {
    return v2 - v1;
}

struct STLE_API SpaceLtRob {
    Space operator()(Space v1, Space v2) {
        return spaceLtRob(v1, v2);
    }
};

template <typename TIt>
STLE_API inline Space spaceLtRobR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceLtRob, SpaceMin>(begin, end);
}

struct STLE_API SpaceLtRobR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceLtRobR(begin, end);
    }
};

STLE_API inline Space spaceLeqRob(Space v1, Space v2) {
    return v2 - v1;
}

struct STLE_API SpaceLeqRob {
    Space operator()(Space v1, Space v2) {
        return spaceLeqRob(v1, v2);
    }
};

template <typename TIt>
STLE_API inline Space spaceLeqRobR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceLeqRob, SpaceMin>(begin, end);
}

struct STLE_API SpaceLeqRobR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceLeqRobR(begin, end);
    }
};

STLE_API inline Space spaceGtRob(Space v1, Space v2) {
    return v1 - v2;
}

struct STLE_API SpaceGtRob {
    Space operator()(Space v1, Space v2) {
        return spaceGtRob(v1, v2);
    }
};

template <typename TIt>
STLE_API inline Space spaceGtRobR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceGtRob, SpaceMin>(begin, end);
}

struct STLE_API SpaceGtRobR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceGtRobR(begin, end);
    }
};

STLE_API inline Space spaceGeqRob(Space v1, Space v2) {
    return v1 - v2;
}

struct STLE_API SpaceGeqRob {
    Space operator()(Space v1, Space v2) {
        return spaceGeqRob(v1, v2);
    }
};

template <typename TIt>
STLE_API inline Space spaceGeqRobR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceGeqRob, SpaceMin>(begin, end);
}

struct STLE_API SpaceGeqRobR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceGeqRobR(begin, end);
    }
};

STLE_API inline Space spaceImpliesRob(Space v1, Space v2) {
    return spaceMax(-v1, v2);
}

struct STLE_API SpaceRobImplies {
    Space operator()(Space v1, Space v2) {
        return spaceImpliesRob(v1, v2);
    }
};

template <typename TIt>
STLE_API inline Space spaceImpliesRobR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count < 2)
        throw std::runtime_error("Implication should have 2 or more arguments.");
    return spaceRightAssocR<SpaceRobImplies, SpaceMake0>(begin, end);
}

struct STLE_API SpaceImpliesRobR {
    template <typename TIt>    
    Space operator()(TIt begin, TIt end) {
        return spaceImpliesRobR(begin, end);
    }
};

STLE_API inline Space spaceIteRob(Space condV, Space thenV, Space elseV) {
    return spaceMax(spaceMin(condV, thenV), spaceMin(-condV, elseV));
}

template <typename TIt>
Space spaceIteRobR(TIt begin, TIt end) {
    auto count = std::distance(begin, end);
    if (count != 3)
        throw std::runtime_error("Ite should have 3 arguments.");
    TIt second = std::next(begin);
    TIt third = std::next(second);
    return spaceIteRob(*begin, *second, *third);
}

struct STLE_API SpaceIteRob {
    Space operator()(Space condV, Space thenV, Space elseV) {
        return spaceIteRob(condV, thenV, elseV);
    }
};

struct STLE_API SpaceIteRobR {
    template <typename TIt>
    Space operator()(TIt begin, TIt end) {
        return spaceIteRobR(begin, end);
    }
};

}

#endif
