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
        REQUIRE(vec.norm() == 2.236068F);
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
}
