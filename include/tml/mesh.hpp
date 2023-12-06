#pragma once

#include "tml/config.hpp" // TML_EXPORT
#include "tml/error.hpp" // tml::error_code
#include "tml/face.hpp" // tml::face
#include "tml/vertex.hpp" // tml::vertex

#include <filesystem> // std::filesystem::path, std::filesystem::exists
#include <string_view> // std::string_view
#include <vector> // std::vector

namespace tml
{
    class TML_EXPORT mesh
    {
    public:

        explicit mesh(std::filesystem::path const& filepath);

        [[nodiscard]] auto get_vertices() const noexcept -> std::vector<vertex> const&;

        [[nodiscard]] auto get_faces() const noexcept -> std::vector<face> const&;

        [[nodiscard]] auto surface_area() const noexcept -> float;

        auto invert_normals() noexcept -> mesh&;

        [[nodiscard]] auto save(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept -> error_code;

    private:

        [[nodiscard]] auto load(std::filesystem::path const& filepath) noexcept -> error_code;

        std::vector<vertex> m_vertices;
        std::vector<face> m_faces;
    };
} // namespace tml
