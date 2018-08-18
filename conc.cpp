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

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<ktx.h>
#include"SerialPort.h"


#include"error.h"
#include"context.h"
#include"resfun.h"
#include"app.h"
#include"abst.h"
#include"conc.h"

#include<stb_image.h> 

//This is Source file for definition of members of concrete classes

namespace N_app
{

	void c_prog_m::pure(app* th)
	{

		std::string vert_s, frag_s;
		
		std::string v_name[] = { "Vertex_m0.txt","Vertex_m1.txt","Vertex_m2.txt","Vertex_m3.txt" };
		std::string c_name[] = { "NULL" , "NULL", "NULL","NULL" };
		std::string e_name[] = { "NULL" ,"NULL", "NULL","NULL" };
		std::string g_name[] = { "NULL" ,"NULL","NULL","NULL" };
		std::string f_name[] = { "Frag_m0.txt", "Frag_m1.txt" , "Frag_m2.txt","Frag_m3.txt" };
		std::string p_name[] = { "prog_m0", "prog_m1" , "prog_m2","prog_m3" };
		

		for (int i = 0; i < sizeof(th->prog_m) / sizeof(GLuint); ++i)
		{

			vert_shad = glCreateShader(GL_VERTEX_SHADER);
			cntrl_shad = glCreateShader(GL_TESS_CONTROL_SHADER);
			eval_shad = glCreateShader(GL_TESS_EVALUATION_SHADER);
			geom_shad = glCreateShader(GL_GEOMETRY_SHADER);
			frag_shad = glCreateShader(GL_FRAGMENT_SHADER);
			chk();
			
			th->prog_m[i] = glCreateProgram();
			
			load_shad(v_name[i].c_str(), vert_shad);
			load_shad(c_name[i].c_str(), cntrl_shad);
			load_shad(e_name[i].c_str(), eval_shad);
			load_shad(g_name[i].c_str(), geom_shad);
			load_shad(f_name[i].c_str(), frag_shad);


			glCompileShader(vert_shad);
			check_compile(vert_shad, v_name[i].c_str());

			glCompileShader(cntrl_shad);
			check_compile(cntrl_shad, c_name[i].c_str());

			glCompileShader(eval_shad);
			check_compile(eval_shad, e_name[i].c_str());

			glCompileShader(geom_shad);
			check_compile(geom_shad, g_name[i].c_str());

			glCompileShader(frag_shad);
			check_compile(frag_shad, f_name[i].c_str());
			chk();
			
			if (v_name[i] != "NULL")//don't attach a shader which isn't present in pipeline (although vertex shader should always be present in pipeline and if its not then corrosponding prog should also be "NULL"
				glAttachShader(th->prog_m[i], vert_shad);
			
			if(c_name[i] != "NULL")
				glAttachShader(th->prog_m[i], cntrl_shad);
			
			if (e_name[i] != "NULL")
				glAttachShader(th->prog_m[i], eval_shad);
			
			if(g_name[i] != "NULL")
				glAttachShader(th->prog_m[i], geom_shad);
			
			if(f_name[i] != "NULL")
				glAttachShader(th->prog_m[i], frag_shad);
			chk();

			glDeleteShader(vert_shad);
			glDeleteShader(cntrl_shad);
			glDeleteShader(eval_shad);
			glDeleteShader(geom_shad);
			glDeleteShader(frag_shad);


			chk();
			if (p_name[i] != "NULL")//link program only when a program is present at given index
			{
				glLinkProgram(th->prog_m[i]);
				check_link(th->prog_m[i], p_name[i]);

			}
			chk();
		}

	}

	void c_count::pure(app* th)
	{
		glGenBuffers(1, &th->counter_buff);
		glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, th->counter_buff);
		glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), NULL, GL_DYNAMIC_COPY);//size is of GLuint bytes because in shader we are using atomic_uint type (unsigned int)
		int zero = 0;
		glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);
		glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, th->counter_buff);
		
	}



	void c_storage_block::pure(app* th)
	{
		glGenBuffers(1, &th->storage_blk_buff);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, th->storage_blk_buff);
		glBufferData(GL_SHADER_STORAGE_BUFFER, th->con_w * th->con_h * sizeof(node), NULL, GL_DYNAMIC_COPY); 
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, th->storage_blk_buff);
	}
	

	void c_image::pure(app*th)
	{
		glGenTextures(1, &th->img_tex);
		glBindTexture(GL_TEXTURE_2D, th->img_tex);
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, th->con_w, th->con_h);
		glBindImageTexture(0, th->img_tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_R32UI);
	}

	

	glm::quat c_matrix::q= glm::angleAxis(glm::radians(0.0f), glm::vec3(1, 0, 0))
		*glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 1, 0))
		*glm::angleAxis(glm::radians(0.0f), glm::vec3(0, 0, 1))
		;


	float c_matrix::ang_x = 0, c_matrix::ang_y = 0, c_matrix::ang_z = 0;
	float c_matrix::pang = 0;
	
	void c_matrix::pure(app*th)
	{
		float k = (th->val - 17) / float((1005 - 17) + 1);//with practical setup of arduino(as exp in dragon_control) at time of testing it was observed that max analog value given by Arduino
													  //is 1003 and min value is 19 but to be safe we are using 17 as min and 1005 as max


		if (th->x_ax == 1)
		{
			ang_x = glm::mix(0, 360, k);

			q = glm::cross(q, glm::angleAxis(glm::radians(ang_x - pang), glm::vec3(1, 0, 0)));
			pang = ang_x;

		}

		if (th->y_ax == 1)
		{
			ang_y = glm::mix(0, 360, k);


			q = glm::cross(q, glm::angleAxis(glm::radians(ang_y - pang), glm::vec3(0, 1, 0)));
			pang = ang_y;
		}

		if (th->z_ax == 1)
		{
			ang_z = glm::mix(0, 360, k);


			q = glm::cross(q, glm::angleAxis(glm::radians(ang_z - pang), glm::vec3(0, 0, 1)));

			pang = ang_z;
		}

		glm::mat4 M_rot = glm::mat4(1);
		M_rot = glm::mat4_cast(q);

		glm::mat4 M_model = glm::scale(glm::mat4(1.0f), glm::vec3(5, 5, 10))
			*M_rot
			;

		static glm::mat4 M_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -15));
		static glm::mat4 M_proj = glm::perspective(glm::radians(45.0f), (float)th->con_w / (float)th->con_h, th->n_plane, th->f_plane);
		th->M_composite = M_proj * M_view*M_model;

		glUniformMatrix4fv(3, 1, GL_FALSE, glm::value_ptr(th->M_composite));
		chk();



	}


	int c_serial_in::pval = 0;
	const char* c_serial_in::port_n = "\\\\.\\COM5";

	char c_serial_in::data[MAX_DATA_LENGTH];


	void c_serial_in::pure(app*th)
	{
		SerialPort obj(port_n);
		if (!obj.isConnected())
		{
			std::cerr << "Arduino not connected for writing to app\n";

		}
		else
		{
			int r = obj.readSerialPort(data, MAX_DATA_LENGTH);

			//printf("%s\n", data);
			th->val = 0;
			for (int i = 0; i<6 && data[i] != 'a'; ++i)//in recieved data, information part can be from 1 to 5 characters (of which first few will be digit and after that an alphabet x,y,z eg 236x or 500z 43x or 8y so to identify at end of info part terminating char 'a' is send from Arduino's program)
			{
				if (isdigit(data[i]))
				{
					th->val = (th->val * 10) + (data[i] - '0');
					//printf("%d ", val);
				}
				else
				{
					if (i == 0)//if non digit is present at first char then it means that info part doesn't have digits which isn't possible so it indicates an error (maybe in synchonization) 
							   //so in this case use prev successful info part
					{
						th->val = pval;
					}
					else//if at least 1 digit is present in info part then occurance of a non digit and non 'a' char means its one of x,y,z char
					{

						switch (data[i])
						{
						case 'x':
							th->x_ax = 1;
							th->y_ax = 0;
							th->z_ax = 0;
							break;

						case 'y':
							th->x_ax = 0;
							th->y_ax = 1;
							th->z_ax = 0;
							break;

						case 'z':
							th->x_ax = 0;
							th->y_ax = 0;
							th->z_ax = 1;
							break;
						}

					}
				}
			}

			if (th->val == 0)
			{
				th->val = pval;
			}

			//printf("%d\n", th->val);
			pval = th->val;

		}



	}

	const char* c_serial_out::port_n = "\\\\.\\COM5";
	char c_serial_out::data[MAX_DATA_LENGTH];
	char c_serial_out::signal_out;

	void c_serial_out::pure(app*th)
	{

		SerialPort obj(port_n);

		if (!obj.isConnected())
		{
			std::cerr << "Arduino not connected for reading from app \n";

		}
		else
		{
			if (!glfwWindowShouldClose(glfwGetCurrentContext()))
			{
				signal_out = 'y';
				obj.writeSerialPort(&signal_out, 1);

			}
			else
			{
				signal_out = 'n';
				obj.writeSerialPort(&signal_out, 1);

			}

		}


	}



};
