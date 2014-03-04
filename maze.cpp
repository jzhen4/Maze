#include "maze.h"

void SquareMaze::makeMaze(int width, int height)
{

	//clear maze if one exists
	if (right.size() > 0)
	{
		right.clear();
		bottom.clear();
		if (mazeImage != NULL)
		{
			delete mazeImage;
			mazeImage = NULL;
		}
	}

	//set values
	w = width;
	h = height;
	s = w*h;

	//coords
	int x,y;

	//used to test for cycles
	DisjointSets cycletest;
	cycletest.addelements(s);
	right.resize(s);
	bottom.resize(s);

	//set all walls to true
	for (int i=0; i<s; i++)
	{
		right[i] = true;
		bottom[i] = true;
	}
	
	//generate random seed
	srand(time(NULL));
	//random number
	int randNum;
	
	//bools for testing if on border
	bool rborder, bborder;

	//total walls removed
	int wallsremoved = 0;
	//random number for index
	int i;

	while(wallsremoved < (s-1))
	{
		i = rand() % s;
		x = i % w;
		y = i / w;

		if ((x == (w-1)) && (y == (h-1)))
		{
			rborder =  true;
			bborder = true;
		}else if (x == (w-1))
		{
			randNum = rand() % 2;
			rborder = true;
			bborder = false;
		}else if (y == (h-1))
		{
			randNum = rand() % 2;
			rborder = false;
			bborder = true;
		}else
		{
			randNum = rand() % 3;
			rborder = false;
			bborder = false;
		}

		if ((rborder == true) && (bborder == true))
		{
			//do nothing
		} else if ((rborder == false) && (bborder == false))
		{
			//delete right wall
			if (randNum == 0)
			{
				if (cycletest.find(i) != cycletest.find(i+1))
				{
					setWall(x, y, 0, false);
					cycletest.setunion(i, i+1);
					wallsremoved++;
				}
			}else if (randNum == 1) //delete bottom wall
			{
				if (cycletest.find(i) != cycletest.find(i+w))
				{
					setWall(i%w, i/w, 1, false);
					cycletest.setunion(i, i+w);
					wallsremoved++;
				}
			}else //delete both walls
			{
				if ((cycletest.find(i) != cycletest.find(i+1)) && (cycletest.find(i) != cycletest.find(i+w)) && (cycletest.find(i+1) != cycletest.find(i+w)))
				{
					setWall(x, y, 0, false);					
					setWall(x, y, 1, false);
					cycletest.setunion(i, i+1);
					cycletest.setunion(i, i+w);
					wallsremoved+=2;
				}
			}
		}else if (rborder == true) // on right border
		{
			if (randNum == 1) //delete bottom wall
			{
				if (cycletest.find(i) != cycletest.find(i+w))
				{				
					setWall(x, y, 1, false);
					cycletest.setunion(i, i+w);
					wallsremoved++;
				}
			}
		}else if (bborder == true) //on bottom border
		{
			if (randNum == 1) //delete right wall
			{
				if (cycletest.find(i) != cycletest.find(i+1))
				{				
					setWall(x, y, 0, false);
					cycletest.setunion(i, i+1);
					wallsremoved++;
				}
			}
		}
	}
}

bool SquareMaze::canTravel(int x, int y, int dir) const
{
	int idx = y*w + x;

	switch (dir) {
		case 0: // +x
			if (x == (w-1)) // if we are on the right edge
				return false;
			if (right[idx] == true) // check if there is a wall to the right
				return false;
			else
				return true;
		case 1: // +y
			if (y == (h-1)) // if we are on the bottom edge
				return false;
			if (bottom[idx] == true) // check if there is a wall to the bottom
				return false;
			else
				return true;
		case 2: // -x
			if (x == 0) // if we are on the left edge
				return false;
			if (right[idx-1] == true) // check if there is a wall to the right of the left cell
				return false;
			else
				return true;
		case 3: // -y
			if (y == 0) // if we are on the top edge
				return false;
			if (bottom[idx-w] == true) // check if there is a wall to the bottom of the upper cell
				return false;
			else
				return true;
	}

	return false; // this will never be reached as long as dir is between 0 and 3
}

void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	if (exists) { // set wall
		if (dir == 0) { // right wall
			right[(y*w) + x] = true;
		} else if (dir == 1) { // bottom wall
			bottom[(y*w) + x] = true;
		}
	} else { // remove wall
		if (dir == 0) { // right wall
			right[(y*w) + x] = false;
		} else if (dir == 1) { // bottom wall
			bottom[(y*w) + x] = false;
		}
	}
}

vector<int> SquareMaze::solveMaze()
{
	int *distances = new int[s];
	int *parents = new int[s];

	findMaxDist(0, 0, distances, 1, parents); // stores distances in 'distances'
		
	maxIndex = 0;
	for (int i=0; i < w; i++) { // store the maximum distance on the bottom row to maxIndex
		if (distances[w*(h-1) + i] > distances[maxIndex])
			maxIndex = w*(h-1) + i;
	}
    
	vector<int> tempSol;
    
	int j = maxIndex;
    
	while (j != 0) { // store backwards path in tempSol
		if (parents[j] == 0) {
			tempSol.push_back(0);
			j++;
		} else if (parents[j] == 1) {
			tempSol.push_back(1);
			j+=w;
		} else if (parents[j] == 2) {
			tempSol.push_back(2);
			j--;
		} else {
			tempSol.push_back(3);
			j-=w;
		}
	}
    
	int temp;
	for (int k=tempSol.size() - 1; k >= 0; k--) { // reverse tempSol and store in solution
		if (tempSol[k] == 0)
			temp = 2;
		else if (tempSol[k] == 1)
			temp = 3;
		else if (tempSol[k] == 2)
			temp = 0;
		else
			temp = 1;
		solution.push_back(temp);
	}

		delete [] parents;
		parents = NULL;
		delete [] distances;
		distances = NULL;
	return tempSol;
}

void SquareMaze::findMaxDist(int i, int depth, int * distances, int dirToHere, int * parents)
{
	int x = i%w; // convert index to coordinates
	int y = i/w;
    
	distances[i] = depth; // store dist in 'distances'
        
	if (canTravel(x, y, 0) && (dirToHere != 2)) { //travel right
		parents[i+1] = 2;
		findMaxDist(i+1, depth+1, distances, 0, parents);
	}
	if (canTravel(x, y, 1) && (dirToHere != 3)) { //travel down
		parents[i+w] = 3;
		findMaxDist(i+w, depth+1, distances, 1, parents);
	}
	if (canTravel(x, y, 2) && (dirToHere != 0)) { //travel left
		parents[i-1] = 0;
		findMaxDist(i-1, depth+1, distances, 2, parents);
	}
	if (canTravel(x, y, 3) && (dirToHere != 1)) { //travel up
		parents[i-w] = 1;
		findMaxDist(i-w, depth+1, distances, 3, parents);
	}
}

PNG* SquareMaze::drawMaze()
{
	mazeImage = new PNG(w*10+1, h*10+1);
    int x, y; // index converted to x and y values
    
    for (int i=0; i < mazeImage->width(); i++) { // blacken topmost row
        if (!((i >= 1) && (i <= 9))) {
            (*mazeImage)(i, 0)->red = 0;
            (*mazeImage)(i, 0)->green = 0;
            (*mazeImage)(i, 0)->blue = 0;
        }
    }
    
    for (int i=0; i < mazeImage->height(); i++) { // blacken leftmost column
        (*mazeImage)(0, i)->red = 0;
        (*mazeImage)(0, i)->green = 0;
        (*mazeImage)(0, i)->blue = 0;
    }
    
    
    for (int i = 0; i < s; i++) { // loop until max number of walls have been removed
        x = i%w;
        y = i/w;
        
        if (right[i] == true) { // draw right wall
            for (int k=0; k <= 10; k++) {
                (*mazeImage)((x+1)*10, y*10+k)->red = 0;
                (*mazeImage)((x+1)*10, y*10+k)->green = 0;
                (*mazeImage)((x+1)*10, y*10+k)->blue = 0;
            }
        }
        if (bottom[i] == true) { // draw bottom wall
            for (int k=0; k <= 10; k++) {
                (*mazeImage)(x*10+k, (y+1)*10)->red = 0;
                (*mazeImage)(x*10+k, (y+1)*10)->green = 0;
                (*mazeImage)(x*10+k, (y+1)*10)->blue = 0;
            }
        }
    }
    
    return mazeImage;
}

PNG* SquareMaze::drawMazeWithSolution()
{
    drawMaze();
    int x = 5;
    int y = 5;
    int temp;
    
    for (int i=0; i < solution.size(); i++) {
        if (solution[i] == 0) { // right step
            temp = x+10;
            while (x <= temp) {
                (*mazeImage)(x, y)->red = 255;
                (*mazeImage)(x, y)->green = 0;
                (*mazeImage)(x, y)->blue = 0;
                x++;
            }
            x--;
        } else if (solution[i] == 1) { // downward step
            temp = y+10;
            while (y <= temp) {
                (*mazeImage)(x, y)->red = 255;
                (*mazeImage)(x, y)->green = 0;
                (*mazeImage)(x, y)->blue = 0;
                y++;
            }
            y--;
        } else if (solution[i] == 2) { // left step
            temp = x-10;
            while (x >= temp) {
                (*mazeImage)(x, y)->red = 255;
                (*mazeImage)(x, y)->green = 0;
                (*mazeImage)(x, y)->blue = 0;
                x--;
            }
            x++;
        } else { // upward step
            temp = y-10;
            while (y >= temp) {
                (*mazeImage)(x, y)->red = 255;
                (*mazeImage)(x, y)->green = 0;
                (*mazeImage)(x, y)->blue = 0;
                y--;
            }
            y++;
        }
    }
    
    x = maxIndex%w;
    y = maxIndex/w;
    for (int k=1; k <= 9; k++) { // Open up exit
        (*mazeImage)(x*10 + k, (y+1) * 10)->red = 255;
        (*mazeImage)(x*10 + k, (y+1) * 10)->green = 255;
        (*mazeImage)(x*10 + k, (y+1) * 10)->blue = 255;
    }
    
    return mazeImage;
}
































