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

#include <mpi.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) 
{
        int rank, size, count;
        MPI_Init( &argc, &argv );
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );

	size_t vecSize = 100;

	// initialize local portion of the vectors a and b
	double *a = new double[vecSize];
	for (size_t i = 0; i < vecSize; i++)
		a[i] = rank * i;

        double *b = new double[vecSize];
        for (size_t i = 0; i < vecSize; i++)
                b[i] = i;

	// calculate local dot product
	double partDot = 0.0;
	for (size_t i = 0; i < vecSize; i++)
		partDot += a[i] * b[i];

	double dot;
	MPI_Allreduce(&partDot, &dot, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

	cout << "I am process " << rank << ", dot product is: " << dot << endl;

	delete [] a;
	delete [] b;

        MPI_Finalize();
        return EXIT_SUCCESS;
}
