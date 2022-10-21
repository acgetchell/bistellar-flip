/// @file delaunay_to_remeshing_test.cpp
/// @brief Convert Delaunay_triangulation_3 to Remeshing_triangulation_3
/// @author Adam Getchell
/// @details Convert the relevant data structures, particularly edges and cells,
/// from the Delaunay_triangulation_3 versions to the Remeshing_triangulation_3
/// versions so that we can use the CGAL::flip_n_to_m function.
/// @date 2022-10-20

#include <doctest/doctest.h>

#include <numbers>

#include "bistellar_flip.hpp"

static inline std::floating_point auto constexpr SQRT_2 =
    std::numbers::sqrt2_v<double>;
static inline auto constexpr INV_SQRT_2 = 1.0 / SQRT_2;

SCENARIO("Perform bistellar flip on Delaunay triangulation using CGAL " *
         doctest::test_suite("delaunay_to_remeshing"))
{
  GIVEN("A valid Delaunay triangulation")
  {
    // Create a Delaunay triangulation
    std::vector<Point> points{
        Point{          0,           0,          0},
        Point{ INV_SQRT_2,           0, INV_SQRT_2},
        Point{          0,  INV_SQRT_2,          0},
        Point{-INV_SQRT_2,           0, INV_SQRT_2},
        Point{          0, -INV_SQRT_2, INV_SQRT_2},
        Point{          0,           0,          2}
    };
    Delaunay triangulation(points.begin(), points.end());
    CHECK(triangulation.is_valid());
  }
}