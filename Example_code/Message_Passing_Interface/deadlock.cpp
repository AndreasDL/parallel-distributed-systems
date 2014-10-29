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
        int rank, size;

        MPI_Init( &argc, &argv );
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );

	if (size < 2) {
		cerr << "Run this program with at least two processes..." << endl;
		MPI_Finalize();
		exit(EXIT_FAILURE);
	}

	MPI_Status status;

	int bufSize = 100000;
	double *a = new double[bufSize];
	double *b = new double[bufSize];

	if (rank == 0) {
		cout << "Warning: this program will likely never terminate..." << endl;
		MPI_Send(a, bufSize, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
		MPI_Send(b, bufSize, MPI_DOUBLE, 1, 2, MPI_COMM_WORLD);
	} else if (rank == 1) {
		MPI_Recv(a, bufSize, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, &status);
		MPI_Recv(b, bufSize, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
	}

	delete [] a;
	delete [] b;

        MPI_Finalize();
        return EXIT_SUCCESS;
}
