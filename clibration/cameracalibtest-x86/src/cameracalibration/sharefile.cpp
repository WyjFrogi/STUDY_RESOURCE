#include "sharefile.h"


string get_current_path()
{
	char curr_path[1024];
#ifdef _WIN32
	::GetModuleFileName(NULL, curr_path, MAX_PATH);
	(_tcsrchr(curr_path, '\\'))[1] = 0;
#else	
	getcwd(curr_path, 1024);
	sprintf(curr_path, "%s/", curr_path);
#endif
	string str_curr_path = curr_path;
	return str_curr_path;
}

