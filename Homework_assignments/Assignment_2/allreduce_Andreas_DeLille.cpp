/***************************************************************************
 *   Name Andreas De Lille
 ***************************************************************************/

#include "mpi.h"
#include <cstdlib>
#include <iostream>

using namespace std;

/**
 * Wrapper function around MPI_Allreduce (leave this unchanged)
 * @param sendbuf Send buffer containing count doubles (input)
 * @param recvbuf Pre-allocated receive buffer (output)
 * @param count Number of elements in the send and receive buffers
 */
void allreduce(double *sendbuf, double *recvbuf, int count){
	MPI_Allreduce(sendbuf, recvbuf, count, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
}

/**
 * Wrapper function around MPI_Allreduce (implement reduce-scatter / allgather algorithm)
 * @param sendbuf Send buffer containing count doubles (input)
 * @param recvbuf Pre-allocated receive buffer (output)
 * @param count Number of elements in the send and receive buffers
 */
void allreduceRSAG(double *sendbuf, double *recvbuf, int count){
	// Note: assume MPI_DOUBLE, MPI_SUM and MPI_COMM_WORLD parameters
	// These may be hard-coded in the implementation. You may assume
	// that the number of processes is a power of two and you may assume
	// that "count" is a multiple of P.

	// Implement the allreduce algorithm using only point-to-point
	// communications, using the reduce-scatter / allgather algorithm
	// (see exercises slides for communication scheme)

	// Do not change the function signature as your implementation
	// will be automatically validated using a test framework.

	//hard coded shizzle
	MPI_Datatype type = MPI_DOUBLE;
	MPI_Comm comm = MPI_COMM_WORLD;
	MPI_Status status; //status needed for the receive
	int rank; MPI_Comm_rank(MPI_COMM_WORLD,&rank); //Rank of process
	int size; MPI_Comm_size(MPI_COMM_WORLD,&size); //size of process pool
	
	int start_send = 0; int stop_send = count;
	int start_recv = 0; int stop_recv = count;
	int items = count; //items that have to be send.
	int buddy; // the rank of the process that is our destination.

	//calculate
	int step = 2;
	while (step <= size){
		items /= 2;
		if (rank%step < step/2){
			//Send first half down
			
			buddy = rank + step/2;
			stop_send -= items;
			start_recv += items;
			//send
			MPI_Send(&sendbuf[start_send],items,type,buddy,123,comm);
			//recv
			MPI_Recv(&recvbuf[start_recv],items,type,buddy,123,comm,&status);
		}else{
			//send second half up
			start_send += items;
			stop_recv -= items;
			buddy = rank -step/2;
			//recv
			MPI_Recv(&recvbuf[start_recv],items,type,buddy,123,comm,&status);
			//send up
			MPI_Send(&sendbuf[start_send],items,type,buddy,123,comm);
		}
		//cout << "[step " << step << "] process " << rank << " stuurt " << items << "dingen (" << start_send << "->" << stop_send << ") naar " << buddy << " en heeft nu items: " << start_recv << " -> " << stop_recv << endl;
		
		//calculate the sum
		for (int i = start_recv; i < stop_recv; i++)
			sendbuf[i] += recvbuf[i];

		start_send = start_recv;
		stop_send = stop_recv;
		step *= 2;
	}

	//value is in send buffer & output needs to be in receive buffer, hence we got to move it
	//(swapping points won't work since these pointers are passed by value)
	for (int i = start_recv; i < stop_recv; i++)
		recvbuf[i] = sendbuf[i];
 
	//!! stop values are no longer valid below this line

	//gather
	step /=2;
	while (step > 1){
		
		if (rank%step < step/2){
			//Send all down
			//what comes in must go first
			buddy = rank + step/2;
			start_recv -= items;
			
			//send
			MPI_Send(&recvbuf[start_send],items,type,buddy,1,comm);
			//recv
			MPI_Recv(&recvbuf[start_recv],items,type,buddy,1,comm,&status);
		}else{
			//send all up
			//what comes is must go last
			buddy = rank - step/2;

			//recv
			MPI_Recv(&recvbuf[start_recv+items],items,type,buddy,1,comm,&status);
			//send up
			MPI_Send(&recvbuf[start_send],items,type,buddy,1,comm);
		}

		step /= 2;
		items *= 2;
		start_send = start_recv;
	}

	return;
}

/**
 * Program entry
 */
int main(int argc, char* argv[]){
	int thisProc, nProc;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &thisProc);
	MPI_Comm_size(MPI_COMM_WORLD, &nProc);
	
	// optionally: write test code
	// (this is not required as we will only test the 
	// allreduceRSAG implementation itself.
	
	//create send and receive buffer
	const int size = 8;//1024*1024;
	double *send = new double[size];
	double *recv = new double[size];
	//dummy values
	for (int i = 0 ; i < size; i++){
		send[i] = i;
		recv[i] = -1; //debug to see whether the buffer is used or not
	}
    //cout << "proces " << thisProc << " all reduce" << endl;

	allreduceRSAG(send,recv,size);
	//allreduce(send,recv,size);

	if (thisProc == 1){
		for (int i = 0; i < size; i++)
			cout << "rank: " << thisProc << " i: " << i << " = " << recv[i] << endl;
	}
//*/
	delete[] send;
	delete[] recv;

	MPI_Finalize();
	exit(EXIT_SUCCESS);
}
