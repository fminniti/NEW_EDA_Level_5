#ifndef AUTOROUTER_H
#define AUTOROUTER_H

/*          EDA_Level_5.cpp
*
* Alumnos:  - Minniti 63286
*           -
*           -
*
* Enrutador automático de pistas para PCB
*
*/

/*          Includes           */
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

class autoRoute
{
	public:
		std::vector<std::vector<char>> initAutoRoute(std::vector<std::vector<char>>& matriz,
													 std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections);


	private:
		std::vector<std::vector<char>> copyMatriz;
		std::vector<size_t> matrixLimit;
		std::vector<std::vector<int>> possibleOrigins;

		void printMat();

		void deleteNumb(void);
		void semiRoute(std::vector<int> , std::vector<int>);

		std::vector<int> getNeighborNode(std::vector<int>*, int);
		bool findDestiny(std::vector<int>*, std::vector<int>*);

		void funCopyMatrix(std::vector<std::vector<char>>&);

		void sortInterconnections(const std::vector<int>*, std::vector<std::vector<int>>*);
		bool sortByDistance(const std::vector<int>&, const std::vector<int>&, const std::vector<int>&);
		double calculateDistance(const std::vector<int>&, const std::vector<int>&);

};

#endif // AUTOROUTER_H