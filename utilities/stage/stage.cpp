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

    return;
}

} // namespace Utilities

// Is adding to the namespace okay?
using namespace Utilities;

CASM::Coordinate extract_site_somehow();
bool already_contains(const std::vector<CASM::Coordinate>& running_list, const CASM::Coordinate& candidate);

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

    auto struc_path = factor_group_launch.fetch<fs::path>("structure");
    auto struc = Rewrap::Structure::from_poscar(struc_path);

    CASM::Coordinate site = extract_site_somehow();

    // find factor group of structure
    CASM::MasterSymGroup factor_group = struc.factor_group();

    std::vector<CASM::Coordinate> final_coords;

    // Apply each factor group operation on the user provided site, and store trasnformed
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
}
