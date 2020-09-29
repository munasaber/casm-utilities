#include <algorithm>
#include "../../CASMcode/include/casm/external/Eigen/Core"
#include "../../CASMcode/include/casm/external/Eigen/Dense"
#include <casmutils/definitions.hpp>
#include <casmutils/xtal/structure.hpp>
#include <casmutils/xtal/structure_tools.hpp>
#include <casmutils/stage.hpp>
#include <filesystem>
#include <iostream>
#include <vector>
#include <CLI/CLI.hpp>
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"
#include "interstitial_mesh.cpp"

int main(int argc, char* argv[]) { 
	CLI::App app{"This app takes in either a coordinate or the dimensions of a mesh and outputs equivalent orbits"};
	casmutils::fs::path structurepath;
 //	app.add_option_function<std::filesystem::path> ("-s, --structure", structure, "Please input the base structure")-> required();
	CLI::Option* structure_path=app.add_option("-s, --structure", structurepath, "Please input the base structure")-> required();
	Eigen::Vector3d coordinate;
//	app.add_option_function<Eigen::Vector3d> ("-c, --coordinate", coordinate, "Please input a sinlge point that you would like to find the orbit of within the structure");
	CLI::Option* coordinate_path= app.add_option("-c, --coordinate", coordinate, "Please input a sinlge point that you would like to find the orbit of within the structure");
	//std::string atom_type;
	std::vector<std::string> atomtype;
	//app.add_option_function<std::string> ("-a, --atomtype", atom_type, "Please put the names of the different atoms in the structure")
	CLI::Option* atomtype_path= app.add_option("-a, --atomtype", atomtype, "Please put the names of the different atoms in the structure") -> allow_extra_args(true);
	std::vector<double> distances;
	//app.add_option_function<double> ("-d, --distances", distances, "Please input a list of the distances between the interstitial coordinates and each atom type in the base structure");
	CLI::Option* distances_path= app.add_option("-d, --distances", distances, "Please input a list of the distances between the interstitial coordinates and each atom type in the base structure");
	std::vector<double> mesh(3);
	//app.add_option_function<Eigen::Vector3d> ("-m, --mesh", mesh, "Please input the mesh dimensions in 'x,y,z' that you would like to examine in the base structure");	
	CLI::Option* mesh_path= app.add_option("-m, --mesh", mesh, "Please input the mesh dimensions in 'x,y,z' that you would like to examine in the base structure");	
	casmutils::fs::path outpath;
	CLI::Option* out_path= app.add_option("-o, --output", outpath, "Output path name");	
        structure_path->check(CLI::ExistingFile);
	CLI11_PARSE(app, argc, argv);
	std::cout<<"The chosen POSCAR is"<<structurepath<< std::endl;
	auto create_structure=casmutils::xtal::Structure::from_poscar(structurepath);
	casmutils::xtal::write_poscar(create_structure, outpath);	
	std::cout<<"The chosen atoms are ";
	std::cout<<std::endl;
	for (const auto& my_atom: atomtype)
	{
		std::cout<<my_atom<<std::endl;	
	}
	std::cout<<std::endl;
	std::cout<< "The chosen distances are ";
	std::cout<<std::endl;
        for (const auto& my_distance: distances)
	{
		std::cout<<my_distance<<std::endl;	
	}
	std::cout<<std::endl;
	std::cout<<"The chosen mesh is" << std::endl;	
	
	for (const auto& mesh_dimensions : mesh)
	{
		std::cout<<mesh_dimensions<<std::endl;	
	}
	return 0; 
}

