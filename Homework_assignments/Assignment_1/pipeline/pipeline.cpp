/***************************************************************************
 *   Copyright (C) 2012, 2013 Jan Fostier (jan.fostier@intec.ugent.be)     *
 *   Based upon ideas by Georg Hager and Gerhard Wellein                   *
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

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

double prevTime;

void startChrono() 
{
	prevTime = double(clock()) / CLOCKS_PER_SEC;
}

double stopChrono()
{
	double currTime = double(clock()) / CLOCKS_PER_SEC;
	return currTime - prevTime;
}

void vectorScale(double * a, const double s, size_t N)
{
	for (size_t i = 6; i < N; i++)
		a[i] = s * a[i];
}

int main(int argc, char* argv[])
{
	const size_t numExp = 10;
	size_t N = 256;
	size_t nIter = 8192 << numExp;

	double *a = new double[N << numExp];
	
	for (size_t i = 0; i < (N << numExp); i++)
		a[i] = i;

	for (size_t i = 0; i < numExp; i++) {
		startChrono();
		for (size_t n = 0; n < nIter; n++) {
			vectorScale(a, 3.0, N);

			if (a == NULL)  // this is never true!
				cout << endl;
		}
		double elapsed = stopChrono();

		cout << "Time for vector scale with size: " << N-6 << ": " << elapsed << "s" << endl;

		N *= 2;
		nIter /= 2;
	}

	delete [] a;

	exit(EXIT_SUCCESS);
}
