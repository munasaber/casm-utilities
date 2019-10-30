#include "casmutils/stage.hpp"
#include <casm/crystallography/Structure.hh>
#include <casm/clex/ScelEnum.hh>
#include <casm/crystallography/SupercellEnumerator.hh>
#include "casmutils/exceptions.hpp"
#include "casmutils/structure.hpp"
#include "casmutils/lattice.hpp"
#include <casm/crystallography/Niggli.hh>


#If you have a particular structure, find the inversion points, rotation axes, and mirrorplanes
namespace Rewrap
{
    vector<SymOp> getfactor_group(struc my_Structure)
    vector<SymOp> gettrace(vector<SymOp> myfactorgroup)
    vector<double> geteigenvals(struc my_Structure, vector<SymOp> myfactorgroup)
    bool test_for_identity(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    if  
    bool test_for_inversion(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    testmatrix=[-1 0 0; 0 -1 0; 0 0 -1]
    #SymOp.character() gives trace
    bool test_for_mirrorplane(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    bool test_for_rotation(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    vector<double> get_inversionpoint()
    vector<vector<double>> get_mirrorplane()
    vector<double> get_rotationaxis()

} // namespace SuperBoxy
