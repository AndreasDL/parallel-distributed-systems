//Code modifier by Andreas De Lille

/***************************************************************************
 *   Copyright (C) 2012-2013 Jan Fostier (jan.fostier@intec.ugent.be)      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <cstring>
#include <cmath>

using namespace std;

// ==========================================================================
// FUNCTION PROTOTYPE
// ==========================================================================

// double generalized matrix-matrix product C = alpha * A * B + beta * C (BLAS)
extern "C" void dgemm_(const char *transA,   // 'N' for normal product, 'T' for transpose of A
		       const char *transB,   // 'N' for normal product, 'T' for transpose of A
		       const int *m,         // number of rows of A and C
		       const int *n,         // number of columns of A and B
		       const int *k,         // number of colums of A, number of rows of B
		       const double *alpha,  // scalar value
		       const double *A,      // pointer to the first element of A
		       const int *LDA,       // leading dimension of A
		       const double *B,      // pointer to the first element of B
		       const int *LDB,       // leading dimension of B
		       const double *beta,   // scalar value
		       double *C,            // pointer to the first element of C
		       const int *LDC);      // leading dimension of C

// ==========================================================================
// TIMING ROUTINES
// ==========================================================================

double prevTime;

void startChrono() 
{
	prevTime = double(clock()) / CLOCKS_PER_SEC;
}

double stopChrono()
{
	double currTime = double(clock()) / CLOCKS_PER_SEC;
	return currTime - prevTime;
}

// ==========================================================================
// MATRIX-VECTOR PRODUCT IMPLEMENTATIONS
// ==========================================================================

size_t min(size_t a, size_t b) {
	return a < b ? a : b;
}

/**
 * Perform a matrix-matrix multiplication C = A*B using a strided memory access implementation
 * @param C Pointer to the first element of the m by n matrix C, stored in column major format (output)
 * @param A Pointer to the first element of the m by p matrix A, stored in column major format
 * @param B Pointer to the first element of the p by n matrix B, stored in column major format
 * @param m Number of rows of C and A
 * @param n Number of columns of C and B
 * @param p Number of columns of A, number of rows of B
 */
void matmatStrided(double *C, const double *A, const double *B, int m, int n, int p)
{
	memset(C, 0, m * n * sizeof(double)); //fill bytes of C with zeros

	for (size_t i = 0; i < m; i++)
		for (size_t j = 0; j < n; j++) {
			double temp = 0;
			for (size_t k = 0; k < p; k++)
				temp += A[k*m + i] * B[j*p + k];
			C[j*m + i] = temp;
	}
}

/**
 * Perform a matrix-matrix multiplication C = A*B using a linear memory access implementation
 * @param C Pointer to the first element of the m by n matrix C, stored in column major format (output)
 * @param A Pointer to the first element of the m by p matrix A, stored in column major format
 * @param B Pointer to the first element of the p by n matrix B, stored in column major format
 * @param m Number of rows of C and A   
 * @param n Number of columns of C and B
 * @param p Number of columns of A, number of rows of B
 */
void matmatLinear(double *C, const double *A, const double *B, int m, int n, int p)
{
	memset(C, 0, m * n * sizeof(double)); //fill bytes of C with zeros

	for (size_t i = 0; i < m; i++)
		for (size_t j = 0; j < n; j++) {
			double temp = 0;
			for (size_t k = 0; k < p; k++)
				temp += A[i*p + k] * B[j*n + k];
			C[i*n + j] = temp;
	}
	//the +<var> part should be the most inner loop for the void to be linear
	//doing this will make sure that the loop jumps just 1 step in the inner loop instead of n/m/p steps
	//Then change row size to column size
}

/**
 * Perform a matrix-matrix multiplication C = A*B using the BLAS
 * @param C Pointer to the first element of the m by n matrix C, stored in column major format (output)
 * @param A Pointer to the first element of the m by p matrix A, stored in column major format
 * @param B Pointer to the first element of the p by n matrix B, stored in column major format
 * @param m Number of rows of C and A
 * @param n Number of columns of C and B
 * @param p Number of columns of A, number of rows of B
 */
void matmatBLAS(double *C, const double *A, const double *B, int m, int n, int p)
{
	const char transA = 'N', transB = 'N';
	const double alpha = 1.0, beta = 0.0;

	dgemm_(&transA, &transB, &m, &n, &p, &alpha, A, &m, B, &p, &beta, C, &m);
}

/**
 * Perform a matrix-matrix multiplication C = A*B using a blocked implementation (temporal locality)
 * @param C Pointer to the first element of the m by n matrix C, stored in column major format (output)
 * @param A Pointer to the first element of the m by p matrix A, stored in column major format
 * @param B Pointer to the first element of the p by n matrix B, stored in column major format
 * @param m Number of rows of C and A
 * @param n Number of columns of C and B
 * @param p Number of columns of A, number of rows of B
 */
void matmatBlocked(double *C, const double *A, const double *B, int m, int n, int p)
{
	const int b = 64; //blocksize

	memset(C, 0, m * n * sizeof(double)); //fill bytes of C with zeros
	
	//do in blocks of size = b
	for (size_t bj = 0; bj < m; bj += b){
		for (size_t bk = 0; bk < p; bk += b){
			for (size_t bi = 0; bi < n; bi += b){

				//last rows & columns
				size_t j_stop = bj + b;
				size_t k_stop = bk + b;
				size_t i_stop = bi + b;
				if (j_stop > m)
					j_stop = m;
				if (k_stop > p)
					k_stop = p;
				if (i_stop > n)
					i_stop = n;

				//calculate the block
				for (size_t j = bj; j < j_stop; j++){
					for (size_t k = bk; k < k_stop; k++){
						for (size_t i = bi; i < i_stop; i++){
							C[j*m + i] = C[j*m + i] + A[k*m + i] * B[j*p + k];
						}
					}
				}


			}
		}
	}
}


int main(int argc, char* argv[])
{
	const size_t numExp = 1;
	const size_t m = 2000, n = 2000, p = 2000;

	double *C = new double[m*n];
	double *A = new double[m*p];
	double *B = new double[p*n];

	// fill in the elements of A
	for (size_t i = 0; i < m*p; i++)
		A[i] = i;

	// fill in the elements of B
	for (size_t i = 0; i < p*n; i++)
		B[i] = i;

	double flops = 2 * numExp * m * n * p;

	startChrono();
	for (size_t i = 0; i < numExp; i++)
		matmatStrided(C, A, B, m, n, p);
	cout << "FLOPS/s for a non-contiguous access matrix-matrix multiplication: " << flops / stopChrono() << endl;

	startChrono();
	for (size_t i = 0; i < numExp; i++)
		matmatLinear(C, A, B, m, n, p);
	cout << "FLOPS/s for a contiguous access matrix-matrix multiplication: " << flops / stopChrono() << endl;

	startChrono();
	for (size_t i = 0; i < numExp; i++)
		matmatBLAS(C, A, B, m, n, p);
	cout << "FLOPS/s for a BLAS matrix-matrix multiplication: " << flops / stopChrono() << endl;

	startChrono();
	for (size_t i = 0; i < numExp; i++)
		matmatBlocked(C, A, B, m, n, p);
	cout << "FLOPS/s for a blocked matrix-matrix multiplication: " << flops / stopChrono() << endl;

	delete [] A;
	delete [] B;
	delete [] C;

	exit(EXIT_SUCCESS);
}
