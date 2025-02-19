#include <igl/read_triangle_mesh.h>
//#include <igl/write_triangle_mesh.h>
// Use igl::xml version to support .dae collada files, too
#include <igl/xml/write_triangle_mesh.h>
#include <igl/pathinfo.h>
#include <igl/qslim.h>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdlib>

int main(int argc, char * argv[])
{
  using namespace std;
  using namespace Eigen;
  using namespace igl;
  MatrixXd V;
  MatrixXi F;
  string in,out;
  double f_in = 0.1;
  int start = 0;
  bool block_intersections = false;
  bool use_trivial = false;
  switch(argc)
  {
    case 5:
    {
      std::string flags = argv[start+1];
      start++;
      block_intersections = flags.find("b") != std::string::npos;
      use_trivial = flags.find("t") != std::string::npos;
    }
    case 4:
      f_in = atof(argv[start+1]);
      in =        argv[start+2];
      out =       argv[start+3];
      break;
    default:
      cerr<<R"(
USAGE:
  decimate [flags|optional] [fraction|whole number] input.[mesh|msh|obj|off|ply|stl|wrl] output.[dae|mesh|obj|off|ply|stl|wrl]

FLAGS:
  t  use "trivial" decimation (rather than qslim)
  b  block edge collapses that would cause new intersections

)";
    return EXIT_FAILURE;
  }
  read_triangle_mesh(in,V,F);
  const int max_m = (f_in < 1?f_in*F.rows(): f_in);
  MatrixXd dV;
  MatrixXi dF;
  {
    Eigen::VectorXi J,I;
    if(use_trivial)
    {
      decimate(V,F,max_m,block_intersections,dV,dF,J,I);
    }else
    {
      qslim(V,F,max_m,block_intersections,dV,dF,J,I);
    }
  }
  return xml::write_triangle_mesh(out,dV,dF,igl::FileEncoding::Binary) ? 
      EXIT_SUCCESS : EXIT_FAILURE;
}
