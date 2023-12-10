#pragma once

#include "tml/config.hpp" // TML_EXPORT
#include "tml/error.hpp" // tml::error_code
#include "tml/face.hpp" // tml::face
#include "tml/vertex.hpp" // tml::vertex

#include <filesystem> // std::filesystem::path, std::filesystem::exists
#include <span> // std::span
#include <vector> // std::vector

namespace tml
{
    class TML_EXPORT mesh
    {
    public:

        explicit mesh(std::filesystem::path const& filepath);

        [[nodiscard]] auto vertices() const noexcept -> std::span<vertex const>;

        [[nodiscard]] auto faces() const noexcept -> std::span<face const>;

        [[nodiscard]] auto area() const noexcept -> float;

        auto center() noexcept -> mesh&;

        auto invert() noexcept -> mesh&;

        auto scale(float factor) noexcept -> mesh&;

        auto noise(float coefficient) noexcept -> mesh&;

        [[nodiscard]] auto save_to_ply(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> error_code;

        [[nodiscard]] auto save_to_stl(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> error_code;

        [[nodiscard]] auto save_to_collada(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> error_code;

    private:

        [[nodiscard]] auto load_from_ply(std::filesystem::path const& filepath) noexcept -> error_code;

        [[nodiscard]] auto load_from_stl(std::filesystem::path const& filepath) noexcept -> error_code;

        [[nodiscard]] auto load_from_collada(std::filesystem::path const& filepath) noexcept -> error_code;

        std::vector<vertex> m_vertices;
        std::vector<face> m_faces;
    };
} // namespace tml
