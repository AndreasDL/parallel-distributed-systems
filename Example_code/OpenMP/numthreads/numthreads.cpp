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
	// this section will be executed by the master thread only
        cout << "Number of threads in serial part: " << omp_get_num_threads() << endl;
	cout << "Hello world from master thread!" << endl;

        cout << "\n ----------- \n" << endl;

	// this section will be executed by all threads 
        // (number of threads = specified by OMP_NUM_THREADS flag)
        #pragma omp parallel
        {
	        cout << "Hello world from thread " << omp_get_thread_num() 
                     << "/" << omp_get_num_threads() << endl;
	}

	cout << "\n ----------- \n" << endl;

	// hard-code the number of threads to three
        omp_set_num_threads(3);

        #pragma omp parallel
        {
	        cout << "Hello world from thread " << omp_get_thread_num() 
                     << "/" << omp_get_num_threads() << endl;
	}

        cout << "\n ----------- \n" << endl;

        #pragma omp parallel num_threads(2)
        {
	        cout << "Hello world from thread " << omp_get_thread_num() 
                     << "/" << omp_get_num_threads() << endl;
	}	

        return EXIT_SUCCESS;
}
