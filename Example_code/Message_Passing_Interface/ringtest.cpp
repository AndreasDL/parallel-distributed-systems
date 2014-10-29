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

void sendRing(char *buffer, int msgLen)
{
        int myRank, numProc;
        MPI_Comm_rank( MPI_COMM_WORLD, &myRank );
        MPI_Comm_size( MPI_COMM_WORLD, &numProc );
        MPI_Status status;

        int prevRank = (myRank - 1 + numProc) % numProc;
        int nextRank = (myRank + 1) % numProc;

        if (myRank == 0) {      // send first, then receive
                MPI_Send( buffer, msgLen, MPI_CHAR, nextRank, 0, MPI_COMM_WORLD);
                MPI_Recv( buffer, msgLen, MPI_CHAR, prevRank, 0, MPI_COMM_WORLD, &status );
        } else {                // receive first, then send
                MPI_Recv( buffer, msgLen, MPI_CHAR, prevRank, 0, MPI_COMM_WORLD, &status );
                MPI_Send( buffer, msgLen, MPI_CHAR, nextRank, 0, MPI_COMM_WORLD);
        }
}

int main(int argc, char* argv[]) 
{
        int rank, size;
        MPI_Init( &argc, &argv );
        MPI_Comm_rank( MPI_COMM_WORLD, &rank );
        MPI_Comm_size( MPI_COMM_WORLD, &size );

        char *buffer = new char[64*1048576];

        int msgLen = 8;
        for (int i = 0; i < 24; i++) {
                double startTime = MPI_Wtime();
                sendRing(buffer, msgLen);
                double elapsed = MPI_Wtime() - startTime;

                if (rank == 0)
                	cout << "Bandwidth for size " << msgLen << ": " << (double)msgLen * size / elapsed << " bytes per second." << endl;
                msgLen *= 2;
        }

        delete [] buffer;

        MPI_Finalize();
        return EXIT_SUCCESS;
}
