# VolRenderedDragon-Arduino
This project was created using modern OpenGL and Arduino. It performs Volume rendering on given model (using OIT per-pixel linked list method). At run-time, orientation of volume rendered model can be controlled via Arduino. Quaternions have been used to prevent Gimbal Lock.

(runtime video :https://www.youtube.com/watch?v=erNQNznFXqI&feature=youtu.be )


Language Used: C++
 

Board used

- Arduino Leonardo



Components used

-3 x 220 Ohm resistors (to act as connections of fixed resistance .Jumper wires could also have been used but their use cause a lot of flutuation in potentiometer readings due to changes in their resistance from twists and tangles )

-3 x 1000 Ohm resistors (as pull down resistors )

-3 x Buttons (to select axis of rotation)

-1 x Potentiometer(or Trimpot) (to rotate model about selected axis)

-Breadboard

-Jumper Wires




External Libruaries used

-GLM

-GLFW

-GLAD

-ASSIMP

-SerialPort


Model Used

-Stanford dragon



file info

-main.cpp : simply contains entry point and call to Opengl applcation part

-app.cpp : contains all the definitions of methods needed for Opengl application part

-context.cpp: deals with creation of window using glfw methods

-abst.h : declares an abstract class( named "abst") with a pure virtual function( named "pure" )

-conc.cpp : defines all the concrete classes (inherited from abstract class "abst" ) .This file is critical as it contains various overridden definitions of function "pure" which is dynamically binded from different parts of code in application (This has been done to partition code logically without having to create many different functions) {Yes, I know that dynamic binding of function is slower than static one but I have done such partition for ease of coding and debugging (just like error handling is done despite its known effects on performance of execution). In the final release code (for building exe), dynamic binding calls can be replaced by corrosponding definitions in conc.cpp}

-dragon_control.ino : Arduino file, it contains code for analysing readings from potentiometer and buttons and transferring them to  Serial Port.

-resfun.cpp : contains definition of various funtions other than member functions of class app. {only a few of these functions have been used in this project (these functions are infact custom abstractions used by coder of this project for his other opengl projects)

-model.cpp : used to extract "Assests" (like vertex coordinates,normals etc) from Assimp data structure




-glad.c : required by GLAD libruary


-SerialPort.cpp : required by SerialPort libruary


-Vertex_m0.txt and Frag_m0.txt : shaders responsible for clearing the linked list


-Vertex_m1.txt and Frag_m1.txt : shaders responsible for filling the linked list 


-Vertex_m2.txt and Frag_m2.txt : shaders responsible for traversing linked list and calculating accomodated depth.


-Vertex_m3.txt and Frag_m3.txt : shaders responsible for drawing red (X) ,green (Y) , blue (Z) axis lines over model


-dragon1.obj : model file


-assimp-vc140-mt.dll :put in project folder


-glew32.dll :put in project folder




