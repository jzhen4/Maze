#ifndef MAZE_H
#define MAZE_H

#include "png.h"
#include "dsets.h"
#include <cstdlib>
#include <vector>

using namespace std;

class SquareMaze {

	public:
	void makeMaze(int width, int height);
	bool canTravel(int x, int y, int dir) const;
	void setWall(int x, int y, int dir, bool exists);
	vector<int> solveMaze();
	PNG * drawMaze();
	PNG * drawMazeWithSolution();
	vector<int> solution;

	private:
	int w, h, s;
	vector<bool> right, bottom;
	PNG * mazeImage;
	int maxIndex;
	void findMaxDist(int a, int depth, int *distances, int dirToHere, int *parents);
};

#endif
	
