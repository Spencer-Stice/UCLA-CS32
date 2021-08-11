/*
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

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec)
{
    bool solvable = false;
    if (sr == er && sc == ec)
        return true;
    maze[sr][sc] = '#';
    if (maze[sr - 1][sc] == '.' && !solvable)
        solvable = pathExists(maze, nRows, nCols, sr - 1, sc, er, ec);
    if (maze[sr][sc+1] == '.' && !solvable)
        solvable = pathExists(maze, nRows, nCols, sr, sc+1, er, ec);
    if (maze[sr + 1][sc] == '.' && !solvable)
        solvable = pathExists(maze, nRows, nCols, sr + 1, sc, er, ec);
    if (maze[sr][sc-1] == '.' && !solvable)
        solvable = pathExists(maze, nRows, nCols, sr, sc-1, er, ec);
    if (solvable)
        return true;
    else
        return false;
}
*/