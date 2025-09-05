#include <iostream>
#include <gmock/gmock.h>

// Idk I tried everything to make it work with header files. 
// It works with those .cpp files included, so I guess I will use test like this, for now
#include "utils.cpp"
#include "Vector2f.cpp"
#include "structs.cpp"
#include "Stats.cpp"
#include "Transform.cpp"
#include "Unit.cpp"

TEST(UnitTest,Overheal)
{
    Transform transform{};
    Stats baseStats{100,200};
    Unit testDummy{UnitType{},transform,baseStats};
    
    testDummy.Heal(200);
    EXPECT_EQ(testDummy.GetStats().m_CurrentHealth,testDummy.GetStats().m_MaxHealth);
}

TEST(UnitTest,Overkill)
{
    Transform transform{};
    Stats baseStats{100,200};
    Unit testDummy{UnitType{},transform,baseStats};

    testDummy.TakeDamage(300);
    EXPECT_EQ(testDummy.GetStats().m_CurrentHealth,0);
}

TEST(UnitTest,HealingWorks)
{
    Transform transform{};
    Stats baseStats{100,200};
    Unit testDummy{UnitType{},transform,baseStats};

    testDummy.TakeDamage(100); 
    testDummy.Heal(50);
    EXPECT_EQ(testDummy.GetStats().m_CurrentHealth,50);
}

TEST(UnitTest,DamageWorks)
{
    Transform transform{};
    Stats baseStats{100,200};
    Unit testDummy{UnitType{},transform,baseStats};

    testDummy.TakeDamage(70);
    EXPECT_EQ(testDummy.GetStats().m_CurrentHealth,30);
}




int main(int argc,char** argv)
{
    ::testing::InitGoogleTest(&argc,argv);

    return RUN_ALL_TESTS();
}
