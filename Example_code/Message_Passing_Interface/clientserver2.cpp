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
#include <unistd.h>

using namespace std;

void createRequest(char *buffer, int *size)
{
	// client creates a request and stores it in the buffer
	*size = 40;
}

void handleRequest(const char *buffer, int size)
{
	// server reads request from buffer and handles it
	sleep(1);
}

int main(int argc, char* argv[]) 
{
	const int MAX_SIZE=40;

        int myRank, nProc;
        MPI_Init( &argc, &argv );
        MPI_Comm_rank( MPI_COMM_WORLD, &myRank );
        MPI_Comm_size( MPI_COMM_WORLD, &nProc );

	// check that we have at least two processes
	if (nProc < 2) {
		cerr << "Run this example using at least two nodes" << endl;
		MPI_Finalize();
		return EXIT_FAILURE;
	}

	if (myRank != 0) {	// execute client clode
		char *sendBuf = new char[MAX_SIZE];
		int sendSize;

		while (true) {
			createRequest(sendBuf, &sendSize);
			MPI_Send(sendBuf, sendSize, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
		}
	} else {
		cout << "This program will run forever!" << endl;

		MPI_Request *req = new MPI_Request[nProc - 1];
		char **buffer = new char*[nProc - 1];

		for (size_t i = 0; i < nProc-1; i++)
			buffer[i] = new char[MAX_SIZE];

		for (size_t i = 0; i < nProc-1; i++)
			MPI_Irecv(buffer[i], MAX_SIZE, MPI_CHAR, i+1, 0, MPI_COMM_WORLD, &req[i]);

		while(true) {
			MPI_Status status;
			int reqIndex, recvSize;
	
			MPI_Waitany(nProc-1, req, &reqIndex, &status);
			MPI_Get_count(&status, MPI_CHAR, &recvSize);
			int srcRank = status.MPI_SOURCE;

			cout << "Handler request from process " << srcRank << " of size: " << recvSize << endl;
			handleRequest(buffer[reqIndex], recvSize);

			MPI_Irecv(buffer[reqIndex], MAX_SIZE, MPI_CHAR, srcRank, 0, MPI_COMM_WORLD, &req[reqIndex]);
		}

	}

        MPI_Finalize();
        return EXIT_SUCCESS;
}