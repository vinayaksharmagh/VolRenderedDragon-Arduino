#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>
#include"error.h"
#include"context.h"
#include"app.h"




int main()
try
{
	N_app::app obj_gl;
	obj_gl.run();

}
catch (std::exception &e)
{
	std::cerr << "ERROR:" << e.what() << std::endl;
	char end;
	std::cin >> end;
}

