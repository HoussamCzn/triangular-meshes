#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <tml/mesh.hpp>

TEST_CASE("Meshes tests", "[library]")
{
    SECTION("Successfully load a valid PLY file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.get_vertices().size() == 8UL);
        REQUIRE(mesh.get_faces().size() == 12UL);
    }

    SECTION("Successfully save a mesh to a PLY file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.save("output.ply", true) == tml::error_code::none);

        std::ifstream const file{"output.ply"};
        REQUIRE(file.good());
        REQUIRE(mesh.get_vertices().size() == 8UL);
        REQUIRE(mesh.get_faces().size() == 12UL);
    }
}
