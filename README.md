# Rectangle World

Welcome to the **Rectangle World** project. This program allows you to create, move, and visualize rectangles on a 2D plane, simulating an environment with gravity rules and checking the instability of the rectangles.

## Features

- **Rectangle Creation**: Creates rectangles at specific coordinates on the plane.
- **Rectangle Movement**: Moves rectangles to the right or left.
- **Gravity**: Rectangles fall due to gravity until they find a support surface.
- **Instability Check**: Calculates the instability of the rectangle system.
- **Visualization**: Displays the rectangles on a 2D plane.

## How to Use

### Compilation

To compile the program, use a C compiler such as `gcc`:

```sh
gcc -o rectangles rectangles.c

## Execution

To run the program, pass the desired commands as command line arguments. For example:

./rectangles create 10,5+3,2 moveright 10,5+4 moveleft 14,5+2

Available Commands
create x,y+l,h: Creates a rectangle where (x,y) are the coordinates of the bottom-left corner and (l,h) are the width and height of the rectangle.
moveright x,y+p: Moves the rectangle at coordinates (x,y) to the right by p positions.
moveleft x,y+p: Moves the rectangle at coordinates (x,y) to the left by p positions.
Example Usage
Here is an example of how to use the program:

sh
Copiar código
./rectangles create 10,5+3,2 create 5,10+4,3 moveright 10,5+2 moveleft 14,5+1
This command does the following:

Creates a rectangle with the bottom-left corner at coordinates (10, 5) and dimensions 3x2.
Creates a rectangle with the bottom-left corner at coordinates (5, 10) and dimensions 4x3.
Moves the rectangle at (10, 5) two positions to the right.
Moves the rectangle at (14, 5) one position to the left.
Code Structure
The code is structured as follows:

Definitions and Structures: Definition of constants and the Rectangle structure. 

Main Functions:

options(): Displays the available command options.
collide(): Checks if two rectangles collide.
apply_gravity(): Adjusts the position of the rectangles due to gravity.
calculate_instability(): Calculates the instability of the rectangle system.
create_rectangle(): Creates a new rectangle.
move_right(): Moves a rectangle to the right.
move_left(): Moves a rectangle to the left.
visualize(): Displays the plane with the rectangles.
Technical Details
Gravity: The apply_gravity() function moves the rectangles down until they find support.
Instability: The calculate_instability() function calculates the proportion of unsupported base of the rectangles and issues an alert if instability is high.
Collisions: The collide() function checks if two rectangles overlap.
Contributions
Contributions are welcome! Feel free to open issues and pull requests for improvements and fixes.


