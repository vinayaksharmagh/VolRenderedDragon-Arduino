namespace N_app
{


	class app
	{


		int con_w, con_h, vp_w, vp_h;
		float n_plane, f_plane,fov;
		float view_depth;//-z coord put in view matrix
		GLuint prog[1];
		GLuint prog_m[4];
		GLuint app_vao;
		GLuint counter_buff;
		GLuint storage_blk_buff;
		GLuint img_tex;
		
		glm::mat4 M_composite;
		
		//following are parts of information which will be extracted from string recieved from Arduino 
		int val;
		//int pval;
		int x_ax;
		int y_ax;
		int z_ax;







		glm::vec4 background;

	public:
		static const float PI;
		
		//app();
		~app()
		{
			glDeleteProgram(prog[0]);
			glDeleteProgram(prog_m[0]);
			glDeleteVertexArrays(1, &app_vao);

		}
		void render();
		void startup();//for taskes and computations just before rendering
		void init();
		void run();
		inline float get_max_proj_y() { return n_plane*tan(fov / 2.0f); } //returns max y coord of near plane(near plane is also called projection plane)
		inline float get_proj_y(float y, float z,float max_proj_y)  //returns projected y coord( on near plane) corrosponding to actual y
		{                 
			z = z - view_depth;//since view trasformation happens later so we need to substract by view_depth (because in view we are sending particles view_depth units forward(in -ve z axis) to imitate its effects
			return -1 * (n_plane*(y / z));
		
		}
		inline float get_max_unclip_y(float z, float max_proj_y)//returns max actual y coord that will get successfully projected on near plane without getting clipped
		{
			z = z - view_depth;
			return -1 * (max_proj_y * (z / n_plane));
		}

		
		friend class c_prog_m;
		friend class c_count;
		friend class c_storage_block;
		friend class c_image;
		friend class c_matrix;
		friend class c_serial_in;
		friend class c_serial_out;
		

		

	};

};