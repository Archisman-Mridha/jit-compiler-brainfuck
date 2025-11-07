module;

#include <fstream>
#include <iostream>
#include <sstream>

export module utils;

using namespace std;

export string read_file(const string& path) {
  ifstream file(path);
  if (!file.is_open( )) {
    cerr << "Failed opening file " << path << endl;
    exit(1);
  }

  stringstream contents;
  contents << file.rdbuf( );

  file.close( );

  return contents.str( );
}
