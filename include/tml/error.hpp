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
    };

    static constexpr std::array errors{
        "None"sv,
        "File could not be found"sv,
        "File already exists"sv,
        "Unknown I/O error"sv,
    };

    [[nodiscard]] constexpr auto format_error(error_code const error) noexcept -> std::string_view
    {
        return errors[static_cast<std::size_t>(error)];
    }
} // namespace tml
