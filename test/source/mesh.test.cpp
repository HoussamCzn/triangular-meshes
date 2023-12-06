#include <catch2/catch_test_macros.hpp>
#include <fstream>
#include <tml/mesh.hpp>

TEST_CASE("Meshes tests", "[library]")
{
    SECTION("Successfully load a valid PLY file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.vertices().size() == 8UL);
        REQUIRE(mesh.faces().size() == 12UL);
    }

    SECTION("Successfully save a mesh to a PLY file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.save("output.ply", true) == tml::error_code::none);

        std::ifstream const file{"output.ply"};
        REQUIRE(file.good());
        REQUIRE(mesh.vertices().size() == 8UL);
        REQUIRE(mesh.faces().size() == 12UL);
    }

    SECTION("Save a mesh to a PLY file that already exists")
    {
        tml::mesh const mesh{"input.ply"};

        WHEN("Overwriting is allowed") { REQUIRE(mesh.save("output.ply", true) == tml::error_code::none); }

        WHEN("Overwriting is not allowed") { REQUIRE(mesh.save("output.ply", false) == tml::error_code::file_already_exists); }
    }

    SECTION("Check the adjacent vertices of a vertex")
    {
        tml::mesh const mesh{"input.ply"};
        auto const& vertices = mesh.vertices();
        REQUIRE(vertices[0].adjacents().size() == 5UL);
        REQUIRE(vertices[1].adjacents().size() == 4UL);
        REQUIRE(vertices[2].adjacents().size() == 4UL);
        REQUIRE(vertices[3].adjacents().size() == 5UL);
        REQUIRE(vertices[4].adjacents().size() == 4UL);
        REQUIRE(vertices[5].adjacents().size() == 5UL);
        REQUIRE(vertices[6].adjacents().size() == 5UL);
        REQUIRE(vertices[7].adjacents().size() == 4UL);
    }

    SECTION("Check the surface area of a mesh")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.surface_area() == 24.0F);
    }
}
