/*
 * EDA_Level_5.cpp
 *
 * Alumnos:  - Minniti 63286
 *           -
 *           -
 *
 * Enrutador automático de pistas para PCB
 *
 */

 /* -------------------- Includes -------------------- */
#include "autoRoute.h"

/*
 * Función para calcular la distancia euclidiana entre dos puntos
 */
double autoRoute::calculateDistance(const std::vector<int>& p1, const std::vector<int>& p2)
{
    int dx = p2[0] - p1[0];
    int dy = p2[1] - p1[1];
    return std::sqrt(dx * dx + dy * dy);
}

/*
 * Función de comparación para ordenar los puntos por distancia euclidiana
 */
bool autoRoute::sortByDistance(const std::vector<int>& p1, const std::vector<int>& p2, const std::vector<int>& reference)
{
    double distance1 = calculateDistance(p1, reference);
    double distance2 = calculateDistance(p2, reference);
    return distance1 < distance2;
}

/*
 * Función para ordenar los puntos de cada clave en el mapa por distancia euclidiana             FALTA VER DEJAMOS O NO LA FUNCION SORT
 */
void autoRoute::sortInterconnections(std::vector<int> origin, std::vector<std::vector<int>> list)
{
    //std::vector<std::vector<int>>& points = *list;
    std::sort(list.begin(), list.end(), [&](const std::vector<int>& p1, const std::vector<int>& p2)
        {
            return sortByDistance(p1, p2, origin);
        });
}