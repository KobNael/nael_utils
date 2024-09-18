#include <boost/program_options.hpp>
#include <iostream>

#include "dto/in/DtoIn.hh"
#include "json/json_handler.hh"

namespace po = boost::program_options;

int exitAndReturn(int retCode_p){
	//Return
	return retCode_p;
}

int processArgs(po::variables_map const &vm_p)
{
	//input
	if( vm_p.count("input-file") )
	{
        dto::DtoIn myContext;
		json::import(vm_p["input-file"].as< std::string >(), &myContext);
	}

	return exitAndReturn(0);
}

int main(int argc, char* argv[])
{
	std::ostringstream usage;
	try {
		po::options_description desc("Available options");
		desc.add_options()
			("help,h", "Print this message")
			("input-file,i", po::value< std::string >(), "Input file (.dbg | .json)")
			("output-file,o", po::value< std::string >(), "Output file (.json)")
		;

		usage << "Usage: options_description [options]" << std::endl;
		usage << desc;

		po::positional_options_description pod;
		pod.add("input-file", 1);
		pod.add("output-file", 1);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).
					options(desc).positional(pod).run(), vm);
		po::notify(vm);

		//Affichage de l'aide
		if( vm.empty() || vm.count("help") )
		{
			std::cout << usage.str();
			//Always exit if help required
			return exitAndReturn(0);
		}
		else
		{
			return processArgs(vm);
		}
	}
	//Catch boost program options exception
	catch(std::exception& e)
	{
		std::cerr << "Error in command line : " << e.what();
		std::cerr << usage.str();
		return exitAndReturn(1);
	}
}

