class context
{
	GLFWwindow *win;

public:

	context();
	context(int w, int h);
	~context();
	inline GLFWwindow * get_win() { return win; }
};
