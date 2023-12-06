#include <catch2/catch_test_macros.hpp>
#include <cmath>
#include <tml/vec3.hpp>

TEST_CASE("Vec3 tests", "[library]")
{
    SECTION("Successfully create a vec3")
    {
        tml::vec3 const vec{0.0F, 1.0F, 2.0F};
        REQUIRE(vec.x() == 0.0F);
        REQUIRE(vec.y() == 1.0F);
        REQUIRE(vec.z() == 2.0F);
    }

    SECTION("Successfully calculate the norm of a vec3")
    {
        tml::vec3 const vec{0.0F, 1.0F, 2.0F};
        REQUIRE(std::round(vec.norm()) == 2.0F);
    }

    SECTION("Successfully calculate the cross product of 2 vec3")
    {
        tml::vec3 const vec1{0.0F, 1.0F, 2.0F};
        tml::vec3 const vec2{3.0F, 4.0F, 5.0F};
        tml::vec3 const vec3{vec1.cross(vec2)};
        REQUIRE(vec3.x() == -3.0F);
        REQUIRE(vec3.y() == 6.0F);
        REQUIRE(vec3.z() == -3.0F);
    }

    SECTION("Successfully add 2 vec3")
    {
        tml::vec3 const vec1{0.0F, 1.0F, 2.0F};
        tml::vec3 const vec2{3.0F, 4.0F, 5.0F};
        tml::vec3 const vec3{vec1 + vec2};
        REQUIRE(vec3.x() == 3.0F);
        REQUIRE(vec3.y() == 5.0F);
        REQUIRE(vec3.z() == 7.0F);
    }

    SECTION("Successfully subtract 2 vec3")
    {
        tml::vec3 const vec1{0.0F, 1.0F, 2.0F};
        tml::vec3 const vec2{3.0F, 4.0F, 5.0F};
        tml::vec3 const vec3{vec1 - vec2};
        REQUIRE(vec3.x() == -3.0F);
        REQUIRE(vec3.y() == -3.0F);
        REQUIRE(vec3.z() == -3.0F);
    }

    SECTION("Successfully add a vec3 to another vec3")
    {
        tml::vec3 vec1{0.0F, 1.0F, 2.0F};
        tml::vec3 const vec2{3.0F, 4.0F, 5.0F};
        vec1 += vec2;
        REQUIRE(vec1.x() == 3.0F);
        REQUIRE(vec1.y() == 5.0F);
        REQUIRE(vec1.z() == 7.0F);
    }

    SECTION("Successfully subtract a vec3 from another vec3")
    {
        tml::vec3 vec1{0.0F, 1.0F, 2.0F};
        tml::vec3 const vec2{3.0F, 4.0F, 5.0F};
        vec1 -= vec2;
        REQUIRE(vec1.x() == -3.0F);
        REQUIRE(vec1.y() == -3.0F);
        REQUIRE(vec1.z() == -3.0F);
    }
}
