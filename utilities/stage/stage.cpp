// Am I supposed to base the location of the include on where I am now? For example, would I need to state
// ../../../CASMcode/include/casm/crystallography/Coordinate.hh?
#include "casmutils/stage.hpp"
#include "CASMcode/include/casm/symmetry/SymOp.hh"
#include "casm/casm_io/VaspIO.hh"
#include "casm/crystallography/Coordinate.hh"
#include "casm/crystallography/Lattice.hh"
#include "casm/crystallography/Structure.hh"
#include "casmutils/definitions.hpp"
#include "casmutils/handlers.hpp"
#include "casmutils/structure.hpp"
#include <boost/program_options.hpp>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>

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
    // to my knowledge I am trying to create a dynamic array of vectors, each with an array of size 3?
    //    factor_group_desc.add_options()("sites", po::value<std::vector<vector<double>>3>()->multitoken()->required(),
    //                                 "Proposed Li sites to find equivalent sites for"
    //                               " units are fractional");
    factor_group_desc.add_options()("sites,l", po::value<std::vector<double>>()->multitoken()->required(),
                                    "location of new sites (arrays of three values "
                                    "corresponding to a, b, and c)");

   /// po::value<std::vector<double>>()->multitoken()->required(),
   //po::value<std::string>()->required()
    return;   
}

} // namespace Utilities



//sooo.... do I just copy these two to .cxx?
using namespace Utilities;

//get sites
//CASM::Coordinate extract_site(const std::string string_coords, const CASM::Lattice &lattice) //or is it CASM::BasicStructure
CASM::Coordinate extract_site(const std::vector<double> &site_coords, const CASM::Lattice &lattice) 
{
        std::cout<<"might as well put it here to test"<<std::endl;
      //	std::vector<std::string> xyz_string(3);
        auto site_vectors=site_coords; 
         
	CASM::Coordinate site(site_vectors.front(), site_vectors.at(1), site_vectors.back(), lattice, CASM::FRAC); //do I edit the inputs to the Constructor? Help! I'll also need to do recursive if multiple inputs
        return site;
}


//check whether or not the site with applied symmetry group is repeated when the site has other factor groups applied
bool already_contains(const std::vector<CASM::Coordinate>& running_list, const CASM::Coordinate& candidate)
{
     int k=0;
     for(int i=0; i< running_list.size(); i++) 
     {
	     
	     //if (running_list[i].dist(candidate)==0.001) ....
     	     if (running_list[i]==candidate)
	     {
		   k=k+1;
	     }		    
	     
      }	     
     if (k>=1)
     {
	     return true;
     }
     else 
	     return false;

	     
}

int main(int argc, char* argv[])
{
    Handler factor_group_launch(argc, argv, factor_group_initializer);
    std::cout<<"after factor_group_launch"<<std::endl;
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
    std::cout<<"after help"<<std::endl;

  

    auto struc_path = factor_group_launch.fetch<fs::path>("structure");
    auto struc = Rewrap::Structure::from_poscar(struc_path);
    std::cout<<"after structure input"<< std::endl;
    CASM::Lattice lattice= struc.lattice();
    std::cout<<"after lattice"<<std::endl;

   // auto site_coords_string = factor_group_launch.fetch<std::string>("sites");
    auto site_coords=factor_group_launch.fetch<std::vector<double>>("sites");
   // std::cout<<site_coords_string<<std::endl;
   // CASM::Coordinate site = extract_site(site_coords_string, lattice);
      CASM::Coordinate site = extract_site(site_coords, lattice);
   
    std::cout<<"after site extraction"<<std::endl;
    
    
   
    // find factor group of structure
    CASM::MasterSymGroup factor_group = struc.factor_group();
//    std::cout<<factor_group<<endl;
    std::vector<CASM::Coordinate> final_coords;
    std::cout<<"The size of the factor group is "<<factor_group.size();
    // Apply each factor group operation on the user provided site, and store transformed
    // values
    for (int j = 0; j < factor_group.size(); ++j)
    {
        auto tmp = site;

        tmp.apply_sym(factor_group[j]);
        tmp.within();   //maybe?
        if (!already_contains(final_coords, tmp))
        {
            final_coords.push_back(tmp);
        }
        /* final_coords.emplace_back(std::move(tmp.apply_sym(factor_group[j]))); */
    }
   // final_coords.print();
  for (int j=0; j<final_coords.size(); j++)
  {
    final_coords[j].print(std::cout, CASM::FRAC); 
    std::cout<<std::endl;
  }



  //Additionally aspects that tells space group of material? That's not trivial is it?
  //Anton wanted something else but I forgot what it was. 
  //Additional option! Input site and test if the same (or some) atom is on equivalently symmetric sites(maybe not even input...). If so, maybe break down unit cell into all atoms that are unique? 
     //plan
     //Take POSCAR coords from first coords.
     //Apply symmetry operations (we should know factor_group from previous thing)
     //Test generated coords against real coords. Go through the entire thing
     //test if number of real coords that match symmetric coords=< generated coords 
     //Generate new POSCAR with same lattice sizes
             //From new POSCAR/coords (iterate over coords)
	     //final_coords.emplace(original lattice)
	     //if generated coord==real coord (outside of original) final_coords.erase(tmp)
	     //
    for (int j=0; j< factor_group.size(); j++)
    {
	   for (int i=0; i<site_coords.size(); ++i)
	   {
	     auto tmp =site[i]
             tmp.apply_sym(aj] 
	     if (!already_contains(final_coords, tmp))   //new function? I'm a bit confused...
	     {
	       if (!symmetric_equivalent(final_coords, tmp)  //type out symeetric equivalent
	       {
                  final_coords.push_back(tmp)
		}
	    
	   //maybe need two if statements. One to make sure no repeats, other to ensure symmetric equivlance?
            }
    }
	    
   //Print out final coordinates into a POSCAR... should this be a seperate function or does it always do it simultaneously? 
	    
               //these for loops should be switched...


}
