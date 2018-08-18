#include<iostream>
#include<string>
#include<stdexcept>
#include<cmath>
#include<fstream>
#include<vector>
#include<cstring>
#include<cstddef>
#include<algorithm>
#include<cstdlib>


#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

//#define STATIC_GLEW

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>

#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"SerialPort.h"


#include"error.h"
#include"context.h"
#include"resfun.h"
#include"app.h"//declaration of app class
#include"abst.h"//declaration of abstract class
#include"conc.h"//delaration of all concrete classes
#include"model.h"


#include<stb_image.h> 



context::context(): win(NULL) {};

context::~context()
{
	glfwTerminate();
}

context::context(int w, int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	win = glfwCreateWindow(w, h, "wind", NULL, NULL);

	if (win == NULL)
	{
		error("window can't be created");
	}

}

