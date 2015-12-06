## Play the Game!
To try out the game, simply download the "Release" folder and open Asteroids.exe.  (The executable requires freeglut.dll to be in the same directory.)

## Made With
- Language used: C++
- Software used: Visual Studio
- Libraries used: OpenGL/FreeGLUT

## Project Notes
This game is the finished result of a semester-long project that part of a computer graphics class that I took in Fall 2015.  We were given certain gameplay requirements to meet but complete freedom in how to design it.  I implemented a game object model that allowed for easy extensibility in creating new game objects and behaviors.  While I was quite pleased with my overall design, if I were to revisit it I would have shifted most of the actual gameplay logic out of Main.cpp and into its own class.

- All of the asteroids are randomly created at runtime (random shapes, colors, velocity, rotation, etc.)
- An extra life is granted for every 10,000 points earned
- Every new level starts with an extra large asteroid (up to a maximum of 20)
- After the player earns 5,000 points, every alien spawn after the first one on a new level will be a special small alien: faster, more accurate, and worth more points
- The player becomes immune for 2 seconds after dying

## Reuse Permissions
This project may be freely used and redistributed.  This project should not be used

## TODO List



# Asteroids
Play the game by downloading the Release folder and running Asteroids.exe! (Requires freeglut.dll to be in the same directory.)

This game was the result of a semester-long project in a computer graphics class I took in Fall 2015.  It was written in C++ using OpenGL with the aid of FreeGLUT.  All code is my own except where otherwise cited.  This project may be used freely except by students enrolled in a class tasked with the same project.
