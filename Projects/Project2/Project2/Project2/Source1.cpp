/*
#include "Sequence.h"
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

void test()
{
	cerr << "test A" << endl;
	Sequence s;
	cerr << "test B" << endl;
	s.insert(0, IntWrapper(10));
	cerr << "test C" << endl;
	s.insert(0, IntWrapper(20));
	cerr << "test D" << endl;
	Sequence s2;
	cerr << "test E" << endl;
	s2.insert(0, IntWrapper(30));
	cerr << "test F" << endl;
	s2 = s;
	cerr << "test G" << endl;
	s2.insert(0, IntWrapper(40));
	cerr << "test H" << endl;
}

int main()
{
	test();
	cerr << "DONE" << endl;
	_CrtDumpMemoryLeaks();
}
*/