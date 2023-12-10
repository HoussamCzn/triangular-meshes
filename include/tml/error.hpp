#pragma once

#include <array> // std::array
#include <string_view> // std::string_view

namespace tml
{
    using std::operator""sv;

    enum class error_code
    {
        none,
        file_not_found,
        file_already_exists,
        unknown_io_error,
        invalid_data,
    };

    [[nodiscard]] constexpr auto operator!(error_code const error) noexcept -> bool
    {
        return error == error_code::none;
    }

    [[nodiscard]] constexpr auto operator==(error_code const lhs, error_code const rhs) noexcept -> bool
    {
        return static_cast<std::size_t>(lhs) == static_cast<std::size_t>(rhs);
    }

    [[nodiscard]] constexpr auto operator!=(error_code const lhs, error_code const rhs) noexcept -> bool
    {
        return !(lhs == rhs);
    }

    static constexpr std::array errors{
        "None"sv, "File or directory does not exist"sv, "File already exists"sv, "Unknown I/O error"sv, "Invalid data"sv,
    };

    [[nodiscard]] constexpr auto format_error(error_code const error) noexcept -> std::string_view
    {
        return errors[static_cast<std::size_t>(error)];
    }
} // namespace tml
