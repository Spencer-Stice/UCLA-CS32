
#include "ScoreList.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
    ScoreList s;
    for(int i = 0; i<11;i++)
        s.add(i+14);
    bool test = s.remove(5);
    int r = s.minimum();
    cout << s.size();
    cout << "Passed all tests" << endl;
}
