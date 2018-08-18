namespace N_app
{
	void load_shad(std::string name, GLuint & sh);
	//void fill_prog(GLuint *prog_arr);
}

glm::vec4 quad_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, float k);
glm::vec4 cubic_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, glm::vec4 D, float k);
glm::vec4 quantic_bezier(glm::vec4 A, glm::vec4 B, glm::vec4 C, glm::vec4 D, glm::vec4 E, float k);

void gen_tex(float* data, int w, int h);
void gen_shape(glm::vec4 *v_pos, glm::mat4 M_gen);

glm::mat3 t_trans(float x, float y);
glm::mat3 t_scale(float s_x, float s_y);
glm::mat3 t_rot(float ang);
