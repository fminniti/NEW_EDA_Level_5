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
#include <string>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;
enum DIRE {LEFT = 0, RIGHT, UP, DOWN};

class autoRoute
{
	public:
		std::vector<std::vector<char>> initAutoRoute(std::vector<std::vector<char>>& matriz,
													 std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections);
		void printSpecialValue(char value);

	private:
		std::vector<std::vector<char>> copyMatriz;

		std::vector<size_t> matrixLimit;
		std::vector<std::vector<int>> possibleOrigins;

		void printMat();

		void deleteNumb(void);
		vector<vector<int>> semiRoute(std::vector<int> , std::vector<int>);

		std::vector<int> getNeighborNode(std::vector<int>*, int);
		bool findDestiny(std::vector<int>*, std::vector<int>*);

		void funCopyMatrix(std::vector<std::vector<char>>&);

		void route(vector<vector<int>>& routeOfp,vector<int> origin, std::vector<vector<int>> destiny);
		void sortInterconnections(const std::vector<int>*, std::vector<std::vector<int>>*);
		bool sortByDistance(const std::vector<int>&, const std::vector<int>&, const std::vector<int>&);
		double calculateDistance(const std::vector<int>&, const std::vector<int>&);

};

#endif // AUTOROUTER_H