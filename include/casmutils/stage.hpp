#ifndef UTILS_STAGE_HH
#define UTILS_STAGE_HH

#include "casmutils/definitions.hpp"
#include "casmutils/structure.hpp"

namespace Rewrap
{
    vector<SymOp> getfactor_group(struc my_Structure)
    vector<SymOp> gettrace(vector<SymOp> myfactorgroup)
    vector<double> geteigenvals(struc my_Structure, vector<SymOp> myfactorgroup)
    bool test_for_identity(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    bool test_for_inversion(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    bool test_for_mirrorplane(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    bool test_for_rotation(struc my_Structure, vector<SymOp> trace, vector<double> eigenvals)
    vector<double> get_inversionpoint()
    vector<vector<double>> get_mirrorplane()
    vector<double> get_rotationaxis()
}
#endif
