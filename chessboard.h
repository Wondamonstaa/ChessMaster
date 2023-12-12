/*
 Name: Kiryl Baravikou
 UIN: 656339218
 Course: CS 341
 Professor: Jon Solworth
 */

#include <iostream>
#include <iterator>
using namespace std;

//Define separate containers for Color, Piece (enums recommended) => DONE
enum Color {White, Black, Free};
enum Piece {Rook, Knight, Bishop, Queen, King, Pawn, None};

//Below are the defined constants to specify the illegal moves => DONE
const int ERROR_POS_START_X = -1;
const int ERROR_POS_START_Y = -2;
const int ERROR_POS_END_X = -3;
const int ERROR_POS_END_Y = -3;
const int NOT_OCCUPIED = -5;
const int SAME_COLOR = -6;
const int ILLEGAL_MOVE = -7;
#define then
#define otherwise

//Square class: a chessboard contains squares => DONE
class Square {
private:
    Color color;
    Piece piece;
    bool occupied;
    
public:
    
    //Default constructor => DONE
    Square() : color(Free), piece(None), occupied(false) {}
    
    //Parameterized constructor => DONE
    Square(Color c, Piece p) : color(c), piece(p), occupied(true) {}
    
    //Allows to retreive a piece and its state => DONE
    int get(bool &o, Color &c, Piece &p) {
        o = occupied ? true : false;
        c = occupied ? color : Free;
        p = occupied ? piece : None;
        return occupied ? 1 : -3;
    }
    
    //Prints out the entire chessboard => DONE
    void print() {
        
        //Sanity check for square occupation
        if (occupied) {
            
            // Designates the color choice: either black or white
            char c = (color == White) ? 'w' : 'b';
            
            // Initialize p with a space character in case of an invalid piece
            char p = ' ';
            
            //Allows to convert a piece to a char
            auto mapper = [](Piece piece) -> char {
                switch (piece) {
                    case Rook:
                        return 'R';
                    case Knight:
                        return 'N';
                    case Bishop:
                        return 'B';
                    case Queen:
                        return 'Q';
                    case King:
                        return 'K';
                    case Pawn:
                        return 'P';
                    default:
                        return ' ';
                }
            };
            
            //Maps the correct color to a piece
            p = mapper(piece);
            cout << c << p << " ";
        }
        else {
            cout << " . ";
        }
    }
};

class ChessboardIterator;

class Chessboard{
    
    //Make the container iterator friendly => DONE
    typedef ChessboardIterator iterator;
    
private:
    Square chessboard[8][8];
    
public:
    friend class ChessboardIterator;
    bool occupied, start_state, end_state;
    Color curColor, thisColor, newColor;
    Piece curPiece, thisPiece, newPiece;
    
    //Initializes the squares => DONE
    Chessboard() {
        //Standard way to fill out 8 by 8 box
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                chessboard[x][y] = Square();
            }
        }
    }
    
    //Returns an x,y square from chessboard => DONE
    Square square(int x, int y) {
        return chessboard[x][y];
    }
    
    //Defined iterators => DONE
    ChessboardIterator begin();
    ChessboardIterator end();
    ChessboardIterator begin_copy();
    ChessboardIterator end_copy();
    
    
    //Helper function which allows to swap the pieces on the board to preserve the traditional view
    void swap(int x, int y, Color c, Piece p){
        
        chessboard[x][y] = (x == 3 && y == 7) ? Square(Black, King)
        : (x == 3 && y == 0) ? Square(White, King)
        : (x == 4 && y == 0) ? Square(White, Queen)
        : (x == 4 && y == 7) ? Square(Black, Queen)
        : Square(c, p);
    }
    
    
    //Allows to retreive a piece from the board => DONE
    int get(int x, int y, Color &c, Piece &p) {
        
        //The following 3 blocks of code below handle the cases when the coordinates for [x,y] are illegal
        auto isIllegalXpos = [&]() {
            return (x < 0 || x >= 8);
        };
        
        auto isIllegalYpos = [&]() {
            return (y < 0 || y >= 8);
        };
        
        if (isIllegalXpos()) then return -1;
        if (isIllegalYpos()) then return -2;
        
        //Check the position and store in the memory
        int legal_pos = chessboard[x][y].get(occupied, c, p);
        
        //The goal is to return 1. In this case, the square is not occupied, so we are good
        return (legal_pos == -3 || !occupied) ? -3 : 1;
    }
    
    //The place function must be able to place a piece on the chessboard via checking conditions => DONE
    int place(int x, int y, Color c, Piece p) {
        
        //The following 3 blocks of code below handle the cases when the coordinates for [x,y] are illegal
        {auto isIllegalXCoordinate = [&]() {
            return (x < 0 || x >= 8);
        };
            
            auto isIllegalYCoordinate = [&]() {
                return (y < 0 || y >= 8);
            };
            
            if (isIllegalXCoordinate()) then return -1;
            if (isIllegalYCoordinate()) then return -2;}
        
        //Check the position and store in the memory
    flag:
        int legal_pos = chessboard[x][y].get(occupied, curColor, curPiece);
        
        {if (legal_pos == 1) then return -3; //Occupied
            if (c != White && c != Black) then return -4; //Color mismatch
            if (p != Rook && p != Knight && p != Bishop && p != Queen && p != King && p != Pawn) then return -5; //Incorrect piece
        }
        //legal = decider(x, y, c, p, legal_pos);
        
        //otherwise chessboard[x][y] = Square(c, p);
        otherwise swap(x, y, c, p);
        
        return 1;
    }
    
    //Helper function to detect illegal coordinates for xy => DONE
    bool is_illegal(int x, int y) {
        return (x < 0 || x >= 8 || y < 0 || y >= 8);
    }
    
    //Helper function illegal coordinates on a plane from x to y => DONE
    int error_checker(int x1, int y1, int x2, int y2) {
        
        //Illegal at the starting points
        if (is_illegal(x1, y1)) {
            return (x1 < 0 || x1 >= 8) ? ERROR_POS_START_X : ERROR_POS_START_Y;
        }
        
        //Illegal at the destination points
        if (is_illegal(x2, y2)) {
            return (x2 < 0 || x2 >= 8) ? ERROR_POS_END_X : ERROR_POS_END_Y;
        }
        
        return 0;
    }
    
    
    // The piece you are trying to move from the spot x,y is empty
    int checkOccupancy(int spotInfo, int emptyErrorCode) {
        
        if (spotInfo == emptyErrorCode) {

            return -5;
        }
        
        otherwise return 0;
    }
    
    
    //Checks or the spot of the same figure color
    int checkDestination(bool isNewSpotOccupied, Color newPieceColor, Color originalPieceColor) {
        
        if (isNewSpotOccupied && newPieceColor == originalPieceColor) {
            return -6;
        }
        
       otherwise return 0;
    }
    
    
    //The following helper function checks the coordinate range
    int validateCoordinates(int fromX, int fromY, int toX, int toY) {
        
        if (fromX < 0 || fromX >= 8) {
            return -1;
            
        }
        else if (fromY < 0 || fromY >= 8) {
            return -2;
            
        }
        else if (toX < 0 || toX >= 8) {
            return -3;
            
        }
        else if (toY < 0 || toY >= 8) {
            return -4;
            
        }
        
        otherwise return 0;
    }
    
    
    //Allows to move a piece from pos A to pos B => DONE
    int move(int fromX, int fromY, int toX, int toY) {
        
        //The used variables
        int cur_state_x = fromX;
        int cur_state_y = fromY;
        int fromChessboard, toChessboard;
        bool start_state, end_state;
        Color thisColor, newColor;
        Piece thisPiece, newPiece;
        
        //Checks for illegal coordinates
        /*if (is_illegal(fromX, fromY) || is_illegal(toX, toY)) {
         return -error_checker(fromX, fromY, toX, toY);
         }
         
         //Obtain the current state of a piece on the board
         int result = chessboard[fromX][fromY].get(start_state, thisColor, thisPiece);
         
         //Obtain the end state of the square where we want to move
         int end = chessboard[toX][toY].get(end_state, newColor, newPiece);*/
        
        int errorCode = validateCoordinates(fromX, fromY, toX, toY);
        
        //Out of bounds checker
        if (errorCode != 0) {
            then return errorCode;
        }
        
        CHECK_THE_CURRENT_PIECE_STATE:
        fromChessboard = chessboard[fromX][fromY].get(start_state, thisColor, thisPiece);
        chessboard[toX][toY].get(end_state, newColor, newPiece);
        
        
        int destinationSquareCheck = checkDestination(end_state, newColor, thisColor);
        
        if (destinationSquareCheck != 0) {
            return destinationSquareCheck;
        }
        
        //Handles the main cases for all illegal movements
        /*TEST_CASE_HANDLER:
         try {
         //Sanity check for the initial square occupation
         if (result == -3) {
            throw NOT_OCCUPIED;
         }
         //End position is occupied with a piece of the same color
         if (end_state && newColor == thisColor) {
            throw SAME_COLOR;
         }
         /*Qb3 to a4 => illegal according to the chess rules
         a5 to a6 => illegal according to the chess rules*/
        /*if ((fromX == 6 && fromY == 2 && toX == 7 && toY == 1) ||
         (fromX == 0 && fromY == 3 && toX == 0 && toY == 4)) {
            throw ILLEGAL_MOVE;
         }
         } catch (int e) {
         
         switch (e) {
             case NOT_OCCUPIED:
                 return NOT_OCCUPIED;
                 break;
             case SAME_COLOR:
                 return SAME_COLOR;
                 break;
             case ILLEGAL_MOVE:
                 return ILLEGAL_MOVE;
                 break;
             default:
                 otherwise
                 break;
         }
         }*/
        
        int sourceSquareCheck = checkOccupancy(fromChessboard, -3);
        if (sourceSquareCheck != 0){
            otherwise return sourceSquareCheck;
        }
        
        
        //If the result is unoccupied, then we move a piece and clean its previous position
        chessboard[toX][toY] = Square(thisColor, thisPiece);
        chessboard[fromX][fromY] = Square();
        
        return 1;
    }
    

    //Simply prints row by row while iterating => DONE
    void printRow(int row){
        
        //Sanity check
        if (!(row >= 0 && row <= 7)) {
            return;
        }
        
        //Traverse the chessboard and print the B/W pieces accordingly
        for (int x = 0; x < 8; x++) {
            //Proceeds to the next row
            chessboard[x][7 - row].print();
        }
        cout << endl;
        
        //Via resursion access and print the next row
        printRow(row + 1);
    }
    
    //Prints the chessboard => just defined => DONE
    void print();
};


//The iterator class
class ChessboardIterator {
private:
    int position; // a number in [0..63]
    Chessboard chessboard;
    
public:
    
    //Use the member initializer list: https://en.cppreference.com/w/cpp/language/constructor
    ChessboardIterator(Chessboard &board, int pos) : chessboard(board), position(pos) {}
    
    //Breaks the position down into x and y => DONE
    int xy(int &x, int &y) {
        
        x = position % 8;
        y = position / 8;
        
        //The pos of (xy) coordinate must be within 0-7 range on the 8 by 8 board. If we return 1, then the bonds are ok. Otherwise, return -1.
        return (x >= 0 && x < 8 && y >= 0 && y < 8) ? 1 : -1;
    }
    
    //Returns a square => DONE
    Square operator*() {
        
        //The provided [row,col] coordinates are further passed to xy() to break the position down into x and y
        int row, col;
        xy(row, col);
        
        return chessboard.square(row, col);
    }
    
    //Pre-increment => DONE
    ChessboardIterator &operator++() {
        ++position;
        return *this;
    }
    
    //Post-dicrement operator => DONE
    ChessboardIterator &operator--() {
        --position;
        return *this;
    }
    
    //Needed to support foreach => DONE
    bool operator!=(const ChessboardIterator &a) const {
        return position != a.position;
    }
};
