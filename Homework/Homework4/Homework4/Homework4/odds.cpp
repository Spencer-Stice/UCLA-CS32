
void removeOdds(vector<int>& v)
{
	vector<int>::iterator it;
	it = v.begin();
	while (it != v.end())
	{
		if ((*it) % 2 != 0)
			it = v.erase(it);
		else
			it++;
	}
}
