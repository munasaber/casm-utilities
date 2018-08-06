#include "casmutils/stage.hpp"

std::vector<std::vector<std::pair<CASM::Site, double>>> find_nearest_neighbors(const Rewrap::Structure& struc,
                                                                               double max_radius)
{
    // need lattice for temporary point
    const auto& lattice = struc.lattice();
    //This will be the final return value
    //For a structure with N atoms, the outer length will be N,
    //each inner vector is made up of the neighbor sites, paired with
    //their distance to the center atom
    std::vector<std::vector<std::pair<CASM::Site, double>>> neighbor_analysis;
    // The periodic shifts of the unit cell will be stored here
    CASM::Coordinate shift_value(lattice);  //Uninitialized FRAC/CART so far

    // This should be read in from user input
    // dim stores a maximum range of the periodic boundary we
    // wish to explore
    auto dim = lattice.enclose_sphere(max_radius);
    // loop over each central site
    for (int i = 0; i < struc.basis.size(); i++)
    {
        //This atom will be shifted into the neighboring unit cells
        CASM::Site tatom(struc.basis[i]);
        // get distance to closest basis site in the unit cell at the origin
        // this is a container holding the sites within max radius of site i
        std::vector<std::pair<CASM::Site, double>> neighbors_and_dists;
        // loop over all other sites to find the distance
        for (int j = 0; j < struc.basis.size(); j++)
        {
            // This gives all the periodic images of the unit cell
            // that we will be checking for neighbors in
            CASM::EigenCounter<Eigen::Vector3i> grid_count(-dim, dim, Eigen::Vector3i::Constant(1));
            // loop over periodic images last
            do
            {
                // construct the current periodic shift value
                shift_value.frac() = grid_count().cast<double>();
                // this is the jth basis site with shift = shift_value
                CASM::Site tatomj(struc.basis[j] + shift_value);
                // this is the distance from site i to the shifted site j
                double dist = tatom.dist(tatomj);
                if (dist <= max_radius)
                {
                    neighbors_and_dists.push_back(std::make_pair(tatomj, dist));
                }
            } while (++grid_count);
        }
        neighbor_analysis.push_back(neighbors_and_dists);
    }
    //sort before returning
    return neighbor_analysis;
}
