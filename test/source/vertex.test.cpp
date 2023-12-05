#include <catch2/catch_test_macros.hpp>
#include <tml/vertex.hpp>

TEST_CASE("Vertices tests", "[library]")
{
    SECTION("Successfully create a vertex")
    {
        tml::vertex const vertex{0.0F, 1.0F, 2.0F};
        REQUIRE(vertex.x() == 0.0F);
        REQUIRE(vertex.y() == 1.0F);
        REQUIRE(vertex.z() == 2.0F);
    }
}
