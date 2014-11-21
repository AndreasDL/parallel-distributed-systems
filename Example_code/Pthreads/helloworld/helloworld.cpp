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

using namespace std;

void* helloWorld(void *)
{
        cout << "Hello World!\n";
}

int main(int argc, char* argv[]) 
{
        pthread_t thread1, thread2;

        pthread_create(&thread1, NULL, &helloWorld, NULL);
        pthread_create(&thread2, NULL, &helloWorld, NULL);

        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        return EXIT_SUCCESS;
}
