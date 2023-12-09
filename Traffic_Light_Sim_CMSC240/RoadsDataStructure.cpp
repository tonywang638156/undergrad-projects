#include "RoadsDataStructure.h"

using namespace std;

// default constructor
Matrix::Matrix(): numRows{DEFAULT_SIZE}, numCols{DEFAULT_SIZE} {

  values = new double*[DEFAULT_SIZE];
  for (int row = 0; row < numRows; ++row) {
    values[row] = new double[numCols];
  }

  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      values[row][col] = 0;
    }
  }
}

// common constructor with given passing rows and cols
Matrix::Matrix(int numrows, int numcols)
	: numRows{numrows}, numCols{numcols} {
	// throw illege passing rows and cols
	if (numrows < 0 || numcols < 0) {
		stringstream ss;
		ss << "Attempt to create Matrix with invalid row or column" << endl;
		throw runtime_error(ss.str().c_str());
	}
	// creating rows
	values = new double*[numRows];
	// creating cols
	for (int row = 0; row < numRows; ++row) {
    	values[row] = new double[numCols];
	}
	// setting elements in Matrix as 0
	for (int row = 0; row < numRows; ++row) {
    	for (int col = 0; col < numCols; ++col) {
			values[row][col] = 0;
		}
	}
}

// copy constructor
Matrix::Matrix(const Matrix& other)
	: numRows{other.numRows}, numCols{other.numCols} {
	// creating the matrix
	values = new double*[other.numRows];
	for (int row = 0; row < numRows; ++row) {
    	values[row] = new double[other.numCols];
	}
	// setting elements in matrix as 0
	for (int row = 0; row < numRows; ++row) {
    	for (int col = 0; col < numCols; ++col) {
			values[row][col] = other.values[row][col];
		}
	}
}

// copy assignment
Matrix& Matrix::operator=(const Matrix& other) {
	if (this == &other) { // if it started the same, no need to copy
		return *this;
	}
	double** new_values = new double*[other.numRows];
	for (int row = 0; row < other.numRows; ++row) {
    	new_values[row] = new double[other.numCols];
	}
	// First delete each row (every row is an array
	for (int row = 0; row < numRows; ++row) {
		delete[] values[row];
	}
	delete[] values; // Then delete the array or pointers to arrays 
	// copy other matrix to this matrix
	values = new_values;
	numRows = other.numRows;
	numCols = other.numCols;
	for (int row = 0; row < numRows; ++row) {
    	for (int col = 0; col < numCols; ++col) {
			values[row][col] = other.values[row][col];
		}
	}
	return *this;
}

// move constructor
Matrix::Matrix(Matrix&& other)noexcept
	: numRows{other.numRows}, numCols{other.numCols}, values{other.values} {
	// put the old matrix into the move-from state
	other.values = nullptr;
	other.numRows = 0;
	other.numCols = 0;
}

// move assignment
Matrix& Matrix::operator=(Matrix&& other)noexcept {
	if (this == &other) { // if it started the same, no need to move
		return *this;
	}
	for (int row = 0; row < numRows; ++row) {
		delete[] values[row]; // First delete each row (every row is an array)
	}
	delete[] values; // Then delete the array or pointers to arrays
	// move from other Matrix to this Matrix
	values = other.values;
	numRows = other.numRows;
	numCols = other.numCols;
	// set the array or pointers to arrays to null pointer
	other.values = nullptr;
	other.numRows = 0; // put the old object into the move-from state
	other.numCols = 0;
	return *this;
}

// overloading operator +
Matrix Matrix::operator+(const Matrix& other) {
	// throw two matrix with not match size exception!!!
	if (numRows != other.numRows || numCols != other.numCols) {
		stringstream ss;
		ss << "Attempt to add two matrices with different size!" << endl;
		throw runtime_error(ss.str().c_str());
	}
	// construct a Matrix to store the summation of two matrix: this and other
	Matrix temp_matrix(numRows, numCols);
	// adding the matrices up for each element.
	for (int row = 0; row < numRows; ++row) {
    	for (int col = 0; col < numCols; ++col) {
			temp_matrix.values[row][col] = this->values[row][col]
									 + other.values[row][col];
		}
	}
	return temp_matrix;
}

// overloading operator +=
Matrix& Matrix::operator+=(const Matrix& other) {
	// throw two size of matrix not match exception!!!
	if (numRows != other.numRows || numCols != other.numCols) {
		stringstream ss;
		ss << "Attempt to add two matrices with different size!" << endl;
		throw runtime_error(ss.str().c_str());
	}
	// going over each token
	for (int row = 0; row < numRows; ++row) {
    	for (int col = 0; col < numCols; ++col) {
			this->values[row][col] += other.values[row][col];
		}
	}
	return *this;	
}

// default destructor
Matrix::~Matrix() {
  // First delete each row (every row is an array)
  for (int row = 0; row < numRows; ++row) {
    delete[] values[row];
  }
  // Then delete the array or pointers to arrays 
  delete[] values;
}

// get the value of each token in matrix
double Matrix::get(int row, int col) {
	if (row < 0 || col < 0 || numRows == 0 || numCols == 0) {
		stringstream ss;
		ss << "Attempt to access element with invalid row or column" << endl;
		throw runtime_error(ss.str().c_str());
	}
	return values[row][col];
}

// get the number of rows for matrix
int Matrix::getNumRows() {
	if (numRows < 0) {
		stringstream ss;
		ss << "Attempt to access element with invalid row" << endl;
		throw runtime_error(ss.str().c_str());
	}
	return numRows;
}

// get the number of cols for matrix
int Matrix::getNumCols() {
	if (numCols < 0) {
		stringstream ss;
		ss << "Attempt to access element with invalid column" << endl;
		throw runtime_error(ss.str().c_str());
	}
	return numCols;
}

// set give token of matrix given value
void Matrix::set(int row, int col, double value) {
	if (row < 0 || col < 0 || numRows == 0 || numCols == 0) {
		stringstream ss;
		ss << "Attempt to access element with invalid row or column" << endl;
		throw runtime_error(ss.str().c_str());
	}
	values[row][col] = value;
}

// print out the information of matrix
void Matrix::print() {
  for (int row = 0; row < numRows; ++row) {
    for (int col = 0; col < numCols; ++col) {
      cout << values[row][col] << " ";
    }
    cout << endl;
  }
}