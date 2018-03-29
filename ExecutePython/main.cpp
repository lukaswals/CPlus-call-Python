#include <iostream>
#include <Python.h>

int main(int argc, char *argv[])
{
	// Execute Python code through C++
	/*
	wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);  // optional but recommended
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print('Today is', ctime(time()))\n");
	if (Py_FinalizeEx() < 0) {
		exit(120);
	}
	PyMem_RawFree(program);
	return 0;
	*/

	// Execute a Python file through C++
	// We expect 3 arguments: program name, Python file name, and Python path
	if (argc < 3) { 
		std::cerr << "Usage: " << argv[0] << " <PYTHON FILE>" << " <PYTHON LIB PATH>" << std::endl;
		std::cerr << "Example: " << argv[0] << " \"hello.py\"" << " \"C:/Anaconda3/envs/py3env/\"" << std::endl;
		return 1;
	}

	wchar_t *program = Py_DecodeLocale(argv[0], NULL);
	if (program == NULL) {
		fprintf(stderr, "Fatal error: cannot decode argv[0]\n");
		exit(1);
	}
	Py_SetProgramName(program);  // optional but recommended
	// Set Python Home, because even if we execute this program on a terminal
	// with virtualenv activated, it does not use the virtualenv path, thus
	// not founding the installed modules
	wchar_t *pythonhome = Py_DecodeLocale(argv[2], NULL);
	Py_SetPythonHome(pythonhome);
	
	Py_Initialize();
	// For some reason, the sys.argv variable is empty, leading to the following
	// error -> AttributeError: module 'sys' has no attribute 'argv'
	// That's why we need the next line...
	PyRun_SimpleString("import sys\n"
		"sys.argv = ['']");

	FILE *file = _Py_fopen(argv[1], "r+");
	if (file != NULL) {
		PyRun_SimpleFile(file, argv[1]);
	}

	Py_Finalize();
	return 0;
}