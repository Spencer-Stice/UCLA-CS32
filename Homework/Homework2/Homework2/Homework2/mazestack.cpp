/*
#include <stack>
#include <string>
#include <iostream>
using namespace std;

////////////////////////////

class Coord
{
public:
    Coord(int rr, int cc) : m_row(rr), m_col(cc) {}
    int r() const { return m_row; }
    int c() const { return m_col; }
private:
    int m_row;
    int m_col;
};

//////////////////////////////

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    stack<Coord> coordStack;
    coordStack.push(Coord(sr, sc));
    maze[sr][sc] = '#';
    while (coordStack.size() != 0)
    {
        Coord currentCoord = coordStack.top();
        coordStack.pop();
        int cR = currentCoord.r();
        int cC = currentCoord.c();
        if (cR == er && cC == ec)
            return true;
        if (maze[cR - 1][cC] == '.')
        {
            coordStack.push(Coord(cR - 1, cC));
            maze[cR - 1][cC] = '#';
        }
        if (maze[cR][cC+1] == '.')
        {
            coordStack.push(Coord(cR, cC+1));
            maze[cR][cC+1] = '#';
        }
        if (maze[cR + 1][cC] == '.')
        {
            coordStack.push(Coord(cR + 1, cC));
            maze[cR + 1][cC] = '#';
        }
        if (maze[cR][cC - 1] == '.')
        {
            coordStack.push(Coord(cR, cC - 1));
            maze[cR][cC - 1] = '#';
        }
        
    }
    return false;
    /*        Push the starting coordinate (sr,sc) onto the coordinate stack and
            update maze[sr][sc] to indicate that the algorithm has encountered
            it (i.e., set maze[sr][sc] to have a value other than '.').
        While the stack is not empty,
        {   Pop the top coordinate off the stack. This gives you the current
                (r,c) location that your algorithm is exploring.
            If the current (r,c) coordinate is equal to the ending coordinate,
                then we've solved the maze so return true!
            Check each place you can move from the current cell as follows:
                If you can move NORTH and haven't encountered that cell yet,
                    then push the coordinate (r-1,c) onto the stack and update
                    maze[r-1][c] to indicate the algorithm has encountered it.
                If you can move EAST and haven't encountered that cell yet,
                    then push the coordinate (r,c+1) onto the stack and update
                    maze[r][c+1] to indicate the algorithm has encountered it.
                If you can move SOUTH and haven't encountered that cell yet,
                    then push the coordinate (r+1,c) onto the stack and update
                    maze[r+1][c] to indicate the algorithm has encountered it.
                If you can move WEST and haven't encountered that cell yet,
                    then push the coordinate (r,c-1) onto the stack and update
                    maze[r][c-1] to indicate the algorithm has encountered it.
        }
        There was no solution, so return false
        
        
}


// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

/////////////////////////////////////
*/