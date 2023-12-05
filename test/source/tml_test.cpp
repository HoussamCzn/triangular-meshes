#include "tml/tml.hpp"

#include <catch2/catch_test_macros.hpp>
#include <string>

TEST_CASE("Name is tml", "[library]")
{
    auto const exported = exported_class{};
    REQUIRE(std::string("tml") == exported.name());
}
