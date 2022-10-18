/// @file flip_n_to_m_test.cpp
/// @brief Apply CGAL::flip_n_to_m to a triangulation
/// @author Adam Getchell
/// @details Test functions defined in bistellar_flip.hpp
/// @date 2022-10-17

// clang-format off
#include <doctest/doctest.h>
#include <CGAL/boost/bimap.hpp>
#include <CGAL/Tetrahedral_remeshing/internal/flip_edges.h>
#include <numbers>
// clang-format on

#include "bistellar_flip.hpp"

static inline auto constexpr SQRT_2     = std::numbers::sqrt2_v<double>;
static inline auto constexpr INV_SQRT_2 = 1.0 / SQRT_2;

SCENARIO("Perform bistellar flip on Delaunay triangulation using CGAL " *
         doctest::test_suite("flip_n_to_m"))
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
      auto incident_cells =
          get_incident_cells(triangulation, pivot_edge.value());
      THEN("We have a pivot edge")
      {
        REQUIRE_MESSAGE(pivot_edge, "Pivot edge not found");
        fmt::print("Pivot edge:\n");
        print_edge(pivot_edge.value());
      }
      THEN("We can obtain the cells incident to that edge")
      {
        REQUIRE_EQ(incident_cells->size(), 4);
      }
      THEN("We can obtain the vertices from the cells incident to that edge")
      {
        auto vertices = get_vertices(incident_cells.value());
        REQUIRE_EQ(vertices.size(), 6);
      }
      THEN("We can call flip_n_to_m")
      {
        // Obtain top and bottom vertices by re-inserting, which returns the
        // Vertex_handle
        auto top = triangulation.insert(Point(0, 0, 2));
        // auto visitor = ???

        //        auto const result = CGAL::Tetrahedral_remeshing::internal::
        //            flip_n_to_m(triangulation, pivot_edge.value(), top,
        //            incident_cells, visitor);
      }
    }
  }
}