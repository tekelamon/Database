/*
	File: Database.cpp
	Author: Teke Byas
	Email: tekeb6687@student.vvc.edu
	Description: This object will store, sort, search, and print the data it holds
		formatted to look like a table
*/

#include <fstream>
#include <cassert>
#include <iomanip>

#include "Table.h"

using namespace std;

// -----------------------------------------------------------------

// Setup Functions

Table::Table() {
	// with the read function implemented,
	// blank Table objects can be created and used
	// unlike with the other project done for class
    prep_found();
}

Table::Table( string **table ) {
	data = table;
    prep_found();
}

Table::Table( const string &file ) {
    read( file );
    prep_found();
}

Table::~Table() {
    for ( int i = 0; i < size; i++ ) {
        delete[] data[i];
    }
    delete[] data;

    for ( int i = 0; i < MAX_SPREAD; i++ ) {
        found[i] = nullptr;
        delete[] found[i];
    }
    delete[] found;
}

void Table::prep_found() {
    amount_found = 0;
    found = new string*[MAX_SPREAD];
    
    for ( int i = 0; i < MAX_SPREAD; i++ ) {
        found[i] = new string[fields];
    }
}

void Table::reset_found() {
    for ( int i = 0; i < MAX_SPREAD; i++ ) {
        found[i] = nullptr;
        delete[] found[i];
    }
    delete[] found;
    prep_found();
}

// -----------------------------------------------------------------

// Input Functions

void Table::read(const string &file) {
	ifstream input( file );

    assert( input.is_open() );

	string buffer;

    int line_count = 0;

    // determines how many rows are needed
    while ( !input.eof() ) {
        getline( input, buffer );
        line_count++;
    }

    // resets the state of 'input' to be read again
    // after the line count of the file is determined
    input.close();
    input.open(file);
    assert( input.is_open() );

    // determines how many columns are needed
    getline( input, buffer );
    for ( int i = 0; i < buffer.size(); i++ )
        if ( buffer[i] == ',' )
            fields++;

    // updates the usable size of 'data'
    size = line_count - 1;    // - 1 because the first line is a header
    data = new string*[size]; // and is only needed for reading the table

    // creates an empty slot for each needed line, then fills it
    for ( int i = 0; i < size; i++ ) {
        data[i] = new string[fields];

        getline( input, buffer );
        data[i] = split( buffer, ',' );
    }

}

string* Table::split(const string &line, char delim) {
    string *person = new string[fields];

	int count = 0;
	string temp = "";

	for (int i = 0; i < line.size(); i++) {
		if (line[i] == delim) {
			person[count] = temp;
            count++;
			temp = "";
		}
		else
			temp += line[i];
	}

    if ( !temp.empty() ) {
        person[count] = temp;
    }

	return person;
}

// -----------------------------------------------------------------

// Sorting Functions

void Table::sort( int pos ) {
    // a work array for storing temporary data
    string **work = new string*[size];
    for ( int i = 0; i < size; i++ )
        work[i] = new string[fields];

    // makes work a copy of data
    copy_array( data, 0, size - 1, work );

    // sorts work into data
    merge_sort( work, 0, size - 1, pos, data );

    // delete the space needed by the work array
    for ( int i = 0; i < size; i++ ) {
        work[i] = nullptr;
        delete[] work[i];
    }
    delete[] work;
}

void Table::swap(string *first, string *second) {
	string *temp = first;
	first = second;
	second = temp;
}

void Table::merge_sort( string **data, int low, int high, int pos, string **work ) {
    if ( high - low < 2 )
        return;
    else {
        int mid = ( high + low ) / 2;
        merge_sort( work, low, mid, pos, data );
        merge_sort( work, mid, high, pos, data );
        merge( data, low, high, pos, work );
    }
}

void Table::merge( string **work, int low, int high, int pos, string **data ) {
    int mid = ( high + low ) / 2;
    int i = low;
    int j = mid;

    // walks up both halves of the sorted array (each half was sorted independently)
    // and zips the two arrays together in sorted order
    for ( int k = low; k < high; k++ ) {
        if ( i < mid && ( work[i][pos] < work[j][pos] || j >= high ) ) {
            data[k] = work[i];
            i++;
        }
        else {
            data[k] = work[j];
            j++;
        }
    }
}

void Table::copy_array( string **from, int low, int high, string ** to ) {
    for ( int i = low; i < high; i++ )
        to[i] = from[i];
}

// -----------------------------------------------------------------

// Searching Functions

void Table::search(const string &find) {
    if ( found[0][0] != "" )
        reset_found();

    for ( int pos = 0; pos < fields; pos++ ) {
        Table::sort( pos );
        bsearch( find, 0, size - 1, pos );
    }
}

void Table::bsearch( const string &find, int low, int high, int pos ) {
    int mid = ( high + low ) / 2;

    if ( mid > high || mid < low )
        return;

    if ( partial_match_found( data[mid][pos], find ) )
        scan_near( find, low, high, pos );
	else if (data[mid][pos] > find)
		bsearch(find, low, mid - 1, pos);
	else if (data[mid][pos] < find)
		bsearch(find, mid + 1, high, pos);
}

bool Table::partial_match_found( const string &person, const string &find ) {
    for ( int i = 0; i < find.size(); i++ )
        if ( find[i] != person[i] )
            return false;
    return true;
}

void Table::scan_near( const string &find, int low, int high, int pos ) {
    if ( ( high - low ) > 20 ) {
        int mid = ( high + low ) / 2;
        for ( int i = mid - 10; i < mid + 10; i++ ) {
            if ( partial_match_found( data[i][pos], find ) ) {
                found[amount_found] = data[i];
                amount_found++;
            }
        }
    }
    else {
        for ( int i = low; i < high; i++ ) {
            if ( partial_match_found( data[i][pos], find ) ) {
                found[amount_found] = data[i];
                amount_found++;
            }
        }
    }
}

// -----------------------------------------------------------------

// Output Functions

ostream& Table::write( ostream &out ) {
    for ( int i = 0; i < amount_found; i++ ) {
        for ( int j = 0; j < fields; j++ )
            out << found[i][j] << ',';
        out << '\n';
    }
    return out;
}

void Table::draw( ostream& out ) {
    // initialize an array of integers to get the width needed for every field
    int *spacing = new int[fields];
    for ( int i = 0; i < fields; i++ )
        spacing[i] = 0;

    // determine the maximum space needed for every field
    for ( int i = 0; i < amount_found; i++ )
        for ( int j = 0; j < fields; j++ )
            if ( spacing[j] < found[i][j].size() )
                spacing[j] = found[i][j].size();

    draw_horizontal_bar( out, '*', '-', '*', spacing );

    // prints the stored data
    for ( int i = 0; i < amount_found; i++ ) {
        out << "| ";
        for ( int j = 0; j < fields; j++ ) {
            out << setw( spacing[j] );
            out << found[i][j] << '|';
        }
        out << '\n';

        if ( i < amount_found - 1 )
            draw_horizontal_bar( out, '|', '-', '|', spacing );
        else
            draw_horizontal_bar( out, '*','-', '*', spacing );
    }
    out << '\n';
    delete[] spacing;
}

void Table::draw_horizontal_bar( ostream &out, char left, char fill, char right, int spacing[] ) {
    out << left;
    for ( int i = 0; i < fields; i++ )
        for ( int j = 0; j <= spacing[i]; j++ )
            out << fill;
    out << right;
    out << '\n';
}
