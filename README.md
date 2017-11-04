# MissileShooter

My first completed major hobby project as a university sophomore. Main description in missile_shooter\main.cpp. Here's a copy of it from (11/4/17)

-----

Other specific notes I'd like to put here:

must have VS 2015 installed to run debug builds for this program directly

-----

/*
OpenGL Missile Shooter

by Nicolas Obias, completed 10/27/17

(Physical requirements spec. in own possession)

This project aims on having the user roam around a scene containing an orange plane
and a white light cube (symbolizing the light for lighting calculations) in a free moving/rotating
camera, with the ability to left click to make a missile appear on one side of the plane and disappear on the other.

The program loads vertex model data from media\model_data.txt. The .txt file consists of triangle data
listed as 3 lines of vertices, where each line is a single vertex. The format of a single vertex line is:

{x} {y} {z} {normal x} {normal y} {normal z}

All 6 of these inputs for a single vertex should be a floating point number with 3 decimal digits.
Although I specified in my own requirements spec. a specific model shape, this model data setup is flexible to
load any model, as long as it's valid data.


WASD to move

Move mouse to rotate camera

Scroll up/down to zoom in/out on scene.

Left click down to make a missile appear.

*/
