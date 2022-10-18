/// @file flip_n_to_m_test.cpp
/// @brief Apply CGAL::flip_n_to_m to a triangulation
/// @author Adam Getchell
/// @details Test functions defined in bistellar_flip.hpp
/// @date 2022-10-17

// clang-format off
#include <CGAL/boost/bimap.hpp>
#include <CGAL/Tetrahedral_remeshing/internal/flip_edges.h>
#include <CGAL/Tetrahedral_remeshing/internal/tetrahedral_adaptive_remeshing_impl.h>
// clang-format on
#include <CGAL/Tetrahedral_remeshing/Remeshing_triangulation_3.h>
//#include <CGAL/tetrahedron_soup_to_triangulation_3.h>

#include <doctest/doctest.h>

#include <boost/optional/optional_io.hpp>
#include <numbers>

#include "bistellar_flip.hpp"

static inline auto constexpr SQRT_2     = std::numbers::sqrt2_v<double>;
static inline auto constexpr INV_SQRT_2 = 1.0 / SQRT_2;

using CellsSmallVector = boost::container::small_vector<Cell_handle, 64>;
using CellsSmallVectorOptional = boost::optional<CellsSmallVector>;
using VertexIncidentCellsMap =
    std::unordered_map<Vertex_handle, CellsSmallVectorOptional>;

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
    WHEN("We convert to a remeshing triangulation")
    {
      using Remeshing_triangulation =
          CGAL::Tetrahedral_remeshing::Remeshing_triangulation_3<
              CGAL::Exact_predicates_inexact_constructions_kernel>;
      //      Remeshing_triangulation triangulation_2 =
      //      CGAL::tetrahedron_soup_to_triangulation_3<Remeshing_triangulation>(triangulation);
      Remeshing_triangulation remeshing_triangulation;
      //      remeshing_triangulation.tds(). = triangulation;
    }
    WHEN("We find the pivot edge in the triangulation")
    {
      auto pivot_edge =
          find_pivot_edge(triangulation, get_finite_edges(triangulation));
      auto incident_cells =
          get_incident_cells(triangulation, pivot_edge.value());
      auto                   vertices = get_vertices(incident_cells.value());
      VertexIncidentCellsMap incident_cells_per_vertex;
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
        REQUIRE_EQ(vertices.size(), 6);
      }
      THEN(
          "We can obtain the container of incident cells to each vertex participating in the bistellar flip")
      {
        for (auto const& vertex : get_vertices(incident_cells.value()))
        {
          CellsSmallVector incident_cells_b;
          // Put the incident cells into incident_cells
          triangulation.tds().incident_cells(
              vertex, std::back_inserter(incident_cells_b));
          // Put the result into an option type
          CellsSmallVectorOptional incident_cells_opt(incident_cells_b);
          // Put the result into a map
          incident_cells_per_vertex.emplace(vertex, incident_cells_opt);
        }
        REQUIRE_EQ(incident_cells_per_vertex.size(), 6);
      }
      THEN("We can call flip_n_to_m")
      {
        //                auto const result =
        //                CGAL::Tetrahedral_remeshing::internal::flip_n_to_m(
        //                    pivot_edge, triangulation, vertices,
        //                    CGAL::Tetrahedral_remeshing::internal::MIN_ANGLE_BASED,
        //                    incident_cells_per_vertex,
        //                    CGAL::Tetrahedral_remeshing::internal::Default_remeshing_visitor());
        //                CHECK_EQ(
        //                    result,
        //                    CGAL::Tetrahedral_remeshing::Sliver_removal_result::VALID_FLIP);
      }
    }
  }
}