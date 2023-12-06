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
        REQUIRE(vertices[0].neighbors().size() == 5UL);
        REQUIRE(vertices[1].neighbors().size() == 4UL);
        REQUIRE(vertices[2].neighbors().size() == 4UL);
        REQUIRE(vertices[3].neighbors().size() == 5UL);
        REQUIRE(vertices[4].neighbors().size() == 4UL);
        REQUIRE(vertices[5].neighbors().size() == 5UL);
        REQUIRE(vertices[6].neighbors().size() == 5UL);
        REQUIRE(vertices[7].neighbors().size() == 4UL);
    }

    SECTION("Check the surface area of a mesh")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.surface_area() == 24.0F);
    }

    SECTION("Successfully center a mesh")
    {
        tml::mesh mesh{"uncentered_input.ply"};
        mesh.center_mesh();
        auto const& vertices = mesh.vertices();
        REQUIRE(vertices[0].x() == -1.0F);
        REQUIRE(vertices[0].y() == -1.0F);
        REQUIRE(vertices[0].z() == -1.0F);
        REQUIRE(vertices[1].x() == 1.0F);
        REQUIRE(vertices[1].y() == -1.0F);
        REQUIRE(vertices[1].z() == -1.0F);
        REQUIRE(vertices[2].x() == -1.0F);
        REQUIRE(vertices[2].y() == 1.0F);
        REQUIRE(vertices[2].z() == -1.0F);
        REQUIRE(vertices[3].x() == 1.0F);
        REQUIRE(vertices[3].y() == 1.0F);
        REQUIRE(vertices[3].z() == -1.0F);
        REQUIRE(vertices[4].x() == -1.0F);
        REQUIRE(vertices[4].y() == -1.0F);
        REQUIRE(vertices[4].z() == 1.0F);
        REQUIRE(vertices[5].x() == 1.0F);
        REQUIRE(vertices[5].y() == -1.0F);
        REQUIRE(vertices[5].z() == 1.0F);
        REQUIRE(vertices[6].x() == -1.0F);
        REQUIRE(vertices[6].y() == 1.0F);
        REQUIRE(vertices[6].z() == 1.0F);
        REQUIRE(vertices[7].x() == 1.0F);
        REQUIRE(vertices[7].y() == 1.0F);
        REQUIRE(vertices[7].z() == 1.0F);
    }
}
