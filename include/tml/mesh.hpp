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
    enum class format
    {
        ply,
        stl,
        collada
    };

    class TML_EXPORT mesh
    {
    public:

        mesh() noexcept = default;

        explicit mesh(std::filesystem::path const& filepath);

        [[nodiscard]] auto vertices() const noexcept -> std::span<vertex const>;

        [[nodiscard]] auto faces() const noexcept -> std::span<face const>;

        [[nodiscard]] auto area() const noexcept -> float;

        auto center() noexcept -> mesh&;

        auto invert() noexcept -> mesh&;

        auto scale(float factor) noexcept -> mesh&;

        auto noise(float coefficient) noexcept -> mesh&;

        template <format Format>
        auto read(std::filesystem::path const& filepath) noexcept -> parse_error
        {
            if (filepath.empty()) [[unlikely]]
            {
                return parse_error{.code = error_code::invalid_filepath};
            }

            if constexpr (Format == format::ply)
            {
                return load_from_ply(filepath);
            }
            else if constexpr (Format == format::stl)
            {
                return load_from_stl(filepath);
            }
            else if constexpr (Format == format::collada)
            {
                return load_from_collada(filepath);
            }
        }

        template <format Format>
        auto write(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept -> write_error
        {
            if (filepath.empty()) [[unlikely]]
            {
                return write_error{.code = error_code::invalid_filepath};
            }

            if constexpr (Format == format::ply)
            {
                return save_to_ply(filepath, can_overwrite);
            }
            else if constexpr (Format == format::stl)
            {
                return save_to_stl(filepath, can_overwrite);
            }
            else if constexpr (Format == format::collada)
            {
                return save_to_collada(filepath, can_overwrite);
            }
        }

    private:

        [[nodiscard]] auto load_from_ply(std::filesystem::path const& filepath) noexcept -> parse_error;

        [[nodiscard]] auto load_from_stl(std::filesystem::path const& filepath) noexcept -> parse_error;

        [[nodiscard]] auto load_from_collada(std::filesystem::path const& filepath) noexcept -> parse_error;

        [[nodiscard]] auto save_to_ply(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> write_error;

        [[nodiscard]] auto save_to_stl(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> write_error;

        [[nodiscard]] auto save_to_collada(std::filesystem::path const& filepath, bool can_overwrite = false) const noexcept
            -> write_error;

        std::vector<vertex> m_vertices;
        std::vector<face> m_faces;
    };
} // namespace tml
