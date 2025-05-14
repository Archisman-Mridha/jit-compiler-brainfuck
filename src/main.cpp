#include <iostream>

import lexer;
import ir;
import utils;

using namespace std;

/*
  The language consists of eight commands. A brainfuck program is a sequence of these commands,
  possibly interspersed with other characters (which are ignored). The commands are executed
  sequentially, with some exceptions : an instruction pointer begins at the first command, and each
  command it points to is executed, after which it normally moves forward to the next command. The
  program terminates when the instruction pointer moves past the last command.

  The brainfuck language uses a simple machine model consisting of :

    (1) the program and instruction pointer

    (2) a one-dimensional array of at least 30,000 byte cells initialized to zero

    (3) a movable data pointer (initialized to point to the leftmost byte of the array)

    (4) two streams of bytes for input and output (most often connected to a keyboard and a monitor
        respectively, and using the ASCII character encoding).
*/

int main(int argc, char* argv[]) {
  if (argc != 2) {
    cerr << "Wrong number of command-line arguments" << endl;
    cerr << "Usage : jit-compiler-brainfuck <input.bf>" << endl;
    exit(1);
  }

  const auto inputFilePath     = argv[1];
  const auto inputFileContents = readFile(inputFilePath);

  const auto lexer = new Lexer(inputFileContents);

  const auto irGenerator = new IRsGenerator(lexer);
  const auto irs         = irGenerator->generate( );

  return 0;
}
