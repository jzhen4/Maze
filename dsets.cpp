#include <vector>
#include "dsets.h"

void DisjointSets::addelements(int num)
{
	int originalSize = v.size();
	v.resize(v.size() + num);
	for (int i = originalSize; i<v.size(); i++)
		v[i]=-1;
}

int DisjointSets::find(int elem)
{
	//recursive search
	//base case. only roots have negative numbers
	if (v[elem]<0) return elem;
	
	v[elem] = find(v[elem]);
	return find(v[elem]);
}

void DisjointSets::setunion(int a, int b)
{
	a = find(a);
	b = find(b);
	int size = v[a] + v[b];
	if (v[a] <= v[b])
	{
		v[b] = a;
		v[a] = size;
	} else
	{
		v[a] = b;
		v[b] = size;
	}
}

