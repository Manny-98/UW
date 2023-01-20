//#include <fstream>
//#include <iostream>
//using namespace std;
//
//int main() {
//  const unsigned short dimx = 100, dimy = 100, max = 255;
//
//  ofstream ofs("test.ppm", ios_base::out | ios_base::binary);
//  ofs << "P6" << endl << dimx << ' ' << dimy << endl << max << endl;
//
//  for (auto j = 0; j < dimy; ++j)
//    for (auto i = 0; i < dimx; ++i)
//      ofs << (char)191 << (char)87 << (char)0; // red, green, blue
//
//  ofs.close();
//
//  return 0;
//}
