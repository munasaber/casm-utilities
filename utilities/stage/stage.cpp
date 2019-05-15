
//Am I supposed to base the location of the include on where I am now? For example, would I need to state ../../../CASMcode/include/casm/crystallography/Coordinate.hh?
#include "casmutils/stage.hpp"
#include "casm/crystallography/Structure.hh"
#include "casm/casm_io/VaspIO.hh"
#include "casmutils/structure.hpp"
#include "casm/crystallography/Coordinate.hh"
#include "casmutils/definitions.hpp"
#include "casmutils/handlers.hpp"
#include <fstream>
#include <iostream>
#include <boost/program_options.hpp>
#include "CASMcode/include/casm/symmetry/SymOp.hh"
#include <cstdlib>
#include <cmath>
#include "casm/crystallography/Lattice.hh"


namespace Utilities
{

void factor_group_initializer(po::options_description& factor_group_desc)
{
    UtilityProgramOptions::add_help_suboption(factor_group_desc);
    UtilityProgramOptions::add_desc_suboption(factor_group_desc);
    UtilityProgramOptions::add_output_suboption(factor_group_desc);
    factor_group_desc.add_options()("structure", po::value<fs::path>()->required(),
                                   "POS.vasp like file you want to "
                                   "find all equivalent lithium sites for");
    //to my knowledge I am trying to create a dynamic array of vectors, each with an array of size 3?
//    factor_group_desc.add_options()("sites", po::value<std::vector<vector<double>>3>()->multitoken()->required(),
  //                                 "Proposed Li sites to find equivalent sites for"
    //                               " units are fractional");
      factor_group_desc.add_options()("sites,l", po::value<std::vector<double>>()->multitoken()->required(),
                               "location of new sites (arrays of three values "
                               "corresponding to a, b, and c)");


    return;
}

}//I'm not adding to the namespace of utilities write, im just using it? why is their no "using" arguement?

//Is adding to the namespace okay? 
using namespace Utilities;

int main(int argc, char* argv[])
{
	  Handler factor_group_launch(argc, argv, factor_group_initializer);
	 
 if (factor_group_launch.count("help"))
    {
        std::cout << factor_group_launch.desc() << std::endl;
        return 1;
    }

    try
    {
        factor_group_launch.notify();
    }

    catch (po::required_option& e)
    {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    auto raw_size = factor_group_launch.fetch<std::vector<double>>("sites");
    std::cout<<raw_size;
		  //programOptions
          // std::string  poscar_path="/home/muna/Desktop/Research/TernaryPhaseDiagram/DensityofStates/DensityofStates_B13C2/POSCAR";
	  auto struc_path = factor_group_launch.fetch<fs::path>("structure");
          auto struc = Rewrap::Structure(struc_path);
       // auto out_struc = struc;
      
	  CASM::Coordinate site;
	
          // Simplicity::write_poscar(struc, struc_path+"_new");
	  Simplicity::write_poscar(struc, struc_path);
	  Simplicity::print_poscar(struc, std::cout);


    //find factor group of structure
          CASM::MasterSymGroup factor_group=struc.factor_group();
	//  std::cout <<"the factor group type is "<< typeid(factor_group).name()<< endl;
          std::cout << factor_group.size()<<std::endl;
          


	  //Get coordinates 
	  auto coords=struc.basis;
          
	
      //get SymOps from MasterSymGroup somehow
      //SymOp(const Eigen::Ref<const matrix_type> &_mat = matrix_type::Identity(),
   //       const Eigen::Ref<const vector_type> &_tau = vector_type::Zero(),
    //      double _map_error = TOL) :
   //   SymOp(_mat, _tau, _map_error, -1, nullptr) {
   // }




      
 //       auto vec = factor_group_launch.fetch<std::vector<double>>("sites");
       //go from fractional to Cartesian (Fractional and Cartesian are vague abstract concepts with no basis in reality, apparently uneeded)
    	//  std::istream main_input;
 	//     bool mode=1;
    	//     CASM::Coordinate::read(main_input(struc_path), CASM::COORD_MODE::CHECK())
   
      // set_lattice(lattice, COORD_TYPE CART);
    


       //may want to start with a single site, later add multiple sites with the and() or() notation in CASM
     


      //Apply factor group to new site
      //CASM::Coordinate &apply_sym(const SymOp &op);
     

	  std::vector<CASM::Coordinate> final_coords;
      for (int j=0;j < factor_group.size(); j=j+1)//this should apply transformation on xyz and lattice for every symop in array probably 
        
	
        	{
        	    final_coords[j]= site.apply_sym(factor_group[j]);   
        	 
		}
	
 


     //may be an easier way to write out Poscar
       Simplicity::print_poscar(final_struc, std::cout)

    //Simplicity::trivial_muna_function();
//    return 0;

    }
