#ifndef AUTOROUTER_H
#define AUTOROUTER_H

/*          EDA_Level_5.cpp
*
* Alumnos:  - Minniti 63286
*           -
*           -
*
* Enrutador autom�tico de pistas para PCB
*
*/

/*          Includes           */
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

enum DIRECTIONS {LEFT = 0,RIGHT, UP, DOWN};

using namespace std;
 
class autoRoute
{
	public:
		std::vector<std::vector<char>> initAutoRoute(std::vector<std::vector<char>>& matriz,
													 std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections);


	private:
		vector<std::vector<char>> copyMatriz;
		vector<size_t> matrixLimit;
		vector<std::vector<int>> possibleOrigins;

		vector<vector<int>> semiroute(vector<int>* origin, vector<int>* destiny);
		void route(vector<vector<int>>& routeOfp,vector<int>* origin, vector<vector<int>>* destiny);
		vector<int> getNeighborNode(std::vector<int>*, int);
		bool findDestiny(std::vector<int>*, std::vector<int>*);

		void funCopyMatrix(std::vector<std::vector<char>>&);

		void route(vector<vector<int>>& routeOfp);//,vector<int>* origin, vector<vector<int>>* destiny);
		char defPositionP(vector<int> pPoint, vector<int> *origin, vector<vector<int>>* destiny);
		void deleteNumb(void);
		void sortInterconnections(const std::vector<int>*, std::vector<std::vector<int>>*);
		bool sortByDistance(const std::vector<int>&, const std::vector<int>&, const std::vector<int>&);
		double calculateDistance(const std::vector<int>&, const std::vector<int>&);

};

#endif // AUTOROUTER_H