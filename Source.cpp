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
	Table data;
    try {
        data.read( argv[1] );
    }
    catch ( const string &error ) {
        cout << error << endl;
    }

    bool cont = true;

    while ( cont ) {
        cont = prompt();
        if ( cont ) {
            data.search( get_string() );
            data.draw( cout );
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