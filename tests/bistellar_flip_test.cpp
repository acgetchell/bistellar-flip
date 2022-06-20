/// @file bistellar_test.cpp
/// @brief Apply bistellar_flip to a triangulation
/// @author Adam Getchell
/// @details Test functions defined in bistellar_flip.hpp
/// @date 2020-06-19

#include "bistellar_flip.hpp"
#include <doctest/doctest.h>
#include <numbers>

static inline auto constexpr SQRT_2 = std::numbers::sqrt2_v<double>;
static inline auto constexpr INV_SQRT_2 = 1.0 / SQRT_2;

SCENARIO("Test Delaunay triangulation convenience functions")
{
    GIVEN("A valid Delaunay triangulation")
    {
        // Create a Delaunay triangulation
        std::vector<Point> vertices{
                Point{0, 0, 0},
                Point{INV_SQRT_2, 0, INV_SQRT_2},
                Point{0, INV_SQRT_2, 0},
                Point{-INV_SQRT_2, 0, INV_SQRT_2},
                Point{0, -INV_SQRT_2, INV_SQRT_2},
                Point{0, 0, 2}};
        Delaunay triangulation(vertices.begin(), vertices.end());
        CHECK(triangulation.is_valid());
        WHEN("We obtain all cells in the triangulation")
        {
            auto cells = get_cells(triangulation);
            THEN("We have 4 cells") { REQUIRE_EQ(cells.size(), 4); }
        }
    }
}