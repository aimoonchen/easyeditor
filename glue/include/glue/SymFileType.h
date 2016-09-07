#ifndef _GLUE_SYM_FILE_TYPE_H_
#define _GLUE_SYM_FILE_TYPE_H_

#include <string>

namespace glue
{

enum SymFileType
{
	UNKNOWN		= 0,
	IMAGE,
	SCALE9,
	TEXTBOX,
	COMPLEX,
};

SymFileType get_sym_file_type(const std::string& filepath);

std::string get_sym_tag_str(SymFileType type);

}

#endif // _GLUE_SYM_FILE_TYPE_H_