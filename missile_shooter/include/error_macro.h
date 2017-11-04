#pragma once
#include <iostream>


//
//This macro provides a formatted description of the file and location of the error want to report
//custom_description is your own descriptions
#define CERR_ERROR(custom_description) std::cerr << "\n--ERROR START--" << std::endl \
 << "File: " << __FILE__ << std::endl \
<< "Line: " << __LINE__ << std::endl \
<< custom_description << std::endl \
<< "--ERROR END--\n" << std::endl