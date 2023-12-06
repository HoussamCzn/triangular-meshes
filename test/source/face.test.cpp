#include <catch2/catch_test_macros.hpp>
#include <tml/face.hpp>

TEST_CASE("Faces tests", "[library]")
{
    SECTION("Successfully create a face")
    {
        tml::face const face{0, 1, 2};
        REQUIRE(face.indices()[0] == 0);
        REQUIRE(face.indices()[1] == 1);
        REQUIRE(face.indices()[2] == 2);
    }

    SECTION("Invert a face")
    {
        tml::face face{0, 1, 2};
        face.invert();
        REQUIRE(face.indices()[0] == 2);
        REQUIRE(face.indices()[1] == 1);
        REQUIRE(face.indices()[2] == 0);
    }
}
