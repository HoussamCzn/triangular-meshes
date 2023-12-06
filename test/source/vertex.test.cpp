#include <catch2/catch_test_macros.hpp>
#include <tml/vec3.hpp>
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

    SECTION("Successfully add an adjacent vertex")
    {
        tml::vertex vertex{0.0F, 1.0F, 2.0F};
        vertex.add_neighbor(1);
        REQUIRE(vertex.neighbors().size() == 1);
        REQUIRE(vertex.neighbors()[0] == 1);
    }

    SECTION("Successfully translate a vertex")
    {
        tml::vertex vertex{0.0F, 1.0F, 2.0F};
        tml::vec3 const offset{3.0F, 4.0F, 5.0F};
        vertex.translate(offset);
        REQUIRE(vertex.x() == 3.0F);
        REQUIRE(vertex.y() == 5.0F);
        REQUIRE(vertex.z() == 7.0F);
    }
}
