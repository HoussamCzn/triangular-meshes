#include "tml/mesh.hpp"

#include <charconv> // std::from_chars
#include <fmt/format.h> // fmt::format
#include <fstream> // std::ifstream
#include <stdexcept> // std::runtime_error

using tml::face;
using tml::mesh;
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
    if (!std::filesystem::exists(filepath)) [[unlikely]]
    {
        return error_code::file_not_found;
    }

    std::ifstream file{filepath};

    if (!file) [[unlikely]]
    {
        return error_code::unknown_io_error;
    }

    static constexpr int vertex_count_offset{15};
    static constexpr int face_count_offset{13};
    std::size_t vertex_count{0};
    std::size_t face_count{0};
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

    for (std::size_t i{0}; i < vertex_count; ++i)
    {
        float x{0.0F};
        float y{0.0F};
        float z{0.0F};
        file >> x >> y >> z;
        m_vertices.emplace_back(x, y, z);
    }

    for (std::size_t i{0}; i < face_count; ++i)
    {
        std::size_t v1{0};
        std::size_t v2{0};
        std::size_t v3{0};
        file >> vertex_count >> v1 >> v2 >> v3;
        m_faces.emplace_back(v1, v2, v3);
        m_vertices[v1].add_adjacent(vertex_count);
        m_vertices[v2].add_adjacent(vertex_count);
        m_vertices[v3].add_adjacent(vertex_count);
    }

    return error_code::none;
}
