#include "tml/mesh.hpp"

#include "tml/vec3.hpp" // tml::vec3

#include <algorithm> // std::min, std::max
#include <bit> // std::bit_cast
#include <charconv> // std::from_chars
#include <fmt/format.h> // fmt::format
#include <fstream> // std::ifstream
#include <random> // std::mt19937, std::uniform_real_distribution, std::random_device
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

auto mesh::vertices() const noexcept -> std::span<vertex const> { return m_vertices; }

auto mesh::faces() const noexcept -> std::span<face const> { return m_faces; }

auto mesh::area() const noexcept -> float
{
    float area{0.0F};

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.indices();
        vec3 const v1{m_vertices[index_v1].x(), m_vertices[index_v1].y(), m_vertices[index_v1].z()};
        vec3 const v2{m_vertices[index_v2].x(), m_vertices[index_v2].y(), m_vertices[index_v2].z()};
        vec3 const v3{m_vertices[index_v3].x(), m_vertices[index_v3].y(), m_vertices[index_v3].z()};
        vec3 const edge1{v2.x() - v1.x(), v2.y() - v1.y(), v2.z() - v1.z()};
        vec3 const edge2{v3.x() - v1.x(), v3.y() - v1.y(), v3.z() - v1.z()};

        area += 0.5F * edge1.cross(edge2).norm();
    }

    return area;
}

auto mesh::center() noexcept -> mesh&
{
    vec3 min{m_vertices.front().x(), m_vertices.front().y(), m_vertices.front().z()};
    vec3 max{min};

    for (auto const& vertex : m_vertices)
    {
        min = vec3{std::min(min.x(), vertex.x()), std::min(min.y(), vertex.y()), std::min(min.z(), vertex.z())};
        max = vec3{std::max(max.x(), vertex.x()), std::max(max.y(), vertex.y()), std::max(max.z(), vertex.z())};
    }

    vec3 const center{(max + min) * 0.5F};

    for (auto& vertex : m_vertices)
    {
        vertex.translate(-center);
    }

    return *this;
}

auto mesh::invert() noexcept -> mesh&
{
    for (auto& face : m_faces)
    {
        face.invert();
    }

    return *this;
}

auto mesh::scale(float factor) noexcept -> mesh&
{
    for (auto& vertex : m_vertices)
    {
        vertex.scale(factor);
    }

    return *this;
}

auto mesh::noise(float coefficient) noexcept -> mesh&
{
    std::mt19937 generator{std::random_device{}()};
    std::uniform_real_distribution<float> distribution{-coefficient, coefficient};

    for (auto& vertex : m_vertices)
    {
        vertex.translate(vec3{distribution(generator), distribution(generator), distribution(generator)});
    }

    return *this;
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
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    file << fmt::format(
        "ply\nformat ascii 1.0\nelement vertex {}\nproperty float x\nproperty float y\nproperty float z\nelement face "
        "{}\nproperty list uchar int vertex_indices\nend_header\n",
        m_vertices.size(), m_faces.size());

    for (auto const& vertex : m_vertices)
    {
        float const x{vertex.x()};
        float const y{vertex.y()};
        float const z{vertex.z()};
        file.write(std::bit_cast<char const*>(&x), sizeof(float));
        file.write(std::bit_cast<char const*>(&y), sizeof(float));
        file.write(std::bit_cast<char const*>(&z), sizeof(float));
        file << '\n';
    }

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.indices();
        file.write(std::bit_cast<char const*>(&index_v1), sizeof(decltype(index_v1)));
        file.write(std::bit_cast<char const*>(&index_v2), sizeof(decltype(index_v2)));
        file.write(std::bit_cast<char const*>(&index_v3), sizeof(decltype(index_v3)));
        file << '\n';
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

        m_vertices[v1].add_neighbor(v2);
        m_vertices[v1].add_neighbor(v3);
        m_vertices[v2].add_neighbor(v1);
        m_vertices[v2].add_neighbor(v3);
        m_vertices[v3].add_neighbor(v1);
        m_vertices[v3].add_neighbor(v2);
    }

    return error_code::none;
}
