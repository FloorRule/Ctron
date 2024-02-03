#pragma warning(disable : 4996)
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>

bool check_for_errors_in_code();

bool check_argv_errors(int argc, char* argv[]);

bool check_input_errors(int argc, char* argv[], std::string& out, std::string& in);

int main(int argc, char* argv[])
{
	
	if (check_argv_errors(argc, argv))
	{
		return 0;
	}

	std::string file_name = argv[1];
	std::string gcc_commend = file_name;
	std::string exe_string = "e";
	gcc_commend.replace(file_name.size() - 1, 1, exe_string);
	gcc_commend += "xe";

	if (check_input_errors(argc, argv, gcc_commend, file_name))
	{
		
		return 0;
	}

	std::string end_Command = "start gcc.exe out.s -o " + gcc_commend;

	Sleep(1000);
	std::cout << "Stage 1: Converting The File To LLVM Code: " << file_name << std::endl;
	std::string command = "start Flex_BisonV6.exe " + file_name + " out.ll";
	system(command.c_str());
	Sleep(500);

	if (!check_for_errors_in_code())
	{
		
		return 0;
	}


	Sleep(500);
	std::cout << "Stage 2: Converting The LLVM File To Assembly Code: " << std::endl;
	system("start llc.exe -march=x86 --x86-asm-syntax=intel out.ll");
	std::cout << "Stage 2: Done!" << std::endl;

	Sleep(500);
	std::cout << "Stage 3: Converting The Assembly File To An EXE file: " << std::endl;
	system(end_Command.c_str());
	std::cout << "Stage 3: Done!" << std::endl;

	Sleep(1000);
	std::cout << "EXE file result:" << std::endl;
	system(gcc_commend.c_str());

	
	return 0;
}

bool check_input_errors(int argc, char* argv[], std::string& out, std::string& in)
{
	if (argc == 4 && std::string(argv[2]) == "-o")
	{
		out = std::string(argv[3]);
		if (in.find(".c") == std::string::npos)
		{
			std::cout << "Ctron: fatal error: file type not supported\n";
			std::cout << "compilation terminated.\n";

			return true;
		}
		if (out.find(".exe") == std::string::npos)
		{
			out += ".exe";
		}
	}
	else if (argc == 3 && std::string(argv[2]) == "-o")
	{
		if (in.find(".c") == std::string::npos)
		{
			std::cout << "Ctron: fatal error: file type not supported\n";
			std::cout << "compilation terminated.\n";

			return true;
		}
	}
	else
	{
		std::cout << "Ctron: error: unrecognized command line option " << argv[2] << "\n";
		std::cout << "compilation terminated.\n";

		return true;
	}
	return false;
}

bool check_argv_errors(int argc, char* argv[])
{
	if (argc == 1)
	{
		std::cout << "Ctron: fatal error: no input files\n";
		std::cout << "compilation terminated.\n";

		return true;
	}
	if (argc > 2)
	{
		return false;
	}
	if (argc > 4)
	{
		std::cout << "Ctron: fatal error: too many arguments\n";
		std::cout << "compilation terminated.\n";

		return true;
	}
	
	
	if (argc == 2 && std::string(argv[1]) == "-help" || std::string(argv[1]) == "-h")
	{
		std::cout << "Usage:\n";
		std::cout << "  Ctron [options]\n";
		std::cout << "  Ctron [source_file] [command]\n";
		std::cout << "options:\n";
		std::cout << "	-h / -help               Display this information.\n";
		std::cout << "commands:\n";
		std::cout << "	-o <file>                Place the output into <file>.\n";
		std::cout << "	-o                       Place the output into [source_file.exe].\n";

		return true;
	}
	else
	{
		std::cout << "Ctron: error: unrecognized command line option " << argv[1] << "\n";
		std::cout << "compilation terminated.\n";

		return true;
	}
	return false;
}


bool check_for_errors_in_code()
{
	std::string line;
	std::string errors = "";

	std::ifstream myfile("dump.txt");
	if (myfile.is_open())
	{
		while (std::getline(myfile, line))
		{
			if (line != "")
			{
				errors += line;
				errors += "\n";
			}
		}
		myfile.close();
	}
	if (errors.size() == 0)
	{
		std::cout << "Stage 1: Done!" << std::endl;
		return true;
	}
	else
	{
		std::cout << "Stage 1: Errors Found!" << std::endl;
		std::cout << errors << std::endl;
		return false;
	}
}