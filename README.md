# Platformer project

An educational project which goal is to learn C++ game development including rendering, physics, UI, tools creation as well as environment setup and multiplatform support.

## Current featureset
- 2D sprites. Support textures with alpha, sorting, scaling and rotating.
- AABBs and raycasts.

## Known issues
- Sorting order or physical layer of a sprite can be changed only on initialization.
- Bounding box is axis aligned (i.e. doesn't correctly support rotations) .