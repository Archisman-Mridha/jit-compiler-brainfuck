#ifndef _JIT_COMPILER_BRAINFUCK_UTILS
#define _JIT_COMPILER_BRAINFUCK_UTILS

  #include <fstream>
  #include <iostream>
  #include <sstream>

  using namespace std;

  string readFile(const string& path) {
    ifstream file(path);
    if(!file.is_open()) {
      cerr<<"Failed opening file "<<path<<endl;
      exit(1);
    }

    stringstream contents;
    contents << file.rdbuf();

    file.close();

    return contents.str();
  }

#endif _JIT_COMPILER_BRAINFUCK_UTILS
