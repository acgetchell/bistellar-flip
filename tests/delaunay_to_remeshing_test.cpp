/// @file delaunay_to_remeshing_test.cpp
/// @brief Convert Delaunay_triangulation_3 to Remeshing_triangulation_3
/// @author Adam Getchell
/// @details Convert the relevant data structures, particularly edges and cells,
/// from the Delaunay_triangulation_3 versions to the Remeshing_triangulation_3
/// versions so that we can use the CGAL::flip_n_to_m function.
/// @date 2022-10-20

#include <doctest/doctest.h>

#include <numbers>
#include <CGAL/Tetrahedral_remeshing/Remeshing_triangulation_3.h>
#include <CGAL/Mesh_complex_3_in_triangulation_3.h>
//#include <CGAL/SMDS_3/tetrahedron_soup_to_mesh.h>
#include "bistellar_flip.hpp"

using Remeshing_triangulation = CGAL::Tetrahedral_remeshing::Remeshing_triangulation_3<K>;
using C3T3 = CGAL::Mesh_complex_3_in_triangulation_3<Remeshing_triangulation>;

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
//    Remeshing_triangulation tr = CGAL::tetrahedron_soup_to_triangulation_3<Remeshing_triangulation>(triangulation.tds().finite_edges_begin(), triangulation.tds().finite_edges_end());
//    C3T3 c3t3;
//    c3t3.triangulation() = tr;
  }
}