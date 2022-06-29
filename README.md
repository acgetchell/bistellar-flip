# bistellar-flip

Bistellar flip proposal for CGAL.

## Description

A bistellar flip operates on 4 cells in a 3-dimensional triangulation.

The *pivot edge* is the edge that is common to all 4 cells. It's endpoints are
*Pivot_from_1* and *Pivot_from_2*.

The *new pivot edge* is the edge that will be common to the 4 new cells.
It's endpoints are *Pivot_to_1* and *Pivot_to_2*, obtained by selecting the 2 vertices
of the 6 comprising the 4-cell complex that are not the *Top*, *Bottom*, *Pivot_from_1*,
or *Pivot_from_2* vertices.

![bistellar flip](docs/bistellar-flip.png "bistellar flip")

Here are the 4 old cells defined by the vertices labelled in the figure:

| Cell         | Vertices                                               |
|--------------|--------------------------------------------------------|
| **before_1** | *Top*, *Pivot_from_1*, *Pivot_from_2*, *Pivot_to_1*    |
| **before_2** | *Top*, *Pivot_from_1*, *Pivot_from_2*, *Pivot_to_2*    |
| **before_3** | *Bottom*, *Pivot_from_1*, *Pivot_from_2*, *Pivot_to_1* |
| **before_4** | *Bottom*, *Pivot_from_1*, *Pivot_from_2*, *Pivot_to_2* |

And here are the 4 new cells:

| Cell         | Vertices                                             |
|--------------|------------------------------------------------------|
| **after_1**  | *Top*, *Pivot_from_1*, *Pivot_to_1*, *Pivot_to_2*    |
| **after_2**  | *Top*, *Pivot_from_2*, *Pivot_to_1*, *Pivot_to_2*    |
| **after_3**  | *Bottom*, *Pivot_from_1*, *Pivot_to_1*, *Pivot_to_2* |
| **after_4**  | *Bottom*, *Pivot_from_2*, *Pivot_to_1*, *Pivot_to_2* |

We also must obtain the neighbors of the 4 old cells, and assign them appropriately to the 4 new cells.
CGAL indicates neighbors by a cell and the index of the vertex opposite the neighboring cell.
The four cells form an octohedron, so there are 8 neighbors, as follows:

| Neighboring cell | Cell         | Vertex opposite |
|------------------|--------------|-----------------|
| **n_1**          | **before_1** | *Pivot_from_2*  |
| **n_2**          | **before_1** | *Pivot_from_1*  |
| **n_3**          | **before_2** | *Pivot_from_1*  |
| **n_4**          | **before_2** | *Pivot_from_2*  |
| **n_5**          | **before_3** | *Pivot_from_2*  |
| **n_6**          | **before_3** | *Pivot_from_1*  |
| **n_7**          | **before_4** | *Pivot_from_1*  |
| **n_8**          | **before_4** | *Pivot_from_2*  |

We then assign neighbors to the 4 new cells:

| New cell    | Neighboring cells                          |
|-------------|--------------------------------------------|
| **after_1** | **n_1**, **n_4**, **after_2**, **after_3** |
| **after_2** | **n_2**, **n_3**, **after_1**, **after_4** |
| **after_3** | **n_5**, **n_8**, **after_1**, **after_4** |
| **after_4** | **n_6**, **n_7**, **after_2**, **after_3** |