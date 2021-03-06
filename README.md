## Video Demo
[![http://www.youtube.com/watch?v=CO72NSnxGpc](http://img.youtube.com/vi/CO72NSnxGpc/0.jpg)](http://www.youtube.com/watch?v=CO72NSnxGpc "http://www.youtube.com/watch?v=CO72NSnxGpc")

## Play the Game!
To try out the game, simply download the "Release" folder and open Asteroids.exe (this executable requires freeglut.dll to be in the same directory).

## Made With
- Language used: C++
- Software used: Visual Studio
- Libraries used: OpenGL/FreeGLUT

## Project Notes
This game is the finished result of a semester-long project that was part of a computer graphics class that I took in Fall 2015. We were given certain gameplay requirements to meet but complete freedom in how to approach our design.  I implemented a component-based game object model that made extending or adding new behaviors an incredibly easy task.  I personally designed all art, including the asteroids which all have unique shapes due to their vertices being randomly created at runtime.

Interesting notes:
- All of the asteroids are randomly created at runtime (random shapes, colors, velocity, rotation, etc.)
- An extra life is granted for every 10,000 points earned
- Every new level starts with an extra large asteroid (up to a maximum of 20)
- After the player earns 5,000 points, every alien spawn (after the first one per level) will be a special small alien: faster, more accurate, and worth more points
- The player becomes immune for 2 seconds after dying

## Reuse Permissions
This project should not be used as a resource by anyone tasked with a similar academic project.  Otherwise, this project may be freely used and redistributed.

## To Do List
- Refactor gameplay logic out of main class
- Improve lighting effects for explosions
