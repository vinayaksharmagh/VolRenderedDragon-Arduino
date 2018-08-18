namespace N_model //contains all resources for extracting model from Assimp's data strcture
{

	struct vertex
	{
		glm::vec3 v_coord;
		glm::vec3 normal;
		glm::vec2 t_coord;
	};


	struct texture
	{
		unsigned int ID;//name of texture (given via glGenTextures)
		std::string type;
		aiString path;

	};

	

	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);

	class mesh
	{
		GLuint model_vao, VBO, EBO;

	public:

		std::vector<vertex> v_;
		std::vector<unsigned int> index_;
		std::vector<texture> t_;
		

		void draw( GLuint p );
		void setup_mesh();
		
		inline mesh(std::vector<vertex> v, std::vector<unsigned int> index, std::vector<texture> t) :v_(v), index_(index), t_(t)
		{
			setup_mesh();
		}
	};


	class model
	{
		std::vector<mesh> v_m;
		std::string dir;
		std::vector<texture>loaded_tex;
		static GLuint prog;//program object used for drawing current instance(*this) of model	

		void load_model(std::string path);
		void process_node(aiNode *n, const aiScene *s);
		mesh process_mesh(aiMesh *m, const aiScene *s);
		std::vector<texture> load_mat_tex(aiMaterial *mat, aiTextureType type, std::string type_name);

		inline static void set_prog(GLuint p) { prog = p; }

	public:
		

	 	inline model(std::string path ) 
		{

			load_model(path);


		}

		
		void draw( GLuint p);
		inline static GLuint get_prog() { return prog ; }

	};


};
