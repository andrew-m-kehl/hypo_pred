#include <gtest/gtest.h>

#include <Stle/SpaceTime.hpp>
#include <Stle/SpaceTimeRob.hpp>

#include <vector>

using namespace Stle;

TEST(SpaceTests, TestMinMaxImpliesR) {
    {
        std::vector<Space> v;        
        ASSERT_EQ(Space::inf(), SpaceMinR{}(v.begin(), v.end()));
        ASSERT_EQ(Space::minusInf(), SpaceMaxR{}(v.begin(), v.end()));
        ASSERT_THROW(SpaceImpliesRobR{}(v.begin(), v.end()), std::runtime_error);
    }
    {
        std::vector<Space> v = {Space(1)};
        ASSERT_EQ(Space(1), SpaceMinR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceMaxR{}(v.begin(), v.end()));
        ASSERT_THROW(SpaceImpliesRobR{}(v.begin(), v.end()), std::runtime_error);
    }
    {
        std::vector<Space> v = {Space(0), Space(1), Space(-2), Space(2), Space(-1)};
        ASSERT_EQ(Space(-2), SpaceMinR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(2), SpaceMaxR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(2), SpaceImpliesRobR{}(v.begin(), v.end()));
    }
}

template <typename TIt>
Space spaceOrNoShortR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceOr, SpaceMake0>(begin, end);
}

template <typename TIt>
Space spaceAndNoShortR(TIt begin, TIt end) {
    return spaceLeftAssocR<SpaceAnd, SpaceMake1>(begin, end);
}

template <typename TIt>
Space spaceOrRightAssocR(TIt begin, TIt end) {
    return spaceRightAssocR<SpaceOr, SpaceMake0>(begin, end);
}

template <typename TIt>
Space spaceAndRightAssocR(TIt begin, TIt end) {
    return spaceRightAssocR<SpaceAnd, SpaceMake1>(begin, end);
}

template <typename TIt>
Space spaceImpliesNoShortR(TIt begin, TIt end) {
    return spaceRightAssocR<SpaceImplies, SpaceMake0>(begin, end);
}

TEST(SpaceTests, TestBooleanR) {
    {
        std::vector<Space> v;        
        ASSERT_EQ(Space(0), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceAndR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceAndR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(0)};
        ASSERT_EQ(Space(0), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(0), Space(0)};
        ASSERT_EQ(Space(0), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(0), Space(1)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(0)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(1)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(1)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(0), Space(0), Space(0)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(0), Space(0), Space(1), Space(0)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(0), Space(1), Space(1)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v = {Space(1), Space(1), Space(1), Space(0)};
        ASSERT_EQ(Space(1), SpaceOrR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceAndR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceImpliesR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceImpliesNoShortR(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceOrRightAssocR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceAndRightAssocR(v.begin(), v.end()));
    }
    
}

TEST(SpaceTests, TestArithmetic) {    
    {
        std::vector<Space> v;        
        ASSERT_EQ(Space(0), SpacePlusR{}(v.begin(), v.end()));            
        ASSERT_EQ(Space(1), SpaceMulR{}(v.begin(), v.end())); 
    }
    {
        std::vector<Space> v { Space(2) };            
        ASSERT_EQ(Space(2), SpacePlusR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(2), SpaceMulR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(2), Space(0) };            
        ASSERT_EQ(Space(2), SpacePlusR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceMulR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(1), Space(2), Space(-1), Space(-2) };            
        ASSERT_EQ(Space(0), SpacePlusR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(4), SpaceMulR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(1), Space(2), Space::inf(), Space(-1), Space(-2) };            
        ASSERT_EQ(Space::inf(), SpacePlusR{}(v.begin(), v.end()));
        ASSERT_EQ(Space::inf(), SpaceMulR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(1), Space(2), Space::minusInf(), Space(-1), Space(-2) };            
        ASSERT_EQ(Space::minusInf(), SpacePlusR{}(v.begin(), v.end()));
        ASSERT_EQ(Space::minusInf(), SpaceMulR{}(v.begin(), v.end()));
    }    
}

template<typename TIt>
Space spaceLtNoShortR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceLt, SpaceAnd>(begin, end);
}

template<typename TIt>
Space spaceLeqNoShortR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceLeq, SpaceAnd>(begin, end);
}

template<typename TIt>
Space spaceGtNoShortR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceGt, SpaceAnd>(begin, end);
}

template<typename TIt>
Space spaceGeqNoShortR(TIt begin, TIt end) {
    return spaceChainableWithOpR<SpaceGeq, SpaceAnd>(begin, end);
}

TEST(SpaceTests, TestComparisons) {
    {
        std::vector<Space> v { Space(0), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));        
        ASSERT_EQ(Space(1), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));        
        ASSERT_EQ(Space(1), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(1) };
        ASSERT_EQ(Space(1), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(1), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(0), Space(0), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(1), Space(2), Space(3) };
        ASSERT_EQ(Space(1), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(1), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(1), Space(3), Space(2) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(3), Space(2), Space(1), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(1), spaceGeqNoShortR(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(3), Space(2), Space(1), Space(0), Space(4) };
        ASSERT_EQ(Space(0), SpaceLtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceLeqR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqR{}(v.begin(), v.end()));
        
        ASSERT_EQ(Space(0), spaceLtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceLeqNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGtNoShortR(v.begin(), v.end()));
        ASSERT_EQ(Space(0), spaceGeqNoShortR(v.begin(), v.end()));
    }
}

TEST(SpaceTests, TestRobComparisons) {
    {
        std::vector<Space> v { Space(0), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtRobR{}(v.begin(), v.end()));        
        ASSERT_EQ(Space(0), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(1) };
        ASSERT_EQ(Space(1), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-1), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-1), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(1), Space(0) };
        ASSERT_EQ(Space(-1), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-1), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(0), Space(0), Space(0) };
        ASSERT_EQ(Space(0), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(0), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(0), Space(2), Space(3), Space(7) };
        ASSERT_EQ(Space(1), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-4), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-4), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(7), Space(3), Space(2), Space(0) };
        ASSERT_EQ(Space(-4), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-4), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(1), SpaceGeqRobR{}(v.begin(), v.end()));
    }
    {
        std::vector<Space> v { Space(3), Space(2), Space(1), Space(0), Space(4) };
        ASSERT_EQ(Space(-1), SpaceLtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-1), SpaceLeqRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-4), SpaceGtRobR{}(v.begin(), v.end()));
        ASSERT_EQ(Space(-4), SpaceGeqRobR{}(v.begin(), v.end()));
    }
}
