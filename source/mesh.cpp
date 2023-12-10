#include "tml/mesh.hpp"

#include "tml/vec3.hpp" // tml::vec3

#include <algorithm> // std::min, std::max
#include <charconv> // std::from_chars
#include <fmt/format.h> // fmt::format
#include <fstream> // std::ifstream
#include <pugixml.hpp> // pugi::xml_document, pugi::xml_parse_result
#include <random> // std::mt19937, std::uniform_real_distribution, std::random_device
#include <ranges> // std::views::iota
#include <sstream> // std::istringstream
#include <stdexcept> // std::runtime_error
#include <unordered_map> // std::unordered_map
#include <vector> // std::vector

using tml::face;
using tml::mesh;
using tml::vertex;

mesh::mesh(std::filesystem::path const& filepath)
{
    error_code result = error_code::none;

    if (filepath.extension() == ".ply")
    {
        result = load_from_ply(filepath);
    }
    else if (filepath.extension() == ".stl")
    {
        result = load_from_stl(filepath);
    }
    else if (filepath.extension() == ".dae")
    {
        result = load_from_collada(filepath);
    }
    else [[unlikely]]
    {
        throw std::runtime_error{fmt::format("Failed to load mesh: Unsupported file extension")};
    }

    if (result != error_code::none) [[unlikely]]
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

auto mesh::save_to_ply(std::filesystem::path const& filepath, bool can_overwrite) const noexcept -> error_code
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
        file << fmt::format("{} {} {}\n", vertex.x(), vertex.y(), vertex.z());
    }

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.indices();
        file << fmt::format("3 {} {} {}\n", index_v1, index_v2, index_v3);
    }

    return error_code::none;
}

auto mesh::save_to_stl(std::filesystem::path const& filepath, bool can_overwrite) const noexcept -> error_code
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

    file << fmt::format("solid {}\n", filepath.stem().string());

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.indices();
        vec3 const v1{m_vertices[index_v1].x(), m_vertices[index_v1].y(), m_vertices[index_v1].z()};
        vec3 const v2{m_vertices[index_v2].x(), m_vertices[index_v2].y(), m_vertices[index_v2].z()};
        vec3 const v3{m_vertices[index_v3].x(), m_vertices[index_v3].y(), m_vertices[index_v3].z()};
        vec3 const edge1{v2.x() - v1.x(), v2.y() - v1.y(), v2.z() - v1.z()};
        vec3 const edge2{v3.x() - v1.x(), v3.y() - v1.y(), v3.z() - v1.z()};
        vec3 const normal{edge1.cross(edge2)};

        file << fmt::format(
            "facet normal {} {} {}\nouter loop\nvertex {} {} {}\nvertex {} {} {}\nvertex {} {} {}\nendloop\nendfacet\n",
            normal.x(), normal.y(), normal.z(), v1.x(), v1.y(), v1.z(), v2.x(), v2.y(), v2.z(), v3.x(), v3.y(), v3.z());
    }

    file << fmt::format("endsolid {}\n", filepath.stem().string());

    return error_code::none;
}

auto mesh::save_to_collada(std::filesystem::path const& filepath, bool can_overwrite) const noexcept -> error_code
{
    if (!can_overwrite && std::filesystem::exists(filepath))
    {
        return error_code::file_already_exists;
    }

    std::ofstream file{filepath};

    if (!file)
    {
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    file << fmt::format(
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<COLLADA version=\"1.5.0\">\n<library_geometries>\n<geometry "
        "id=\"mesh\">\n<mesh>\n<source id=\"mesh-coords\">\n<float_array id=\"mesh-coords-array\" count=\"{}\">",
        m_vertices.size() * 3);

    for (auto const& vertex : m_vertices)
    {
        file << fmt::format("{} {} {} ", vertex.x(), vertex.y(), vertex.z());
    }

    file << "</float_array>\n<technique_common>\n<accessor count=\"" << m_vertices.size()
         << "\" offset=\"0\" source=\"#mesh-coords-array\" stride=\"3\">\n<param name=\"X\" type=\"float\"/>\n<param name=\"Y\" "
            "type=\"float\"/>\n<param name=\"Z\" type=\"float\"/>\n</accessor>\n</technique_common>\n</source>\n<vertices "
            "id=\"mesh-vertices\">\n<input semantic=\"POSITION\" source=\"#mesh-coords\"/>\n</vertices>\n<triangles count=\""
         << m_faces.size() << "\">\n<input offset=\"0\" semantic=\"VERTEX\" source=\"#mesh-vertices\"/>\n<p>";

    for (auto const& face : m_faces)
    {
        auto const [index_v1, index_v2, index_v3] = face.indices();
        file << fmt::format("{} {} {} ", index_v1, index_v2, index_v3);
    }

    file << "</p>\n</triangles>\n</mesh>\n</geometry>\n</library_geometries>\n</COLLADA>";

    return error_code::none;
}

auto mesh::load_from_ply(std::filesystem::path const& filepath) noexcept -> error_code
{
    std::ifstream file{filepath};

    if (!file) [[unlikely]]
    {
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    static constexpr std::ptrdiff_t vertex_count_offset{15L};
    static constexpr std::ptrdiff_t face_count_offset{13L};
    std::size_t vertex_count{0UL};
    std::size_t face_count{0UL};
    std::string line;

    while (std::getline(file, line))
    {
        if (line.starts_with("end_header"))
        {
            break;
        }

        auto const* start = line.data();
        auto const* end = line.data();
        std::advance(end, line.size());

        if (line.starts_with("element vertex"))
        {
            std::advance(start, vertex_count_offset);
            auto const [ptr, ec] = std::from_chars(start, end, vertex_count);

            if (ec != std::errc()) [[unlikely]]
            {
                return error_code::invalid_data;
            }
        }
        else if (line.starts_with("element face"))
        {
            std::advance(start, face_count_offset);
            auto const [ptr, ec] = std::from_chars(start, end, face_count);

            if (ec != std::errc()) [[unlikely]]
            {
                return error_code::invalid_data;
            }
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

auto mesh::load_from_stl(std::filesystem::path const& filepath) noexcept -> error_code
{
    std::ifstream file{filepath};
    if (!file)
    {
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    std::string line;
    std::unordered_map<vertex, std::size_t> vertex_indices;

    while (std::getline(file, line))
    {
        if (line.starts_with("vertex"))
        {
            char const* start = line.data();
            char const* end = line.data();
            float x{0.0F};
            float y{0.0F};
            float z{0.0F};

            std::advance(start, 7);
            std::advance(end, line.size());

            auto [ptr, ec] = std::from_chars(start, end, x);
            if (ec != std::errc()) [[unlikely]]
            {
                return error_code::invalid_data;
            }
            std::advance(ptr, 1);

            auto [ptr2, ec2] = std::from_chars(ptr, end, y);
            if (ec2 != std::errc()) [[unlikely]]
            {
                return error_code::invalid_data;
            }
            std::advance(ptr2, 1);

            auto [_, ec3] = std::from_chars(ptr2, end, z);
            if (ec3 != std::errc()) [[unlikely]]
            {
                return error_code::invalid_data;
            }

            auto const [it, inserted] = vertex_indices.try_emplace(vertex{x, y, z}, m_vertices.size());
            if (inserted)
            {
                m_vertices.emplace_back(x, y, z);
            }
        }
        else if (line.starts_with("endloop"))
        {
            std::size_t const v1 = vertex_indices[m_vertices[m_vertices.size() - 3]];
            std::size_t const v2 = vertex_indices[m_vertices[m_vertices.size() - 2]];
            std::size_t const v3 = vertex_indices[m_vertices[m_vertices.size() - 1]];
            m_faces.emplace_back(v1, v2, v3);

            m_vertices[v1].add_neighbor(v2);
            m_vertices[v1].add_neighbor(v3);
            m_vertices[v2].add_neighbor(v1);
            m_vertices[v2].add_neighbor(v3);
            m_vertices[v3].add_neighbor(v1);
            m_vertices[v3].add_neighbor(v2);
        }
    }

    return error_code::none;
}

auto mesh::load_from_collada(std::filesystem::path const& filepath) noexcept -> error_code
{
    using pugi::xml_document;
    using pugi::xml_parse_result;

    xml_document document;
    xml_parse_result const result = document.load_file(filepath.c_str());

    if (!result) [[unlikely]]
    {
        return std::filesystem::exists(filepath) ? error_code::unknown_io_error : error_code::file_not_found;
    }

    auto const library_geometries = document.child("COLLADA").child("library_geometries");

    for (auto const& geometry : library_geometries)
    {
        for (auto const& mesh : geometry.children("mesh"))
        {
            for (auto const& source : mesh.children("source"))
            {
                auto const* float_array = source.child("float_array").child_value();
                std::istringstream iss(float_array);
                std::vector<float> vertex_data(std::istream_iterator<float>{iss}, {});

                if (vertex_data.size() % 3 != 0) [[unlikely]]
                {
                    return error_code::invalid_data;
                }

                for (auto const idx : std::views::iota(0UL, vertex_data.size() / 3))
                {
                    m_vertices.emplace_back(vertex_data[idx * 3], vertex_data[idx * 3 + 1], vertex_data[idx * 3 + 2]);
                }
            }

            for (auto const& triangles : mesh.children("triangles"))
            {
                std::istringstream iss(triangles.child("p").child_value());
                std::vector<size_t> face_data(std::istream_iterator<size_t>{iss}, {});

                if (face_data.size() % 3 != 0) [[unlikely]]
                {
                    return error_code::invalid_data;
                }

                for (auto const idx : std::views::iota(0UL, face_data.size() / 3))
                {
                    std::size_t const v1 = face_data[idx * 3];
                    std::size_t const v2 = face_data[idx * 3 + 1];
                    std::size_t const v3 = face_data[idx * 3 + 2];
                    m_faces.emplace_back(v1, v2, v3);

                    m_vertices[v1].add_neighbor(v2);
                    m_vertices[v1].add_neighbor(v3);
                    m_vertices[v2].add_neighbor(v1);
                    m_vertices[v2].add_neighbor(v3);
                    m_vertices[v3].add_neighbor(v1);
                    m_vertices[v3].add_neighbor(v2);
                }
            }
        }
    }

    return error_code::none;
}
