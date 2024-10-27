//
// Created by Daniel X Feng
// Created Date: 29 Nov 2023.
//

#ifndef SRC_ALGEBRA_H
#define SRC_ALGEBRA_H

#include <iostream>
#include <vector>

using Matrix = std::vector<std::vector<double>>;

// Some useful tools of algebra
namespace algebra {
    // Return n*m matrix with all elements equal to zero.
    Matrix zeros(size_t n, size_t m);

    // Return n*m matrix with all elements equal to one.
    Matrix ones(size_t n, size_t m);

    // Return n*m matrix with all elements a random number between given min and max.
    Matrix random(size_t n, size_t m, double min, double max);

    // Display the matrix;
    void show(const Matrix& matrix);

    // Return a new matrix that multiplies the given matrix into the given constant scalar c
    Matrix multiply(const Matrix& matrix, double c);

    // Return a new matrix that multiplies the given matrix1 into given matrix2.
    Matrix multiply(const Matrix& matrix1, const Matrix& matrix2);

    // Return a new matrix that adds the constant number c to every element of given matrix.
    Matrix sum(const Matrix& matrix, double c);

    // Return a new matrix that adds 2 matrices to each other.
    Matrix sum(const Matrix& matrix1, const Matrix& matrix2);

    // Return a transpose matrix of the input matrix.
    Matrix transpose(const Matrix& matrix);

    // Return a new matrix of the minor of the input matrix with respect to nth row and mth column.
    Matrix minor(const Matrix& matrix, size_t n, size_t m);

    // Return the calculation of the determinant of the input matrix.
    double determinant(const Matrix& matrix);

    // Return the matrix's inverse.
    Matrix inverse(const Matrix& matrix);

    // Return a new matrix that will concatenate given matrix1 and matrix2 along the given axis.
    Matrix concatenate(const Matrix& matrix1, const Matrix& matrix2, int axis=0);

    // Return the swap matrix that swaps r1th row with r2th.
    Matrix ero_swap(const Matrix& matrix, size_t r1, size_t r2);

    // Return a new matrix that multiplies every element in rth row with constant number c.
    Matrix ero_multiply(const Matrix& matrix, size_t r, double c);

    // Return a new matrix that sum adds r1th x c into r2th row.
    Matrix ero_sum(const Matrix& matrix, size_t r1, double c, size_t r2);

    // Return a new matrix that calculates the upper triangular form of the matrix using the ERO operations.
    Matrix upper_triangular(const Matrix& matrix);
}

#endif //SRC_ALGEBRA_H
