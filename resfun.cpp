#include<iostream>
#include<fstream>
#include<string>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"app.h"
#include"error.h"

//D) here we have removed fill_prog function and shifted it to class c_prog's overloaded pure() (See conc.cpp //A)


namespace N_app
{
	void load_shad(std::string name, GLuint & sh)//load_shad reads corrospinding file and gets its string(See //1), points to that string via array of pointer(See //2) and using this array of pointer loads source code in shader object(See //3)
	{
		if (name!= "NULL")//put "NULL" as name if a particular shader is absent in a particular prog object 
		{
			std::ifstream ifs(name.c_str());

			std::string s, s_;


			while (getline(ifs, s_))//1)
			{
				s = s + s_ + "\n";

			}

			const GLchar *str[] = { s.c_str() };//2

			glShaderSource(sh, 1, str, NULL);//3
		}
		
		else
		{
			const GLchar *str[] = { "\0" };
			glShaderSource(sh, 1,str , NULL);

		}
	}


}


	glm::vec4 quad_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, float k)
	{
		glm::vec4 D = glm::mix(A, B, k);
		glm::vec4 E = glm::mix(B, C, k);

		glm::vec4 P = glm::mix(D, E, k);

		return P;
	}

	glm::vec4 cubic_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, glm::vec4 D, float k)
	{
		glm::vec4 E = glm::mix(A, B, k);
		glm::vec4 G = glm::mix(B, C, k);
		glm::vec4 H = glm::mix(C, D, k);

		return quad_bezier(E, G, H, k);

	}

	glm::vec4 quantic_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, glm::vec4 D, glm::vec4 E, float k)
	{
		glm::vec4 G = glm::mix(A, B, k);
		glm::vec4 H = glm::mix(B, C, k);
		glm::vec4 I = glm::mix(C, D, k);
		glm::vec4 J = glm::mix(D, E, k);

		return cubic_bezier(G, H, I, J, k);
	}


	////////////////////////////////////////////////////////////////C////////////////////////////////////////////////////////////////////



	glm::mat3 t_trans(float x, float y)
	{
		return glm::mat3(
			glm::vec3(1, 0, 0),
			glm::vec3(0, 1, 0),
			glm::vec3(x, y, 1)
		);
	}

	glm::mat3 t_scale(float s_x, float s_y)
	{
		return glm::mat3(
			glm::vec3(s_x, 0, 0),
			glm::vec3(0, s_y, 0),
			glm::vec3(0, 0, 1)
		);
	}

	glm::mat3 t_rot(float ang)
	{
		return glm::mat3(
			glm::vec3(cos(ang), -sin(ang), 0),
			glm::vec3(sin(ang), cos(ang), 0),
			glm::vec3(0, 0, 1)
		);
	}

	
	/////////////////////////////////////////////////////////////C/////////////////////////////////////////////////////////////////////

	/*
	void gen_tex(float* data, int w, int h)
	{
		int x, y;//they corrospoind to x,y coord of fragments

		for (y = 0; y < h; ++y)
		{
			for (x = 0; x < w; ++x)
			{

				data[(y*w + x) * 4 + 0] = float((x&y) & 0xff) / 255.0;
				data[(y*w + x) * 4 + 1] = float((x | y) & 0xFF) / 255.0;

				data[(y*w + x) * 4 + 2] = float((x^y) & 0xFF) / 255.0;
				data[(y*w + x) * 4 + 3] = 1.0;


			}


		}
	}
	*/


	void gen_tex(float* data, int w, int h)//this gen_tex creates a checkbox pattern, by altering the color between 0 and 1
	//our aim is to have black and white boxes alternately among adjacent fragments , this is acheived as exp below

	{
		int x, y;//they corrospoind to x,y coord of fragments
		int count;

		for (y = 0; y < h; ++y)
		{

			if (y % 2 == 0)
			{
				count = 0;
			}
			else
			{
				count = 1;
			}
			for (x = 0; x < w; ++x)
			{
				if (count % 2 == 0)
				{
					data[(y*w + x) * 4 + 0] = 0;//black color 
					data[(y*w + x) * 4 + 1] = 0;
					data[(y*w + x) * 4 + 2] = 0;
					data[(y*w + x) * 4 + 3] = 1.0;
				}
				else
				{
					data[(y*w + x) * 4 + 0] = 1;//white color
					data[(y*w + x) * 4 + 1] = 1;
					data[(y*w + x) * 4 + 2] = 1;
					data[(y*w + x) * 4 + 3] = 1.0;

				}

				++count;//count is incrementd as x (for each y) increases, count increase either from 0 or from 1

			}


		}

	}



	void gen_shape(glm::vec4 *v_pos, glm::mat4 M_gen)
	{
		float a = 4;//2*a is side length of one face

		glm::vec4 v[] = {//combination of triangles to form 2d shape
			glm::vec4(a,a,0,1),
			glm::vec4(-a,-a,0,1),
			glm::vec4(a,-a,0,1),

			glm::vec4(a,a,0,1),
			glm::vec4(-a,-a,0,1),
			glm::vec4(-a,a,0,1),
			
		};


		int count = 1;
		float x = 0, y = 0, z = 0, x_ax = 0, y_ax = 0, z_ax = 0;
		float ang = 0.0f;

		for (int i = 1; i <= 36; ++i)//this loop helps in creating a 3d shape by using and arranging/orienting 2d shape defined in v[] above
		//eg here cube is created using 2d square defined in v[] above.
		//this square is trasformed using suitable variation of M_gen matrix .
		//using M_gen square is first pulled back 'a' units along +ve z axis to form front face of cube
		//then it is pushed forward 'a' units in -ve z axis to form back face of cube
		//(we are traversing by a units because side length of square is 2*a so side length of cube should also be 2*a , thus parallel opposite faces of cube should be
		//2*a units apart)
		//then it is rotated by 90 deg about x axis and then moved in +ve y axis by 'a' units (to form up face)
		//similarly, it is rotated and traversed to form remaining faces as descb below
		{

			x = 0, y = 0, z = 0, x_ax = 0, y_ax = 0, z_ax = 0;
			float ang = 0.0f;


			switch (count)
			{

			case 1://front face
				x_ax = y_ax = z_ax = 1;//this is done to give some axis of rotation to glm::rotate (we can't keep all 3 axis 0(it won't give any error but won't give desired results)
				z = 1;//push face back in z axis (translate to +ve z axis)
				break;

			case 2://back face
				x_ax = y_ax = z_ax = 1;
				z = -1;//push face forward in z axis (translate to -ve z axis)
				break;

			case 3://up face
				ang = 90.0f;
				x_ax = 1;
				y = 1;//rotate face about x axis and push it up in y axis
				break;
			case 4://down face
				ang = 90.0f;
				x_ax = 1;
				y = -1;
				break;

			case 5://right face
				ang = 90.0f;
				y_ax = 1;
				x = 1;
				break;

			case 6://left face
				ang = 90.0f;
				y_ax = 1;
				x = -1;
				break;
			}

			M_gen = glm::translate(glm::mat4(1.0f), glm::vec3(x*(a), y*(a ), z*(a)))
				*glm::rotate(glm::mat4(1.0f), glm::radians(ang), glm::vec3(x_ax, y_ax, z_ax));

			v_pos[i - 1] = M_gen * v[(i-1) % 6];//all the transformed verticies are saved in v_pos

			if (i % 6 == 0)//each face will be created by 2 triangles(or 6 verticies), so each leap of 6 is counted as one face
			{
				++count;

			}

		}
	}
	/////////////////////////////////////////////////////////////A/////////////////////////////////////////////////////////////////////


