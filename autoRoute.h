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
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cmath>

class autoRoute
{
	public:


	private:

		void sortInterconnections(std::vector<int>, std::vector<std::vector<int>>);
		bool sortByDistance(const std::vector<int>&, const std::vector<int>&, const std::vector<int>&);
		double calculateDistance(const std::vector<int>&, const std::vector<int>&);

};

#endif // AUTOROUTER_H