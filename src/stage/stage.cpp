#include <algorithm>
#include "../../../utiliteam/submodules/eigen-git-mirror/Eigen/Core"
#include "../../../utiliteam/submodules/eigen-git-mirror/Eigen/Dense"
#include <casmutils/definitions.hpp>
#include <casmutils/stage.hpp>
#include <filesystem>
#include <iostream>
#include <CLI/CLI.hpp>
#include "CLI/App.hpp"
#include "CLI/Formatter.hpp"
#include "CLI/Config.hpp"

int main() { 
	CLI::App app{"This app takes in either a coordinate or the dimensions of a mesh and outputs equivalent orbits"};
        app.add_option_function<std::filesystem::path> ("-s, --structure", structure, "Please input the base structure")-> required();
	app.add_option_function<Eigen::Vector3d> ("-p, --point", coordinate, "Please input a sinlge point that you would like to find the orbit of within the structure");
	app.add_option_function<std::string> ("-a, --atomtype", atom_type, "Please put the names of the different atoms in the structure")
	app.add_option_function<double> ("-d, --distances", distances, "Please input a list of the distances between the interstitial coordinates and each atom type in the base structure");
	app.add_option_function<Eigen::Vector3d> ("-m, --mesh", mesh, "Please input the mesh dimensions in 'x,y,z' that you would like to examine in the base structure");	
	return 0; 
}

