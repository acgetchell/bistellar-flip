/// @file bistellar_test.cpp
/// @brief Apply bistellar_flip to a triangulation
/// @author Adam Getchell
/// @details Test functions defined in bistellar_flip.hpp
/// @date 2020-06-19

#include "bistellar_flip.hpp"

#include <doctest/doctest.h>

#include <numbers>

static inline auto constexpr SQRT_2     = std::numbers::sqrt2_v<double>;
static inline auto constexpr INV_SQRT_2 = 1.0 / SQRT_2;

SCENARIO("Test Delaunay triangulation convenience functions" *
         doctest::test_suite("bistellar"))
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
    auto edges = get_finite_edges(triangulation);
    WHEN("We get all finite cells in the triangulation")
    {
      auto cells = get_finite_cells(triangulation);
      THEN("We have 4 cells") { REQUIRE_EQ(cells.size(), 4); }
    }
    WHEN("We get all finite edges in the triangulation")
    {
      THEN("We have 13 edges") { REQUIRE_EQ(edges.size(), 13); }
    }
    WHEN("We find the pivot edge in the triangulation")
    {
      auto pivot_edge = find_pivot_edge(triangulation, edges);
      auto Contains   = [&points](Point point) {
        return std::any_of(points.begin(), points.end(),
                             [&point](Point p) { return p == point; });
      };
      THEN("We have a pivot edge")
      {
        CHECK_MESSAGE(pivot_edge, "Pivot edge found");
        REQUIRE(triangulation.tds().is_edge(
            pivot_edge->first, pivot_edge->second, pivot_edge->third));
        auto pivot_from_1 =
            pivot_edge->first->vertex(pivot_edge->second)->point();
        auto pivot_from_2 =
            pivot_edge->first->vertex(pivot_edge->third)->point();
        // Verify Contains
        REQUIRE_FALSE(Contains(Point{0, 0, 1}));
        REQUIRE(Contains(pivot_from_1));
        REQUIRE(Contains(pivot_from_2));
      }
    }
  }
}