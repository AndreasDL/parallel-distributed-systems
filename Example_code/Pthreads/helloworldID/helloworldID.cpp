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

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <cstdio>

using namespace std;

typedef struct {
        int threadID;
        int numThreads;
} inputArgs;

void* helloWorld(void *args)
{
	int threadID = ((inputArgs*)args)->threadID;
        int numThreads = ((inputArgs*)args)->numThreads;

        printf("Hello World from thread %d / %d\n", threadID, numThreads);
}

int main(int argc, char* argv[]) 
{
        int numThreads = 4;

        pthread_t *thread = new pthread_t[numThreads];
        inputArgs *arg = new inputArgs[numThreads];

        for (int i = 0; i < numThreads; i++) {
                arg[i].threadID = i;
                arg[i].numThreads = numThreads;
                pthread_create(&thread[i], NULL, &helloWorld, (void*)&arg[i]);
        }

        for (int i = 0; i < numThreads; i++)
                pthread_join(thread[i], NULL);

	delete [] thread;
        delete [] arg;

        return EXIT_SUCCESS;
}
