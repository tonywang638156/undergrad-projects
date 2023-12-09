#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>

using namespace std;

class Matrix {

private:

  static const int DEFAULT_SIZE{3};

  int numRows;
  int numCols;

  // a pointer to an array of double pointers
  double** values;  

public:
  //Both the first two constructors create matrices filled with zeros)
  Matrix();
  Matrix(int numRows, int numCols);
  Matrix(const Matrix& other);
  Matrix& operator=(const Matrix& other);
  Matrix(Matrix&& other)noexcept;
  Matrix& operator=(Matrix&& other)noexcept;
  Matrix operator+(const Matrix& other);
  Matrix& operator+=(const Matrix& other);
  ~Matrix();

  double get(int row, int col);
  int getNumRows();
  int getNumCols();
  void set(int row, int col, double value);
  void print();
};

#endif

