// Am I supposed to base the location of the include on where I am now? For example, would I need to state
// ../../../CASMcode/include/casm/crystallography/Coordinate.hh?
#include "casmutils/stage.hpp"
#include "casm/casm_io/VaspIO.hh"
#include "casm/crystallography/Coordinate.hh"
#include "casm/crystallography/Lattice.hh"
#include "casm/crystallography/Structure.hh"
#include "casm/symmetry/SymOp.hh"
#include "casmutils/definitions.hpp"
#include "casmutils/handlers.hpp"
#include "casmutils/lattice.hpp"
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
    // TODO: Make this one site, or implement multiple sites
    factor_group_desc.add_options()("sites,l", po::value<std::vector<double>>()->multitoken()->required(),
                                    "location of new sites (arrays of three values "
                                    "corresponding to a, b, and c)");
    return;
}

} // namespace Utilities

using namespace Utilities;

// get sites
// CASM::Coordinate extract_site(const std::string string_coords, const CASM::Lattice &lattice) //or is it
// CASM::BasicStructure

// check whether or not the site with applied symmetry group is repeated when the site has other factor groups applied
// TODO: use STL
bool already_contains(const std::vector<CASM::Coordinate>& running_list, const CASM::Coordinate& candidate)
{
    int k = 0;
    for (int i = 0; i < running_list.size(); i++)
    {
        // if (running_list[i].dist(candidate)==0.001) ....
        if (running_list[i] == candidate)
        {
            k = k + 1;
        }
    }
    if (k >= 1)
    {
        return true;
    }
    else
        return false;
}

int main(int argc, char* argv[])
{
    try
    {
        Handler factor_group_launch(argc, argv, factor_group_initializer);
        if (factor_group_launch.count("help"))
        {
            std::cout << factor_group_launch.desc() << std::endl;
            return 1;
        }

        factor_group_launch.notify();

        auto struc_path = factor_group_launch.fetch<fs::path>("structure");
        auto struc = Rewrap::Structure::from_poscar(struc_path);

        Rewrap::Lattice lattice = struc.lattice();

        // auto site_coords_string = factor_group_launch.fetch<std::string>("sites");
        auto site_coords = factor_group_launch.fetch<std::vector<double>>("sites");
        CASM::Coordinate site(site_coords[0], site_coords[1], site_coords[2], lattice, CASM::FRAC);

        // find factor group of structure
        CASM::MasterSymGroup factor_group = struc.factor_group();
        std::vector<CASM::Coordinate> final_coords;

        // Apply each factor group operation on the user provided site, and store transformed
        // values
        std::cout<<"Applying "<<factor_group.size()<<" operations from the factor group."<<std::endl;
        for (int j = 0; j < factor_group.size(); ++j)
        {
            auto tmp = site;

            tmp.apply_sym(factor_group[j]);
            tmp.within();

            if (std::find(final_coords.begin(), final_coords.end(), tmp) == final_coords.end())
            {
                final_coords.push_back(tmp);
            }
        }

        for (int j = 0; j < final_coords.size(); j++)
        {
            final_coords[j].print(std::cout, CASM::FRAC);
            std::cout << std::endl;
        }
    }

    catch (po::required_option& e)
    {
        std::cerr << e.what() << std::endl;
        return 2;
    }

    catch (boost::exception_detail::clone_impl<boost::exception_detail::error_info_injector<boost::program_options::unknown_option> >& e)
    {
        std::cout<<"Input format for coordinates must be within unit cell (no negative values)."<<std::endl;
        return 3;
    }

    return 0;
}
