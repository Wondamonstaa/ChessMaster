/*
 Name: Kiryl Baravikou
 UIN: 656339218
 Course: CS 341
 Professor: Jon Solworth
 */


#include "chessboard.h"

// define containers for color mappings (2 colors) and piece mappings (6 pieces)

/*Returns and instance of ChessboardIterator => this is the begin iterator*/
ChessboardIterator Chessboard::begin() {
    //Points to the current object
    return ChessboardIterator(*this, 0);
}

/*Returns and instance of ChessboardIterator => this is the begin iterator for mirrowing the board*/
ChessboardIterator Chessboard::begin_copy() {
    //Points to the current object
    return ChessboardIterator(*this, -1);
}

/*Returns and instance of ChessboardIterator => this is the end iterator*/
ChessboardIterator Chessboard::end() {
    //Above 64 => out of bounds => chessboard size
    return ChessboardIterator(*this, 64);
}

/*Returns and instance of ChessboardIterator => this is the end iterator*/
ChessboardIterator Chessboard::end_copy() {
    //Above 64 => out of bounds => chessboard size
    return ChessboardIterator(*this, 63);
}

//The following function prints out the board
void Chessboard::print() {
  
  int i = 0;
  
  // Loop through rows in reverse order to print with white at the bottom
  for(ChessboardIterator I = end_copy(); I != begin_copy(); --I){

    //Initialize the square object and assign the ptr to the iterator
    Square sq = *I;
    //Keep incrementing the induction variable and print the board
    i++;
    sq.print();

    //If the current cell is a multiple of 8, then the newline is printed
    cout << ((i % 8 == 0 && i < 63) ? "\n" : "");
  }
    
  cout << endl;
}
