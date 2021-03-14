# Computer Graphics Assignment
### The group members are:
Pranav Balaji - 2019A7PS0040H  
Subienay Ganesh - 2019A7PS0096H  
Kaustubh Bhanj - 2019A7PS0009H  
<br>
**Please download and extract the release "Source/ComputerGraphicsAssignment1.rar" in order to view the html documentation**
Otherwise, <br><br/>

For the source code, go to [OpenGL/src](https://github.com/ComputerGraphics-Group/OpenGL/blob/master/OpenGL/src/Application.cpp)

The shader file is present in [OpenGL/res/shaders](https://github.com/ComputerGraphics-Group/OpenGL/blob/master/OpenGL/res/shaders/basic.shader)

Code documentation is present in [Documentation/Doxygen/html](https://github.com/ComputerGraphics-Group/OpenGL/tree/master/Documentation/Doxygen/html)

Documentation of our journey through the assignment is present in [Documentation/Our Journey](https://github.com/ComputerGraphics-Group/OpenGL/tree/master/Documentation/Our%20Journey)

A ready-made executable is present as a release called "dipole" which simulates the vector field of the latest electric dipole mentioned in our journey.
You can zoom using the mouse wheel and change sparsity using the mouse buttons.

You can modify the `vectorField(int x, int y)` function to any vector field, but please remember to change the scale accordingly.
For an order 1 field like xi+yj, use a `scale` of 0.1 to 0.01. For an order 2 field like x²i+y²j, use a `scale` of 0.0001 to 0.00001 and so on. Similarly for fields like sinxi+cosyj, use a `scale` of 10 or 100.

The coloring scheme that is active currently is dx and dy based. Please refer to our journey for more details.
