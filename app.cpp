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
#include<sstream>
#include<cctype>

#define GLM_FORCE_RADIANS

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtc/quaternion.hpp>


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


namespace N_app
{


	const float app::PI = 22.0f / 7;


	void app::run()//it will act as main for opengl application 
	{


		vp_w = (con_w = 1280);
		vp_h = (con_h = 800);

		context obj(con_w, con_h);
		glfwMakeContextCurrent(obj.get_win());

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			error("can't get proc address");

		}

		glViewport(0, 0, vp_w, vp_h);

		chk();
		init();
		startup();
		render();
	}


	void app::init()
	{

		n_plane = 0.1;
		f_plane = 1000;
		fov = glm::radians(45.0f);
		view_depth = 1.5;
		M_composite = glm::mat4(1.0);
		
		val = 0;
		x_ax = y_ax = z_ax = 0;
		abst *ptr;

		ptr = new c_prog_m;
		ptr->pure(this);


	}


	void app::startup()
	{
		abst *ptr;
		ptr = new c_count;
		ptr->pure(this);

		ptr = new c_storage_block;
		ptr->pure(this);

		ptr = new c_image;
		ptr->pure(this);


		glGenVertexArrays(1, &app_vao);
		glBindVertexArray(app_vao);
	}

	
	void app::render()
	{

		N_model::model m("model\\dragon\\dragon1.obj");

		abst* ptr;
		while (!glfwWindowShouldClose(glfwGetCurrentContext()))
		{

			ptr = new c_serial_in;
			ptr->pure(this);

			float one = 1.0f;

			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

			glUseProgram(prog_m[0]);//this program corrosponds to a pipeline which clears linked list obtained from prev loop (by filling all texel coordinates with 0xFFFFFFFF(max unsigned int) )
									
			glBindVertexArray(app_vao);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			 
			glUseProgram(prog_m[1]);//this program corrosponds to a pipeline which fills linked list as per new orientation of model. 


			static unsigned int zero = 0;
			glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
			glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, counter_buff);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, storage_blk_buff);
			glBindImageTexture(0, img_tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);

			ptr = new c_matrix;
			ptr->pure(this);

			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
			m.draw(prog_m[1]);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

			glUseProgram(prog_m[2]);//This program corosponds to piplne which traverses linked list for calculating accomodated depth which is then used as intensity of color with which model is being drawn 
			glBindVertexArray(app_vao);


			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
			
			glUseProgram(prog_m[3]);//this program corrosponds to pipeline responsible for drawing axis lines over volume rendered model

			glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(M_composite));
			chk();

			glDrawArrays(GL_LINES, 0, 6);
			glLineWidth(5);
			glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);
			
			glfwSwapBuffers(glfwGetCurrentContext());
			glfwPollEvents();
			
			ptr = new c_serial_out;
			ptr->pure(this);


		}


	}

	

}


