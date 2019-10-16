# Platformer project

An educational project which goal is to learn C++ game development including rendering, physics, UI, tools creation as well as environment setup and multiplatform support.

The first version is meant to be a technical training in C++ and OpenGL, so it is lacking visual fidelity. The next iteration, however, is all about bringing juice to this small game. I plan to add support for particle systems, sprite animations, post-processing, screen shake and nice UI transitions. There will also be some technical redesign with the introduction of Entity Component System and Cmake as a build tool. 

## Current featureset
- 2D sprites. Support textures with alpha, sorting, scaling and rotating.
- AABBs and raycasts.
- AI navigation across platforms
- Shooting enemies
- Game loop with restart on player death

## In the next version:
- Entity Component System!
- Improvements to the feel of the game: animations, post-processing, sounds
- CMake support

## Known issues
- Sorting order or physical layer of a sprite can be changed only on initialization.
- Bounding box is axis aligned (i.e. doesn't correctly support rotations).