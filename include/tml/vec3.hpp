#pragma once

#include "tml/config.hpp" // TML_EXPORT

namespace tml
{
    class TML_EXPORT [[nodiscard]] vec3
    {
    public:

        vec3(float x, float y, float z) noexcept;

        auto x() const noexcept -> float;

        auto y() const noexcept -> float;

        auto z() const noexcept -> float;

        auto norm() const noexcept -> float;

        auto cross(vec3 const& other) const noexcept -> vec3;

        auto operator==(vec3 const& other) const noexcept -> bool;

        auto operator!=(vec3 const& other) const noexcept -> bool;

        friend auto operator+(vec3 const& lhs, vec3 const& rhs) noexcept -> vec3;

        friend auto operator-(vec3 const& lhs, vec3 const& rhs) noexcept -> vec3;

        friend auto operator-(vec3 const& rhs) noexcept -> vec3;

        friend auto operator*(vec3 const& lhs, float scalar) noexcept -> vec3;

        friend auto operator*(float scalar, vec3 const& rhs) noexcept -> vec3;

        friend auto operator+=(vec3& lhs, vec3 const& rhs) noexcept -> vec3&;

        friend auto operator-=(vec3& lhs, vec3 const& rhs) noexcept -> vec3&;

        friend auto operator*=(vec3& lhs, float scalar) noexcept -> vec3&;

    private:

        float m_x, m_y, m_z;
    };
} // namespace tml
