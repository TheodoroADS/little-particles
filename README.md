# Simple particle simulation for fun

[![](https://raw.githubusercontent.com/theodoroADS/little-particles/main/media/demo_thumb.jpg)](https://raw.githubusercontent.com/theodoroADS/little-particles/main/media/little_demo.mp4)

I just wanted to have some fun learning raylib and trying to the a simple planet simulation.
This could be a nice project example for people trying to learn raylib like myself :).

The particles in my "simulation" are attracted to the mouse and by each other, and I apply a spring-like force for collisions.

## Dependencies 

If by some chance anyone wants to build this project, you would need [Raylib](https://www.raylib.com/).

I have compiled my project with raylib for windows (I have created local include/ and lib/ directories with the raylib header and DLL), but feel free do change my makefile to use the library in your system (you just have to compile main.c and then link with raylib and its dependencies).

