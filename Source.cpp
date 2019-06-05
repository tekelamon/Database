/*
	File: Source.cpp (main.cpp)
	Author: Teke Byas
	Email: tekeb6687@student.vvc.edu
	Description: A tabular version of the sorting and searching project done in CIS 202
		This version will be done with one object containing a table rather than two objects
*/

#include <iostream>

#include "Table.h"

using namespace std;

bool prompt();
string get_string();

int main(int argc, char *argv[]) {
	Table t;
    try {
        if ( argc == 1 )
            throw "No input file given.";
        if ( argc < 2 )
            throw "Too many arguments given.";
        t.read( argv[1] );
    }
    catch ( const string &error ) {
        cout << error << endl;
        return -1;
    }

    bool cont = true;

    while ( cont ) {
        cont = prompt();
        if ( cont ) {
            t.search( get_string() );
            t.draw( cout );
        }
    }

	return 0;
}

bool prompt() {
    cout << "Would you like to search? (Y/N) ";
    string ans;
    cin >> ans;
    if ( ans[0] == 'y' || ans[0] == 'Y' ) {
        return true;
    }
    else {
        return false;
    }
}

string get_string() {
	cout << "Enter a search term: ";
	string find;
	cin >> find;
	return find;
}