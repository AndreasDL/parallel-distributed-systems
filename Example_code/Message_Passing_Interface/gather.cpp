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

	int root = 0;
	int sendBuf = rank;	// simply send your own rank to the root process

	if (rank == root) {
		int *recvBuf = new int[size];

		MPI_Gather(&sendBuf, 1, MPI_INT, recvBuf, 1, MPI_INT, root, MPI_COMM_WORLD);

		cout << "Receive buffer at root process: " << endl;
		for (size_t i = 0; i < size; i++)
			cout << recvBuf[i] << " ";
		cout << endl;

		delete [] recvBuf;
	} else {
		MPI_Gather(&sendBuf, 1, MPI_INT, NULL, 1, MPI_INT, root, MPI_COMM_WORLD);
	}

        MPI_Finalize();
        return EXIT_SUCCESS;
}
