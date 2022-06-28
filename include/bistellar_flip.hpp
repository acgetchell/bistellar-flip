/// @file bistellar.cpp
/// @brief Example bistellar fip
/// @author Adam Getchell
/// @details Show how to use the bistellar_flip functions on a 3D triangulation.
/// Some convenience functions are defined here because the internal
/// functions of the Triangulation_3 class are not currently accessible to
/// the bistellar_flip functions.
/// @date Created: 2022-06-19

#ifndef BISTELLAR_FLIP_BISTELLAR_FLIP_HPP
#define BISTELLAR_FLIP_BISTELLAR_FLIP_HPP

#include <CGAL/circulator.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_cell_base_with_info_3.h>
#include <CGAL/Triangulation_vertex_base_with_info_3.h>
#include <fmt/format.h>

#include <iostream>
#include <optional>
#include <sstream>
#include <string>

using K   = CGAL::Exact_predicates_inexact_constructions_kernel;
using Vb  = CGAL::Triangulation_vertex_base_with_info_3<int, K>;
using Cb  = CGAL::Triangulation_cell_base_with_info_3<int, K>;
using Tds = CGAL::Triangulation_data_structure_3<Vb, Cb, CGAL::Sequential_tag>;
using Delaunay         = CGAL::Delaunay_triangulation_3<K, Tds>;
using Cell_handle      = Delaunay::Cell_handle;
using Edge_handle      = CGAL::Triple<Cell_handle, int, int>;
using Vertex_handle    = Delaunay::Vertex_handle;
using Point            = Delaunay::Point;
using Cell_container   = std::vector<Cell_handle>;
using Edge_container   = std::vector<Edge_handle>;
using Vertex_container = std::vector<Vertex_handle>;

/// @return A container of all the finite cells in the triangulation.
[[nodiscard]] inline auto get_finite_cells(Delaunay const& triangulation)
    -> Cell_container
{
  Cell_container cells;
  for (auto cit = triangulation.finite_cells_begin();
       cit != triangulation.finite_cells_end(); ++cit)
  {
    // Each cell handle is valid
    assert(triangulation.tds().is_cell(cit));
    cells.emplace_back(cit);
  }
  return cells;
}  // get_finite_cells()

/// @return A container of all the finite edges in the triangulation.
[[nodiscard]] inline auto get_finite_edges(Delaunay const& triangulation)
    -> Edge_container
{
  Edge_container edges;
  for (auto eit = triangulation.finite_edges_begin();
       eit != triangulation.finite_edges_end(); ++eit)
  {
    Cell_handle const cell = eit->first;
    Edge_handle const edge{cell, cell->index(cell->vertex(eit->second)),
                           cell->index(cell->vertex(eit->third))};
    // Each edge handle is valid
    assert(triangulation.tds().is_valid(edge.first, edge.second, edge.third));
    edges.emplace_back(edge);
  }
  return edges;
}  // get_finite_edges()

/// @return An edge with 4 incident finite cells
[[nodiscard]] inline auto find_pivot_edge(Delaunay const&       triangulation,
                                          Edge_container const& edges)
    -> std::optional<Edge_handle>
{
  for (auto const& edge : edges)
  {
    auto           circulator = triangulation.incident_cells(edge, edge.first);
    Cell_container incident_cells;
    do {
      // filter out boundary edges with incident infinite cells
      if (!triangulation.is_infinite(circulator))
      {
        incident_cells.emplace_back(circulator);
      }
    }
    while (++circulator != edge.first);
    if (incident_cells.size() == 4) { return edge; }
  }
  return std::nullopt;
}  // find_pivot_edge()

/// @return A container of all finite vertices in the triangulation.
[[nodiscard]] inline auto get_finite_vertices(Delaunay const& triangulation)
    -> Vertex_container
{
  Vertex_container vertices;
  for (auto vit = triangulation.finite_vertices_begin();
       vit != triangulation.finite_vertices_end(); ++vit)
  {
    assert(triangulation.tds().is_vertex(vit));
    vertices.emplace_back(vit);
  }
  return vertices;
}  // get_finite_vertices()

/// @brief Print the edge in human-readable form.
/// @param edge The edge to print.
inline void print_edge(Edge_handle const& edge)
{
  auto              Point1 = edge.first->vertex(edge.second)->point();
  std::stringstream point1_ss;
  point1_ss << Point1;
  auto              Point2 = edge.first->vertex(edge.third)->point();
  std::stringstream point2_ss;
  point2_ss << Point2;
  fmt::print("Point {} ({}) -> Point {} ({})\n", edge.second, point1_ss.str(),
             edge.third, point2_ss.str());
}  // print_edge()

/// @brief Return a container of cells incident to an edge.
/// @param triangulation The triangulation with the cells.
/// @param edge The edge to find the incident cells.
/// @return A container of cells incident to an edge, or std::nullopt
[[nodiscard]] auto get_incident_cells(Delaunay const&    triangulation,
                                      Edge_handle const& edge)
    -> std::optional<Cell_container>
{
  if (!triangulation.tds().is_valid(edge.first, edge.second, edge.third))
  {
    return std::nullopt;
  }
  auto           circulator = triangulation.incident_cells(edge, edge.first);
  Cell_container incident_cells;
  do {
    // filter out boundary edges with incident infinite cells
    if (!triangulation.is_infinite(circulator))
    {
      incident_cells.emplace_back(circulator);
    }
  }
  while (++circulator != edge.first);

  return incident_cells;
}  // incident_cells_from_edge()

/// @brief Return a container of vertices from a container of cells.
/// @param cells The container of cells.
/// @return A container of vertices in the cells
[[nodiscard]] auto get_vertices_from_cells(Cell_container const& cells)
{
  std::unordered_set<Vertex_handle> vertices;
  auto get_vertices = [&vertices](auto const& cell) {
    for (int i = 0; i < 4; ++i) { vertices.emplace(cell->vertex(i)); }
  };
  std::for_each(cells.begin(), cells.end(), get_vertices);
  Vertex_container result{vertices.begin(), vertices.end()};
  return result;
}  // get_vertices_from_cells()

/// @brief Perform a bistellar flip on triangulation via the given edge
/// @param triangulation The triangulation to flip
/// @param edge The edge to pivot on
/// @param top Top vertex of the cells being flipped
/// @param bottom Bottom vertex of the cells being flipped
/// @return A flipped triangulation if successful
[[nodiscard]] inline auto bistellar_flip(Delaunay&            triangulation,
                                         Edge_handle const&   edge,
                                         Vertex_handle const& top,
                                         Vertex_handle const& bottom)
    -> std::optional<Delaunay>
{
  // Get the cells incident to the edge
  auto incident_cells = get_incident_cells(triangulation, edge);

  // Check that there are exactly 4 incident cells
  if (!incident_cells || incident_cells->size() != 4) { return std::nullopt; }

  // Check incident cells are valid
  for (auto const& cell : incident_cells.value())
  {
    if (!cell->is_valid()) { return std::nullopt; }
  }

  // Get vertices from pivot edge
  auto const& pivot_from_1 = edge.first->vertex(edge.second);
  auto const& pivot_from_2 = edge.first->vertex(edge.third);

  // Get vertices from cells
  auto vertices            = get_vertices_from_cells(incident_cells.value());

  // Get vertices for new pivot edge
  Vertex_container new_pivot_vertices;
  std::copy_if(vertices.begin(), vertices.end(),
               std::back_inserter(new_pivot_vertices), [&](auto const& vertex) {
                 return vertex != pivot_from_1 && vertex != pivot_from_2 &&
                        vertex != top && vertex != bottom;
               });

  // Check that there are exactly 2 new pivot vertices
  if (new_pivot_vertices.size() != 2) { return std::nullopt; }

  // Label the vertices in the new pivot edge
  auto const& pivot_to_1 = new_pivot_vertices[0];
  auto const& pivot_to_2 = new_pivot_vertices[1];

  // Now we need to classify the cells by the vertices they contain
  Cell_handle before_1;  // top, pivot_from_1, pivot_from_2, pivot_to_1
  Cell_handle before_2;  // top, pivot_from_1, pivot_from_2, pivot_to_2
  Cell_handle before_3;  // bottom, pivot_from_1, pivot_from_2, pivot_to_1
  Cell_handle before_4;  // bottom, pivot_from_1, pivot_from_2, pivot_to_2
  for (auto const& cell : incident_cells.value())
  {
    if (cell->has_vertex(top))
    {
      if (cell->has_vertex(pivot_to_1)) { before_1 = cell; }
      else { before_2 = cell; }
    }
    else
    {
      if (cell->has_vertex(pivot_to_1)) { before_3 = cell; }
      else { before_4 = cell; }
    }
  }

  // Verify these cells are valid
  if (!before_1->is_valid() || !before_2->is_valid() || !before_3->is_valid() ||
      !before_4->is_valid())
  {
    return std::nullopt;
  }

  // Now find the exterior neighbors of the cells
  Cell_handle n_1 = before_1->neighbor(before_1->index(pivot_from_2));
  Cell_handle n_2 = before_1->neighbor(before_1->index(pivot_from_1));
  Cell_handle n_3 = before_2->neighbor(before_2->index(pivot_from_1));
  Cell_handle n_4 = before_2->neighbor(before_2->index(pivot_from_2));
  Cell_handle n_5 = before_3->neighbor(before_3->index(pivot_from_2));
  Cell_handle n_6 = before_3->neighbor(before_3->index(pivot_from_1));
  Cell_handle n_7 = before_4->neighbor(before_4->index(pivot_from_1));
  Cell_handle n_8 = before_4->neighbor(before_4->index(pivot_from_2));

  // Next, delete the old cells
  triangulation.tds().delete_cell(before_1);
  triangulation.tds().delete_cell(before_2);
  triangulation.tds().delete_cell(before_3);
  triangulation.tds().delete_cell(before_4);

  fmt::print("Cells in the triangulation after deleting old cells: {}\n",
             triangulation.number_of_cells());

  // Now create the new cells
  Cell_handle a_1 = triangulation.tds().create_cell(top, pivot_from_1,
                                                    pivot_to_1, pivot_to_2);
  Cell_handle a_2 = triangulation.tds().create_cell(top, pivot_from_2,
                                                    pivot_to_1, pivot_to_2);
  Cell_handle a_3 = triangulation.tds().create_cell(bottom, pivot_from_1,
                                                    pivot_to_1, pivot_to_2);
  Cell_handle a_4 = triangulation.tds().create_cell(bottom, pivot_from_2,
                                                    pivot_to_1, pivot_to_2);

  // Now set the neighbors of the new cells
  a_1->set_neighbors(n_1, n_4, a_2, a_3);
  a_2->set_neighbors(n_2, n_3, a_1, a_4);
  a_3->set_neighbors(n_5, n_8, a_4, a_1);
  a_4->set_neighbors(n_6, n_7, a_2, a_3);

  fmt::print("Cells in the triangulation after adding new cells: {}\n",
             triangulation.number_of_cells());

  // Fix any cell orientation issues
  if (!triangulation.is_valid()) { triangulation.tds().reorient(); }

  // Check validity of cells
  if (a_1->is_valid() && a_2->is_valid() && a_3->is_valid() && a_4->is_valid())
  {
    return std::make_optional(triangulation);
  }
  else { return std::nullopt; }
}  // bistellar_flip

#endif  // BISTELLAR_FLIP_BISTELLAR_FLIP_HPP
