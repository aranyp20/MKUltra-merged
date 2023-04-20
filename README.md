# MKUltra-merged


## Summary

- Outer shell generation with modified 'Marching cubes' algorithm, using interval arithmetic for optimization.

- Inner shell generation from an outer shell, detecting self-intersections. Settings: number of inner shells, distance between shells.

- Infill generation by rotating paralell lines around Z axis. Settings: number of rotations, distance between parallell lines

- Support structure generation with blending and other implicit operations...


- Implicit surface fitting on objects defined in .obj files. (need limited rho -> need another eigen solve method, and currently cant print out surfaces where positive values can be inside an object.)

- Currently using built-in surfaces

- Colorizes outer shell with dot(normal,vec3(0,0,-1)) value ([0,1]). This value shows in which areas support is mostly needed.

- GUI: 2D layer view (ScrollBar select), 3D view. Shows support (checkbox).

## Dependencies:

- Eigen: https://eigen.tuxfamily.org/index.php?title=Main_Page
- Qt: https://www.qt.io/
