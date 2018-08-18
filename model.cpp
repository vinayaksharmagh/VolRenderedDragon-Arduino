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

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

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


namespace N_model
{
	GLuint model::prog;//declared here to avoid linker error


	void mesh::setup_mesh()
	{

		glGenVertexArrays(1, &model_vao);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		chk();
		glBindVertexArray(model_vao);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		chk();
		glBufferData(GL_ARRAY_BUFFER, v_.size() * sizeof(vertex), &v_[0], GL_STATIC_DRAW);
		chk();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_.size() * sizeof(unsigned int),&index_[0], GL_STATIC_DRAW); 
		chk();
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*) 0);
		
		chk();
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex,normal));
		
		chk();
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)offsetof(vertex, t_coord));
		

		glBindVertexArray(0);
		chk();
	}


	void mesh::draw( GLuint p )//drawing each mesh
	{
			unsigned int diffuseNr = 1;
			unsigned int specularNr = 1;
			unsigned int normalNr = 1;
			unsigned int heightNr = 1;
			for (unsigned int i = 0; i < t_.size(); ++i)//use unsigned int for loop var so that larges indicies can be used (in case of models indicies can be large)
			{
				glActiveTexture(GL_TEXTURE0 + i); 
					
				std::stringstream ss;
				std::string number;
				std::string name = t_[i].type;
				std::string name_;
				
				if (name == "texture_diffuse")
					ss << diffuseNr++; 
				else if (name == "texture_specular")
					ss << specularNr++; 
				else if (name == "texture_normal")
					ss << normalNr++;
				else if (name == "texture_height")
					ss << heightNr++;


				number = ss.str();

				name_ = "material." + name + number;
				glUniform1i(glGetUniformLocation(p, name_.c_str()), i);//gets location of a uniform(named as per a convention texture_type_number ) in given program

				chk();
				glBindTexture(GL_TEXTURE_2D, t_[i].ID);
			}
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(model_vao);
			glDrawElements(GL_TRIANGLES, index_.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
			chk();
		
	}


	void model::draw( GLuint p )//drawing each model (by drawing all mesh data stored in v_m vector )
	{
		set_prog(p);//it is used just to keep record of program being used currently 
		glUseProgram(p);

		for (unsigned int i = 0; i < v_m.size(); ++i)
		{
			v_m[i].draw(p);

		}
		chk();

	}

	

	void model::load_model(std::string path)
	{

		//std::cerr << "A" << std::endl;
		Assimp::Importer imp_obj;
		const aiScene * s = imp_obj.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!s || s->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !s->mRootNode)
		{
			std::string st1 = "ERROR: ASSIMP ";
			std::string st2 = imp_obj.GetErrorString();
			error(st1 + st2);
		}
		dir = path.substr(0, path.find_last_of('\\'));

		process_node(s->mRootNode, s);//this function call is put inside load_model because we want a single invocation(to load_model) to do all that's necessay to get model into code 
	}

	
	void model::process_node(aiNode *n, const aiScene *s)
		
	{
	
		for (unsigned int i = 0; i < n->mNumMeshes; ++i)
		{
			aiMesh * m = s->mMeshes[n->mMeshes[i]]; //obtaining all the mesh objects to which current node points via indicies in its mMeshes array
			
			v_m.push_back(process_mesh(m, s));//extracting data from obtained mesh object and ptting it in v_m vector for future drawing

		}


		for (unsigned int i = 0; i < n->mNumChildren; ++i)//identifying a child of node(via elements in its mChildren array) and then immediately processing that child and then identifying child of this child and immediately processing it (Depth First Search)

		{
			process_node(n->mChildren[i], s); 
		}
		
		

	}


	mesh model::process_mesh(aiMesh * m, const aiScene *s)
	{ 

		std::vector<vertex> v;
		std::vector<unsigned int> index;
		std::vector<texture> t;

		for (unsigned int i = 0; i < m->mNumVertices; ++i)//in this function we simply extract data about verticies, texture coord, normals,materials etc and use it to create an object of class mesh(declared in model.h) which
			//is pushed in v_m vector for future use (drawing)
		{
			vertex temp_vert;
			glm::vec3 temp_vec;


			//extracting vertex data
			temp_vec.x = m->mVertices[i].x;
			temp_vec.y = m->mVertices[i].y;
			temp_vec.z = m->mVertices[i].z;

			temp_vert.v_coord = temp_vec;



		//extracting texture data

			if (m->mTextureCoords[0])//if mesh has texture coord
			{
				//std::cerr << "G" << std::endl;
				glm::vec2 temp_vec2;

				temp_vec2.x = m->mTextureCoords[0][i].x; 
				temp_vec2.y = m->mTextureCoords[0][i].y;
			
				temp_vert.t_coord = temp_vec2;
			}
			else
			{

				temp_vert.t_coord = glm::vec2(0.0f, 0.0f);

			}

			v.push_back(temp_vert);

		}


		for (unsigned int i = 0; i < m->mNumFaces; ++i)//filling index vector index
		{

			aiFace f = m->mFaces[i];

			for (unsigned int j = 0; j < f.mNumIndices; ++j)
			{
				index.push_back(f.mIndices[j]);
			}

		}

		
		if (m->mMaterialIndex >= 0)
		{
			aiMaterial * mat = s->mMaterials[m->mMaterialIndex];

			std::vector<texture> diffuseMaps = load_mat_tex(mat,aiTextureType_DIFFUSE, "texture_diffuse");
			t.insert(t.end(), diffuseMaps.begin(), diffuseMaps.end());
			
			std::vector<texture> specularMaps =load_mat_tex(mat, aiTextureType_SPECULAR, "texture_specular");
			t.insert(t.end(), specularMaps.begin(), specularMaps.end());

			std::vector<texture> normalMaps = load_mat_tex(mat, aiTextureType_HEIGHT, "texture_normal");
			t.insert(t.end(), normalMaps.begin(), normalMaps.end());
			
			std::vector<texture> heightMaps = load_mat_tex(mat, aiTextureType_AMBIENT, "texture_height");
			t.insert(t.end(), heightMaps.begin(), heightMaps.end());
		
		}
		

		return mesh(v, index, t);
	}


	std::vector<texture> model::load_mat_tex(aiMaterial *mat, aiTextureType type, std::string type_name)
	{
		std::vector<texture> t;

		for (unsigned int i = 0; i < mat->GetTextureCount(type); ++i)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			bool skip=false;

			for (unsigned int j = 0; j < loaded_tex.size(); ++j)//check if texture was previously loaded (if yes then use that)
			{
				if (std::strcmp(str.C_Str(), loaded_tex[i].path.C_Str()) == 0)
				{
					t.push_back(loaded_tex[j]);
					skip = true;
					break;

				}

			}


			
			if (!skip)//if texture hasn't been loaded previously only then load it afresh
			{
				texture temp_tex;
				temp_tex.ID = TextureFromFile(str.C_Str(), dir);
				temp_tex.type = type_name;
				temp_tex.path = str;
				t.push_back(temp_tex);
				loaded_tex.push_back(temp_tex);
			}
		
		}

		return t;

	}


	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma)//simply loads data from an image file in a directory (in the same directory where model object is present)
		//and using this data creates a texture  (it is a helper function and can be placed in resfun.cpp within N_model namespace)
	{
		std::string filename = std::string(path);
		filename = directory + '\\' + filename;

		unsigned int textureID;
		glGenTextures(1, &textureID);

		int width, height, nrComponents;
		unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
		if (data)
		{
			GLenum format;
			if (nrComponents == 1)
				format = GL_RED;
			else if (nrComponents == 3)
				format = GL_RGB;
			else if (nrComponents == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			stbi_image_free(data);
		}
		else
		{
			std::cout << "Texture failed to load at path: " << path << std::endl;
			stbi_image_free(data);
		}

		return textureID;

		chk();
	}





};