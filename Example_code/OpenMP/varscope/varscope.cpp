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

#include <omp.h>
#include <iostream>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) 
{
	cout << "Hello world from master thread!" << endl;

	int N = 1000;
	double *a = new double[N];
        double *b = new double[N];
        double *c = new double[N];

	// initialize c and b
	for (int i = 0; i < N; i++)
		b[i] = c[i] = i;

        #pragma omp parallel
        {
                #pragma omp master
                       cout << "Number of threads: " << omp_get_num_threads() << endl;        

		// wait until master thread has printed message
                #pragma omp barrier

                // all threads can see the same (initialized) variables a, b, c;
                // each thread gets a local copy of variables below
                int tid = omp_get_thread_num();
                int start = tid * N / omp_get_num_threads();
                int end = (tid + 1) * N / omp_get_num_threads();

                for(int i = start; i < end; i++)
                        a[i] = b[i] + c[i];
	}


	for (int i = 0; i < N; i++)
		cout << a[i] << " ";
	cout << endl;

        cout << "Pointer address of a: " << a << endl;
        #pragma omp parallel private(a, b, c)
        {
                // at this point a, b, c are local, uninitialized variables
                #pragma omp critical
                {
                        cout << "Local variable a (uninitialized): " << a << endl;
                }
        }

        delete [] a;
        delete [] b;
        delete [] c;

        return EXIT_SUCCESS;
}
