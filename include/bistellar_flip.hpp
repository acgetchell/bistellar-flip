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

#include <optional>

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
}  // get_finite_cells

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
}  // get_finite_edges

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
}  // find_pivot_edge

#endif  // BISTELLAR_FLIP_BISTELLAR_FLIP_HPP
