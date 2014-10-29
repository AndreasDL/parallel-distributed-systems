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
	char buffer[40];
	MPI_Status status;

        MPI_Init( &argc, &argv );
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );

	if (rank != 0) {
		sprintf(buffer, "Hello World");
		MPI_Send(buffer, 12, MPI_CHAR, 0, 123, MPI_COMM_WORLD);
	} else {
		for (int i = 1; i < size; i++) {
	                MPI_Recv(buffer, 40, MPI_CHAR, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_CHAR, &count);
			cout << "Process " << rank << " received "  << buffer << " from process " 
                             << status.MPI_SOURCE << " with size " << count << " and tag " << status.MPI_TAG << endl;
		}
		cout << "Process " << rank << " had received all messages.  Exiting..." << endl;
	}

        MPI_Finalize();
        return EXIT_SUCCESS;
}
