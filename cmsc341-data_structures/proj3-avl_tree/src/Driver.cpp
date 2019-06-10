#include "AVLForest.h"
#include <fstream>

// we know data is either char or string
// we know numbers are either int or float

int main(int argc, char** argv)
{
  // avl forest to be populated
  AVLForest avlForest;

  // read input from file
  std::ifstream inputFile;
  std::ifstream rangeFile;
  std::string line;

  // open file for reading
  inputFile.open(argv[1], std::ios::in);

  // read all lines
  while(std::getline(inputFile, line))
  {
    // process each line
    avlForest.insert(line);
  }

  // print all trees
  std::cout<<"\n==============\n";
  std::cout<<"PRINTING TREES";
  std::cout<<"\n==============\n";
  std::cout<<avlForest;

  // print ranges
  std::cout<<"\n===============\n";
  std::cout<<"PRINTING RANGES";
  std::cout<<"\n===============\n";
  rangeFile.open(argv[2], std::ios::in);
  
  while(std::getline(rangeFile, line))
  {
    avlForest.print_range(std::cout, line);
  }
}
