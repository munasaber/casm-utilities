#include "casmutils/stage.hpp"
#include <casm/crystallography/Structure.hh>
#include <casm/clex/ScelEnum.hh>
#include <casm/crystallography/SupercellEnumerator.hh>
#include "casmutils/exceptions.hpp"
#include "casmutils/structure.hpp"
#include "casmutils/lattice.hpp"
#include <casm/crystallography/Niggli.hh>



//how do we find out what's in the namespace of Simplicity again
//Namespace 
//int main()
//{
//void returnCART()
//{
//std::cout<<"Nice haircut"<<std::endl;
//return;
//}

   
  // double repeatedsites(x, y, z)
   //{
	   //grab inputs and do transformation, call actual function in stage.cpp?
	   //I'm really not sure what this things for, but I think its just to declare the simplicity    namespace (and possibly other namespaces?)
   //}

//} // namespace SuperBoxy
//TODO: Make this take a lattice by pointer and make it standalone function
  void Lattice::symmetrize(const SymGroup &relaxed_pg) {
    Eigen::Matrix3d tLat2(Eigen::Matrix3d::Zero());
    Eigen::Matrix3d frac_mat;
    for(Index ng = 0; ng < relaxed_pg.size(); ng++) {
      frac_mat = iround(inv_lat_column_mat() * relaxed_pg[ng].matrix() * lat_column_mat()).cast<double>();
      tLat2 += frac_mat.transpose() * lat_column_mat().transpose() * lat_column_mat() * frac_mat;
    }

    tLat2 /= double(relaxed_pg.size());

    // tLat2 has the symmetrized lengths and angles -- it is equal to L.transpose()*L, where L=lat_column_mat()
    // we will find the sqrt of tLat2 and then reorient it so that it matches the original lattice
    Eigen::Matrix3d tMat(tLat2), tMat2;

    Eigen::JacobiSVD<Eigen::Matrix3d> tSVD(tMat, Eigen::ComputeFullU | Eigen::ComputeFullV);
    tMat = Eigen::Matrix3d::Zero();
    for(int i = 0; i < 3; i++) {
      tMat(i, i) = tSVD.singularValues()[i];
    }

    tMat2 = tSVD.matrixU() * tMat * tSVD.matrixV().transpose();

    tMat = lat_column_mat();

    tSVD.compute(tMat2 * tMat.transpose());

    tMat = tSVD.matrixV() * tSVD.matrixU().transpose() * tMat2;

    for(int i = 0; i < 3; i++) {
      for(int j = 0; j < 3; j++) {
        tLat2(i, j) = tMat(i, j);
      }
    }

    (*this) = Lattice(tLat2);



    return;
  }
