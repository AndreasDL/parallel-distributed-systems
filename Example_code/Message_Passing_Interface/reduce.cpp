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

	if (rank == 0) {
		cout << "Enter the number of random numbers to generate: ";
		cin >> count;
	}

	MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int myStart = int((double)rank * (double)count / (double)size);
	int nextStart = int((double)(rank + 1) * (double)count / (double)size);

	const double h = 1.0 / (double)count;
	double x = h * ((double)myStart + 0.5);
	double partSum = 0.0;

	for (int i = myStart; i < nextStart; i++) {
		partSum += 4.0 / (1.0 + x * x);
		x += h;
	}

	double sum;
	MPI_Reduce(&partSum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	sum *= h;

	cout.precision(16);
	if (rank == 0)
		cout << "Estimation for pi: " << sum << endl;

        MPI_Finalize();
        return EXIT_SUCCESS;
}
