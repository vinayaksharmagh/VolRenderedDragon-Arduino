
namespace N_app
{


	class c_prog_m : public abst
	{
		GLuint vert_shad;
		GLuint cntrl_shad;
		GLuint eval_shad;
		GLuint geom_shad;
		GLuint frag_shad;
	public:
		void pure(app* th);

	};



	class c_storage_block :public abst
	{
		struct node //application representation of node struct
		{
			glm::vec4 color;
			float depth;
			int facing;
			unsigned int next;
		};


	public:
		void pure(app* th);


	};

	class c_count :public abst
	{

	public:
		void pure(app* th);


	};


	class c_image :public abst
	{

	public:
		void pure(app* th);


	};



	class c_matrix :public abst
	{
		static glm::quat q;
		static float ang_x, ang_y, ang_z;
		static float pang;//previous angle


	public:
		void pure(app* th);


	};

	class c_serial_in :public abst
	{
		static int pval;
		static const char* port_n;

		static char data[MAX_DATA_LENGTH];
	public:
		void pure(app* th);

	};


	class c_serial_out :public abst
	{
		static const char* port_n;
		static char data[MAX_DATA_LENGTH];
		static char signal_out;

	public:
		void pure(app* th);


	};





};
