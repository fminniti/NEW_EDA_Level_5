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
void autoRoute::sortInterconnections(const std::vector<int>* origin, std::vector<std::vector<int>>* list)
{
    std::vector<std::vector<int>>& points = *list;
    std::sort(points.begin(), points.end(), [&](const std::vector<int>& p1, const std::vector<int>& p2)
        {
            return sortByDistance(p1, p2, *origin);
        });
}

/*
 *
 */
void autoRoute::funCopyMatrix(std::vector<std::vector<char>>& source)
{
    size_t rows = source.size();
    size_t cols = source[0].size();

    copyMatriz.resize(rows, std::vector<char>(cols));

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            copyMatriz[i][j] = source[i][j];
        }
    }

    matrixLimit.push_back(cols);
    matrixLimit.push_back(rows);
}

std::vector<std::vector<char>> autoRoute::initAutoRoute(std::vector<std::vector<char>>& matriz,
    std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections)
{
    bool isFind = false;
    std::vector<int> origin;

    funCopyMatrix(matriz);

    for (auto& entry : interconnections)
    {
        sortInterconnections(&entry.first, &entry.second);
    }

    for (auto& entry : interconnections)
    {
        possibleOrigins.push_back(entry.first);
        for (auto& destiny : entry.second)
        {
            sortInterconnections(&destiny, &possibleOrigins);
            for (auto& origin : possibleOrigins)
            {
                deleteNumb();
                isFind = findDestiny(&origin, &destiny);

                if (isFind == true)
                {
                    semiRoute(origin,destiny);
                    possibleOrigins.push_back(destiny);
                    deleteNumb();
                    printMat();
                    break;
                }
            }
            if (isFind == false)
            {
                //no pudo encontrar un camino y deberiamos emepezar de nuevo(LO DEJAMOS PARA EL FINAL)
            }
        }
        if(isFind == true)
        {
            isFind = false;
            //se llama a la funcion route() que completa con la PISTA CORRESPONDIENTE en las casillas que alla una "P"
            possibleOrigins.clear();
        }
    }

    return copyMatriz;
}

/*
 * Esta mal la logica de los pasos, hay que pensar como hacerlo pero esta funcion creo que es la unica opcion.
 * Esta funcion lo unico que hace es completar la matriz con los numeros de los pasos dados hasta encontrar el destino desde un cierto origen.
 */
bool autoRoute::findDestiny(std::vector<int>* origin, std::vector<int>* destiny)
{
    std::queue<std::vector<int>> q;
    q.push(*origin);
    char stepCounter = 0;
    std::vector<int> neighborNode;

    while (!q.empty())
    {
        std::vector<int> node = q.front();
        q.pop();

        stepCounter = copyMatriz[node[1]][node[0]];
        if (stepCounter >= 32)
        {
            stepCounter = 0;
        }
        stepCounter++;

        for (int i = 0; i < 4; i++)
        {
            neighborNode = getNeighborNode(&node, i);

            if (neighborNode.empty())
            {
                continue;
            }
            else
            {
                char dataNode = copyMatriz[neighborNode[1]][neighborNode[0]];

                if (dataNode == '.')
                {
                    q.push(neighborNode);
                    copyMatriz[neighborNode[1]][neighborNode[0]] = stepCounter;
                }
                else if (dataNode == '#')
                {
                    if ( (neighborNode[0] == (*destiny)[0]) && (neighborNode[1] == (*destiny)[1]) ) return true;
                }
            }
        }
    }
    return false;
}

/*
 *
 */
std::vector<int> autoRoute::getNeighborNode(std::vector<int>* origin, int orientation)
{
    std::vector<int> node;
    node.clear();
    if (orientation == 0) //UP
    {
        if ((*origin)[1] > 0)
        {
            node.push_back((*origin)[0]);
            node.push_back((*origin)[1] - 1); 
        }
    }
    if (orientation == 1) //DOWN
    {
        if ((*origin)[1] < (matrixLimit[1]-1))
        {
            node.push_back((*origin)[0]);
            node.push_back((*origin)[1] + 1);
        }
    }
    if (orientation == 2) //Left
    {
        if ((*origin)[0] > 0)
        {
            node.push_back((*origin)[0] - 1);
            node.push_back((*origin)[1]);
        }
    }
    if (orientation == 3) //Right
    {
        if ((*origin)[0] < (matrixLimit[0]-1))
        {
            node.push_back((*origin)[0] + 1);
            node.push_back((*origin)[1]);
        }
    }
    return node;
}

void autoRoute::printMat()
{
    std::cout << "\nMatriz:\n";
    for (const auto& row : copyMatriz)
    {
        for (const auto& value : row)
        {
            if (value < 35)
            {
                std::cout << static_cast<int>(value) << ' ';
            }
            else
            {
                std::cout << value << ' ';
            }
        }
        std::cout << '\n';
    }
}

/*
 * Funcion encargada de eliminar los numeros
 */
void autoRoute::deleteNumb(void)
{
    for (auto& row : copyMatriz)
    {
        for (auto& element : row)
        {
            if (element < 35)
            {
                element = '.';
            }
        }
    }
}

void autoRoute::semiRoute(std::vector<int> origin, std::vector<int> destiny)
{
    bool isOrigin = false;
    std::vector<int> node = destiny;
    std::vector<int> neighborNode;
    std::vector<int> nextNeigbor;
    char minValue = 127;

    while (!isOrigin)
    {
        if( (node[1] != destiny[1]) || (node[0] != destiny[0]))
        {
            copyMatriz[node[1]][node[0]] = 'p';
            possibleOrigins.push_back(node);
        }

        for (int i = 0; i < 4; i++)
        {
            neighborNode = getNeighborNode(&node, i);
            char dataNode = copyMatriz[neighborNode[1]][neighborNode[0]];

            if (neighborNode.empty())
            {
                continue;
            }
            if (dataNode == '#' || dataNode == 'p')
            {
                if ((neighborNode[0] == origin[0]) && (neighborNode[1] == origin[1]))
                {
                    isOrigin = true;
                    break;
                }
            }
            else if (dataNode < 35)
            {
                if (dataNode < minValue)
                {
                    minValue = dataNode;
                    nextNeigbor = neighborNode;
                }
            }
        }
        node = nextNeigbor;
    }
}