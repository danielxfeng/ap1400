//
// Created by Daniel X Feng
// Created Date: 29 Nov 2023.
//

#include "random"
#include "iterator"
#include <cmath>
#include <chrono>
#include "algebra.h"

using std::cout;
using std::endl;
using std::vector;
using std::logic_error;
using std::ostream_iterator;

namespace algebra {
    Matrix zeros(size_t n, size_t m) {
        Matrix matrix(n, vector<double>(m));
        return matrix;
    }

    Matrix ones(size_t n, size_t m) {
        Matrix matrix(n, vector<double>(m, 1));
        return matrix;
    }

    Matrix random(size_t n, size_t m, double min, double max) {
        if (min > max) throw logic_error("min should be less than max");
        // Define a random engine.
        static std::default_random_engine e(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<double> u(min, max);
        Matrix matrix(n, vector<double>(m));
        for (auto& row : matrix) {
            for (auto& element : row) {
                element = u(e);  // Generate a random value for each element.
            }
        }
        return matrix;
    }

    void show(const Matrix& matrix) {
        for (const auto & i : matrix) {
            // std::copy to iterator to cout to print.
            copy(i.begin(), i.end(),
                 ostream_iterator<double>(cout, " "));
            cout << endl;
        }
    }

    Matrix multiply(const Matrix& matrix, double c) {
        // Check if the matrix is empty, and throw an exception if so.
        if (matrix.empty()) throw logic_error("The matrix should not be empty.");
        Matrix res;
        // Iterate by row.
        for (const auto & i : matrix) {
            vector<double> v;
            // Generate the elements of a row.
            for (double j : i) {
                v.push_back(j * c);
            }
            res.push_back(v);
        }
        return res;
    }

    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if either matrix is empty, and return an empty matrix if so.
        if (matrix1.empty() || matrix2.empty()) return {};
        // Check for compatible dimensions for matrix multiplication.
        if (matrix1[0].size() != matrix2.size())
            throw logic_error("The number of columns in the first matrix must equal "
                              "the number of rows in the second matrix.");
        // Define the dimensions of the result matrix.
        size_t rows = matrix1.size();
        size_t cols = matrix2[0].size();
        size_t inner = matrix1[0].size();
        // Create a result matrix filled with zeros
        Matrix res(rows, std::vector<double>(cols));
        // Perform matrix multiplication
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                for (size_t k = 0; k < inner; k++) {
                    // Accumulate the product of corresponding elements
                    res[i][j] += matrix1[i][k] * matrix2[k][j];
                }
            }
        }
        return res;
    }

    Matrix sum(const Matrix& matrix, double c) {
        // Check if the matrix is empty, return an empty matrix.
        if (matrix.empty()) return {};
        Matrix res;
        // Perform the sum of two matrix.
        for (const auto & i : matrix) {
            vector<double> v;
            // Add c to every element of a row.
            for (double j : i) {
                v.push_back(j + c);
            }
            res.push_back(v);
        }
        return res;
    }

    Matrix sum(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if both matrix is empty, return an empty matrix if so.
        if (matrix1.empty() && matrix2.empty()) return {};
        // Check if either matrix is empty, throw an error if so.
        if (matrix1.empty() || matrix2.empty())
            throw logic_error("There is at least an empty matrix");
        // Check if both matrices have same dimensions.
        if (matrix1.size() != matrix2.size() || matrix1[0].size() != matrix2[0].size())
            throw logic_error("Both matrices must have the same dimensions.");
        // Create a result matrix.
        Matrix res;
        // Perform the sum.
        for (size_t i = 0; i < matrix1.size(); i++) {
            vector<double> v;
            for (size_t j = 0; j < matrix1[i].size(); j++) {
                // Apply the sum by elements by 2 matrices.
                v.push_back(matrix1[i][j] + matrix2[i][j]);
            }
            res.push_back(v);
        }
        return res;
    }

    Matrix transpose(const Matrix& matrix) {
        // Check if the matrix is empty, return an empty matrix if true
        if (matrix.empty()) return {};
        // Determine the number of rows and columns for the transposed matrix
        size_t rows = matrix[0].size(); // Rows in the transposed matrix equal columns in the original
        size_t cols = matrix.size();    // Columns in the transposed matrix equal rows in the original
        // Create a new matrix with dimensions of the transposed matrix
        Matrix res{rows, vector<double>(cols)};
        // Iterate over the matrix to transpose it
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                // Assign elements to their transposed positions
                res[i][j] = matrix[j][i];
            }
        }
        return res;
    }

    Matrix minor(const Matrix& matrix, size_t n, size_t m) {
        // Check if the matrix is empty, return an empty matrix if true
        if (matrix.empty()) return {};
        // Check if n and m are within the bounds of the matrix dimensions
        if (m >= matrix.size() || n >= matrix[0].size())
            throw logic_error("Illegal parameter n or m");
        // Initialize an empty matrix for the result
        Matrix res;
        // Iterate over the rows of the matrix
        for (size_t i = 0; i < matrix.size(); i++) {
            // Skip the row n, as it's not included in the minor
            if (i == n) continue;
            vector<double> v; // A vector to store the row of the minor matrix
            // Iterate over the columns of the matrix
            for (size_t j = 0; j < matrix[i].size(); j++) {
                // Skip the column m, as it's not included in the minor
                if (j == m) continue;
                // Add the element to the row vector
                v.push_back(matrix[i][j]);
            }
            // Add the row vector to the minor matrix
            res.push_back(v);
        }
        return res;
    }

    double determinant(const Matrix& matrix) {
        // Check if the matrix is empty, return 1 as the determinant of an empty matrix
        if (matrix.empty()) return 1;
        // Base case: if the matrix is 1x1, return the single element
        if (matrix.size() == 1) return matrix[0][0];
        // Initialize the determinant result
        double res = 0;
        // Iterate over the elements of the first row
        for (size_t i = 0; i < matrix[0].size(); i++) {
            // Compute the minor of the matrix excluding the current row and column
            Matrix small_matrix = minor(matrix, 0, i);
            // Calculate the cofactor of the current element
            double cofactor = (determinant(small_matrix)) * (pow(-1, i));
            // Add the product of the element and its cofactor to the determinant
            res += matrix[0][i] * cofactor;
        }
        return res;
    }

    // The help function to calculate the adjoint of a given square matrix
    Matrix adjoint(const Matrix& matrix) {
        // Check if the matrix is empty or not square, throw an error if so
        if (matrix.empty() || matrix.size() != matrix[0].size())
            throw logic_error("Matrix must be non-empty and square.");
        // Get the size of the matrix
        size_t n = matrix.size();
        // Create an n x n matrix initialized with zeros, to hold the adjoint
        Matrix adj = zeros(n, n);
        // Iterate over all elements in the matrix
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < n; j++) {
                // Calculate the minor of the matrix excluding the current row (i) and column (j)
                Matrix min = minor(matrix, i, j);
                // Calculate the cofactor of the current element
                double cofactor = determinant(min) * (pow(-1, i + j));
                // Set the cofactor in the transposed position in the adjoint matrix
                adj[j][i] = cofactor;
            }
        }
        return adj;
    }

    Matrix inverse(const Matrix& matrix) {
        // Check if the matrix is empty, return an empty matrix if true
        if (matrix.empty()) return {};
        // Calculate the determinant of the matrix
        double det = determinant(matrix);
        // Check if the determinant is close to zero (matrix is non-invertible)
        if (abs(det) < 1e-9) throw logic_error("Matrix is not invertible.");
        // Calculate the adjoint of the matrix
        Matrix adj = adjoint(matrix);
        // Multiply the adjoint by 1/determinant to get the inverse
        // This is based on the formula: inverse(matrix) = adjoint(matrix) / determinant(matrix)
        Matrix inv = multiply(adj, 1.0 / det);
        return inv;
    }


    // A helper function to concatenate two matrices by rows
    Matrix concatenate_by_row(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if both matrices have the same number of rows
        if (matrix1.size() != matrix2.size())
            throw logic_error("The two matrices should have the same number of rows.");
        // Initialize an empty matrix to store the result
        Matrix res;
        // Iterate over the rows of the matrices
        for (int i = 0; i < matrix1.size(); i++) {
            vector<double> v; // Create a temporary vector to store the concatenated row
            // Insert all elements of the ith row of the first matrix into v
            v.insert(v.end(), matrix1[i].begin(), matrix1[i].end());
            // Insert all elements of the ith row of the second matrix into v
            v.insert(v.end(), matrix2[i].begin(), matrix2[i].end());
            // Add the concatenated row to the resulting matrix
            res.push_back(v);
        }
        return res;
    }

    // A helper function to concatenate two matrices by columns
    Matrix concatenate_by_column(const Matrix& matrix1, const Matrix& matrix2) {
        // Check if both matrices have the same number of columns
        if (matrix1[0].size() != matrix2[0].size())
            throw logic_error("The two matrix should have same row length.");
        // Initialize an empty matrix to store the result
        Matrix res;
        // Insert all elements of the ith cols of the first matrix into v
        res.insert(res.end(), matrix1.begin(), matrix1.end());
        // Insert all elements of the ith cols of the second matrix into v
        res.insert(res.end(), matrix2.begin(), matrix2.end());
        return res;
    }

    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis) {
        // If one matrix is empty, return another
        if (matrix1.empty()) return matrix2;
        if (matrix2.empty()) return matrix1;
        // If axis is 0, concatenate the matrices by column
        if (!axis) return concatenate_by_column(matrix1, matrix2);
            // If axis is 1, concatenate the matrices by row
        else if (axis == 1) return concatenate_by_row(matrix1, matrix2);
            // If axis is neither 0 nor 1, throw an error
        else throw logic_error("The axis should be 0 or 1.");
    }

    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2) {
        // Check if the row indices are within the bounds of the matrix
        size_t size = matrix.size();
        if (r1 >= size || r2 >= size) throw logic_error("The parameter r1 or r2 is out of range.");
        // Create a copy of the matrix
        Matrix res(matrix);
        // Swap the rows r1 and r2
        res[r1] = res[r2];
        res[r2] = matrix[r1];
        return res;
    }


    Matrix ero_multiply(const Matrix& matrix, size_t r, double c) {
        // Check if the row index is within the bounds of the matrix
        if (r >= matrix.size()) throw logic_error("The parameter r is out of range.");
        // Create a copy of the matrix
        Matrix res(matrix);
        // Multiply each element in row r by the constant c
        for (int i = 0; i < matrix[r].size(); i++) {
            res[r][i] = matrix[r][i] * c;
        }
        return res;
    }

    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2) {
        // Check if the row indices are within the bounds of the matrix
        size_t size = matrix.size();
        if (r1 >= size || r2 >= size) throw logic_error("The parameter r1 or r2 is out of range.");
        // Create a copy of the matrix
        Matrix res(matrix);
        // Add c times row r1 to row r2
        for (int i = 0; i < matrix[r1].size(); i++) {
            res[r2][i] = matrix[r1][i] * c + matrix[r2][i];
        }
        return res;
    }

    // A helper function to perform ero swap when a diagonal element is zero
    void ero_swap_when_zero_diagonal(Matrix& matrix, int i) {
        // Check if the current diagonal element is close to zero
        if (abs(matrix[i][i]) > 1e-9) return;
        // Iterate over the rows below the current row
        for (int j = i + 1; j < matrix.size(); j++) {
            // Check for a non-zero element in the same column
            if (abs(matrix[j][i]) > 1e-9) {
                // Perform ero_swap if a suitable row is found
                ero_swap(matrix, i, j);
                break;
            }
        }
    }

    Matrix upper_triangular(const Matrix& matrix) {
        // Check if the matrix is empty, return an empty matrix if so
        if (matrix.empty()) return {};
        // Check if the matrix is square
        if (matrix.size() != matrix[0].size()) throw logic_error("The matrix should be square.");
        // Copy the input matrix to work on
        Matrix res(matrix);
        // Iterate over the columns of the matrix
        for (int i = 0; i < res.size(); i++) {
            // Perform ero swap for zero diagonal elements
            ero_swap_when_zero_diagonal(res, i);
            // Perform row-wise elimination
            for (int j = i + 1; j < res.size(); j++) {
                // Skip if the element is close to zero
                if (abs(res[j][i]) <= 1e-9) continue;
                // Perform ero sum to zero out the elements below the diagonal
                res = ero_sum(res, i, -res[j][i] / res[i][i], j);
            }
        }
        return res;
    }
}
