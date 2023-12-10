#include <catch2/catch_test_macros.hpp>
#include <fmt/core.h>
#include <fstream>
#include <numeric>
#include <tml/mesh.hpp>

TEST_CASE("Meshes tests", "[library]")
{
    SECTION("Successfully save a mesh to a PLY file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.save_to_ply("output.ply", true) == tml::error_code::none);

        std::ifstream const file{"output.ply"};
        REQUIRE(file.good());
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Successfully save a mesh to a STL file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.save_to_stl("output.stl", true) == tml::error_code::none);

        std::ifstream const file{"output.stl"};
        REQUIRE(file.good());
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Successfully save a mesh to a Collada file")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.save_to_collada("output.dae", true) == tml::error_code::none);

        std::ifstream const file{"output.dae"};
        REQUIRE(file.good());
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Save a mesh to a PLY file that already exists")
    {
        tml::mesh const mesh{"input.ply"};

        WHEN("Overwriting is allowed") { REQUIRE(mesh.save_to_ply("output.ply", true) == tml::error_code::none); }
        WHEN("Overwriting is not allowed")
        {
            REQUIRE(mesh.save_to_ply("output.ply", false) == tml::error_code::file_already_exists);
        }
    }

    SECTION("Successfully load a valid PLY file")
    {
        tml::mesh const mesh{"output.ply"};
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Successfully load a valid STL file")
    {
        tml::mesh const mesh{"output.stl"};
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Successfully load a valid Collada file")
    {
        tml::mesh const mesh{"output.dae"};
        REQUIRE(mesh.faces().size() == 12UL);
        REQUIRE(mesh.vertices().size() == 8UL);
    }

    SECTION("Check the adjacent vertices of a vertex in a .ply file")
    {
        tml::mesh const mesh{"output.ply"};
        auto const vertices = mesh.vertices();
        REQUIRE(std::accumulate(vertices.begin(), vertices.end(), 0UL,
                                [](std::size_t const sum, tml::vertex const& vertex) -> std::size_t {
                                    return sum + vertex.neighbors().size();
                                }) == 36UL);
    }

    SECTION("Check the adjacent vertices of a vertex in a .stl file")
    {
        tml::mesh const mesh{"output.stl"};
        auto const vertices = mesh.vertices();
        REQUIRE(std::accumulate(vertices.begin(), vertices.end(), 0UL,
                                [](std::size_t const sum, tml::vertex const& vertex) -> std::size_t {
                                    return sum + vertex.neighbors().size();
                                }) == 36UL);
    }

    SECTION("Check the adjacent vertices of a vertex in a .dae file")
    {
        tml::mesh const mesh{"output.dae"};
        auto const vertices = mesh.vertices();
        REQUIRE(std::accumulate(vertices.begin(), vertices.end(), 0UL,
                                [](std::size_t const sum, tml::vertex const& vertex) -> std::size_t {
                                    return sum + vertex.neighbors().size();
                                }) == 36UL);
    }

    SECTION("Check the surface area of a mesh")
    {
        tml::mesh const mesh{"input.ply"};
        REQUIRE(mesh.area() == 24.0F);
    }

    SECTION("Successfully center a mesh")
    {
        tml::mesh mesh{"uncentered_input.ply"};
        mesh.center();
        auto const vertices = mesh.vertices();
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

    SECTION("Successfully scale a mesh")
    {
        tml::mesh mesh{"input.ply"};
        mesh.scale(2.0F);
        auto const vertices = mesh.vertices();
        REQUIRE(vertices[0].x() == -2.0F);
        REQUIRE(vertices[0].y() == -2.0F);
        REQUIRE(vertices[0].z() == -2.0F);
        REQUIRE(vertices[1].x() == 2.0F);
        REQUIRE(vertices[1].y() == -2.0F);
        REQUIRE(vertices[1].z() == -2.0F);
        REQUIRE(vertices[2].x() == -2.0F);
        REQUIRE(vertices[2].y() == 2.0F);
        REQUIRE(vertices[2].z() == -2.0F);
        REQUIRE(vertices[3].x() == 2.0F);
        REQUIRE(vertices[3].y() == 2.0F);
        REQUIRE(vertices[3].z() == -2.0F);
        REQUIRE(vertices[4].x() == -2.0F);
        REQUIRE(vertices[4].y() == -2.0F);
        REQUIRE(vertices[4].z() == 2.0F);
        REQUIRE(vertices[5].x() == 2.0F);
        REQUIRE(vertices[5].y() == -2.0F);
        REQUIRE(vertices[5].z() == 2.0F);
        REQUIRE(vertices[6].x() == -2.0F);
        REQUIRE(vertices[6].y() == 2.0F);
        REQUIRE(vertices[6].z() == 2.0F);
        REQUIRE(vertices[7].x() == 2.0F);
        REQUIRE(vertices[7].y() == 2.0F);
        REQUIRE(vertices[7].z() == 2.0F);
    }

    SECTION("Successfully invert a mesh")
    {
        tml::mesh mesh{"input.ply"};
        mesh.invert();
        auto const faces = mesh.faces();
        REQUIRE(faces[0].indices() == std::array<std::size_t, 3UL>{0UL, 1UL, 3UL});
        REQUIRE(faces[1].indices() == std::array<std::size_t, 3UL>{0UL, 3UL, 2UL});
        REQUIRE(faces[2].indices() == std::array<std::size_t, 3UL>{1UL, 7UL, 3UL});
        REQUIRE(faces[3].indices() == std::array<std::size_t, 3UL>{7UL, 1UL, 5UL});
        REQUIRE(faces[4].indices() == std::array<std::size_t, 3UL>{7UL, 5UL, 6UL});
        REQUIRE(faces[5].indices() == std::array<std::size_t, 3UL>{4UL, 6UL, 5UL});
        REQUIRE(faces[6].indices() == std::array<std::size_t, 3UL>{2UL, 6UL, 4UL});
        REQUIRE(faces[7].indices() == std::array<std::size_t, 3UL>{0UL, 2UL, 4UL});
        REQUIRE(faces[8].indices() == std::array<std::size_t, 3UL>{3UL, 7UL, 6UL});
        REQUIRE(faces[9].indices() == std::array<std::size_t, 3UL>{6UL, 2UL, 3UL});
        REQUIRE(faces[10].indices() == std::array<std::size_t, 3UL>{0UL, 5UL, 1UL});
        REQUIRE(faces[11].indices() == std::array<std::size_t, 3UL>{5UL, 0UL, 4UL});
    }
}
