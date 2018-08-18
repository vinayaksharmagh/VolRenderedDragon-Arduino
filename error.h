#include<ktx.h>
void check(const char * file, int line);
void check(KTX_error_code err_code, const char *file, int line);

#define chk() check(__FILE__,__LINE__)

#define chk_ktx(E_C)  check(E_C,__FILE__, __LINE__)

void error(std::string s);


//////////////////////////////////////////////////////////////A///////////////////////////////////////////////////////////////
//here sh, pro are being passed to check functions by value instead of by reference(as done in prev programs) , this is done to prevent 
//leakage of original data members of class 

void check_compile(GLuint sh, std::string name);

void check_link(GLuint pro, std::string name);
//////////////////////////////////////////////////////////////A///////////////////////////////////////////////////////////////