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
         doctest::test_suite("bistellar_flip"))
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
    WHEN("We get all finite vertices in the triangulation")
    {
      THEN("We have 6 vertices")
      {
        auto vertices = get_finite_vertices(triangulation);
        REQUIRE_EQ(vertices.size(), 6);
      }
    }
  }
}

SCENARIO("Perform bistellar flip on Delaunay triangulation" *
         doctest::test_suite("bistellar_flip"))
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
    WHEN("We find the pivot edge in the triangulation")
    {
      auto pivot_edge =
          find_pivot_edge(triangulation, get_finite_edges(triangulation));
      THEN("We have a pivot edge")
      {
        CHECK_MESSAGE(pivot_edge, "Pivot edge found");
        print_edge(pivot_edge.value());
      }
      THEN("We can perform a bistellar flip")
      {
        // Obtain top and bottom vertices by re-inserting, which returns the
        // Vertex_handle
        auto top    = triangulation.insert(Point(0, 0, 2));
        auto bottom = triangulation.insert(Point(0, 0, 0));
        // Check this didn't actually change vertices in the triangulation
        REQUIRE_EQ(points.size(), 6);
        auto flipped_triangulation =
            bistellar_flip(triangulation, pivot_edge.value(), top, bottom);
        REQUIRE(flipped_triangulation);
        REQUIRE(flipped_triangulation->is_valid());
      }
    }
  }
}