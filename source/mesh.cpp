#include "tml/mesh.hpp"

#include "tml/vec3.hpp" // tml::vec3

#include <charconv> // std::from_chars
#include <fmt/format.h> // fmt::format
#include <fstream> // std::ifstream
#include <stdexcept> // std::runtime_error

using tml::face;
using tml::mesh;
using tml::vec3;
using tml::vertex;

mesh::mesh(std::filesystem::path const& filepath)
{
    if (error_code const result = load(filepath); result != error_code::none) [[unlikely]]
    {
        throw std::runtime_error{fmt::format("Failed to load mesh: {}", format_error(result))};
    }
}

auto mesh::get_vertices() const noexcept -> std::vector<vertex> const& { return m_vertices; }

auto mesh::get_faces() const noexcept -> std::vector<face> const& { return m_faces; }

auto mesh::surface_area() const noexcept -> float
{
    float area{0.0F};

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.get_indices();
        vec3 const v1{m_vertices[index_v1].x(), m_vertices[index_v1].y(), m_vertices[index_v1].z()};
        vec3 const v2{m_vertices[index_v2].x(), m_vertices[index_v2].y(), m_vertices[index_v2].z()};
        vec3 const v3{m_vertices[index_v3].x(), m_vertices[index_v3].y(), m_vertices[index_v3].z()};
        vec3 const edge1{v2.x() - v1.x(), v2.y() - v1.y(), v2.z() - v1.z()};
        vec3 const edge2{v3.x() - v1.x(), v3.y() - v1.y(), v3.z() - v1.z()};

        area += 0.5F * edge1.cross(edge2).norm();
    }

    return area;
}

auto mesh::save(std::filesystem::path const& filepath, bool can_overwrite) const noexcept -> error_code
{
    if (!can_overwrite && std::filesystem::exists(filepath)) [[unlikely]]
    {
        return error_code::file_already_exists;
    }

    std::ofstream file{filepath};

    if (!file) [[unlikely]]
    {
        return error_code::unknown_io_error;
    }

    file << fmt::format(
        "ply\nformat ascii 1.0\nelement vertex {}\nproperty float x\nproperty float y\nproperty float z\nelement face "
        "{}\nproperty list uchar int vertex_indices\nend_header\n",
        m_vertices.size(), m_faces.size());

    for (auto const& vertex : m_vertices)
    {
        file << fmt::format("{} {} {}\n", vertex.x(), vertex.y(), vertex.z());
    }

    for (auto const& face : m_faces)
    {
        std::array indices = face.get_indices();
        file << fmt::format("3 {} {} {}\n", indices[0], indices[1], indices[2]);
    }

    return error_code::none;
}

auto mesh::load(std::filesystem::path const& filepath) noexcept -> error_code
{
    std::ifstream file{filepath};

    if (!file) [[unlikely]]
    {
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    static constexpr std::ptrdiff_t vertex_count_offset{15};
    static constexpr std::ptrdiff_t face_count_offset{13};
    std::size_t vertex_count{0UL};
    std::size_t face_count{0UL};
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with("element vertex"))
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            std::from_chars(line.data() + vertex_count_offset, line.data() + line.size(), vertex_count);
        }
        else if (line.starts_with("element face"))
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
            std::from_chars(line.data() + face_count_offset, line.data() + line.size(), face_count);
        }
        else if (line.starts_with("end_header"))
        {
            break;
        }
    }

    m_vertices.reserve(vertex_count);
    m_faces.reserve(face_count);

    for (std::size_t i{0UL}; i < vertex_count; ++i)
    {
        float x{0.0F};
        float y{0.0F};
        float z{0.0F};
        file >> x >> y >> z;
        m_vertices.emplace_back(x, y, z);
    }

    for (std::size_t i{0UL}; i < face_count; ++i)
    {
        std::size_t v1{0UL};
        std::size_t v2{0UL};
        std::size_t v3{0UL};
        file >> vertex_count >> v1 >> v2 >> v3;
        m_faces.emplace_back(v1, v2, v3);

        m_vertices[v1].add_adjacent(v2);
        m_vertices[v1].add_adjacent(v3);
        m_vertices[v2].add_adjacent(v1);
        m_vertices[v2].add_adjacent(v3);
        m_vertices[v3].add_adjacent(v1);
        m_vertices[v3].add_adjacent(v2);
    }

    return error_code::none;
}
