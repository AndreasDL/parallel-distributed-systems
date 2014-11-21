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
        size_t N = 1000000;
	
        double pi = 0.0;
        double w = 1.0 / double(N);
        double sum = 0.0;
	
        #pragma omp parallel firstprivate(sum)
        {
                #pragma omp for
	        for (size_t i = 0; i < N; i++) {
			double x = w*(i - 0.5);
			sum += 4.0 / (1.0 + x*x);
	        }
                
                #pragma omp critical
                        pi += w*sum;
	}
	
	cout << pi << endl;

        return EXIT_SUCCESS;
}
