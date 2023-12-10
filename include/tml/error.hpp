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
        invalid_filepath,
    };

    static constexpr std::array errors{
        "None"sv,
        "The file or directory does not exist"sv,
        "The specified file already exists"sv,
        "An unknown I/O error occurred"sv,
        "Read data is invalid, the file might be corrupted"sv,
        "The provided filepath is not valid"sv,
    };

    [[nodiscard]] constexpr auto format_error(error_code const error) noexcept -> std::string_view
    {
        return errors[static_cast<std::size_t>(error)];
    }

    struct parse_error
    {
        error_code code{};

        constexpr explicit operator bool() const noexcept { return code != error_code::none; }

        constexpr auto operator!() const noexcept -> bool { return !this->operator bool(); }

        constexpr auto operator==(parse_error const& other) const noexcept -> bool { return code == other.code; }

        constexpr auto operator!=(parse_error const& other) const noexcept -> bool { return !(*this == other); }

        constexpr auto operator=(error_code const error) noexcept -> parse_error&
        {
            code = error;
            return *this;
        }

        [[nodiscard]] constexpr auto message() const noexcept -> std::string_view { return format_error(code); }
    };

    struct write_error
    {
        error_code code{};

        constexpr explicit operator bool() const noexcept { return code != error_code::none; }

        constexpr auto operator!() const noexcept -> bool { return !this->operator bool(); }

        constexpr auto operator==(write_error const& other) const noexcept -> bool { return code == other.code; }

        constexpr auto operator!=(write_error const& other) const noexcept -> bool { return !(*this == other); }

        constexpr auto operator=(error_code const error) noexcept -> write_error&
        {
            code = error;
            return *this;
        }

        [[nodiscard]] constexpr auto message() const noexcept -> std::string_view { return format_error(code); }
    };
} // namespace tml
