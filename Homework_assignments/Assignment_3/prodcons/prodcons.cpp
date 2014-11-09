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

#include <cstdlib> 
#include <iostream> 
#include <ctime> 
#include <cstring> 
#include <cmath> 
#include <pthread.h> 
#include <vector>

using namespace std;
vector<int> stack;

// ==========================================================================
// THREAD ROUTINES
// ==========================================================================

int bake()
{
        cout << "Baked donut...\n";
	return 0;
}

void* producer(void *vargs)
{
	while (true) {
		int element = bake();

		// if (stack.size() == 10)
		//      thread goes to sleep

		stack.push_back(element); // push "created" element onto the stack

		// if (stack.size() == 1)
		//      wake consumer thread
	}

	return NULL;
}

void eat(int donut)
{
	cout << "Ate donut...\n";
}

void* consumer(void *vargs)
{
	while (true) {
                // if (stack.empty())
		//     thread goes to sleep

		int donut = stack.back(); // get element
		stack.pop_back(); // remove element

		// if (stack.size() == 9)
		//     wake producer thread

		eat(donut);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	cout << "Warning: this code runs forever..." << endl;

	pthread_t prod, cons;

	// create threads
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);

	// this point will never be reached ...
	pthread_join(prod, NULL);
	pthread_join(cons, NULL);

	exit(EXIT_SUCCESS);
}
