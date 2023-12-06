#pragma once

#include "tml/config.hpp" // TML_EXPORT

#include <array> // std::array

namespace tml
{
    class TML_EXPORT face
    {
    public:

        face(std::size_t v1, std::size_t v2, std::size_t v3) noexcept;

        [[nodiscard]] auto get_indices() const noexcept -> std::array<std::size_t, 3> const&;

        auto invert() noexcept -> face&;

    private:

        std::array<std::size_t, 3> m_indices;
    };
} // namespace tml
