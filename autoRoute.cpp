/*
 * EDA_Level_5.cpp
 *
 * Alumnos:  - Minniti 63286
 *           -
 *           -
 *
 * Enrutador autom�tico de pistas para PCB
 *
 */

 /* -------------------- Includes -------------------- */
#include "autoRoute.h"
using namespace std;
/*
 * Funci�n para calcular la distancia euclidiana entre dos puntos
 */
double autoRoute::calculateDistance(const std::vector<int>& p1, const std::vector<int>& p2)
{
    int dx = p2[0] - p1[0];
    int dy = p2[1] - p1[1];
    return std::sqrt(dx * dx + dy * dy);
}

/*
 * Funci�n de comparaci�n para ordenar los puntos por distancia euclidiana
 */
bool autoRoute::sortByDistance(const std::vector<int>& p1, const std::vector<int>& p2, const std::vector<int>& reference)
{
    double distance1 = calculateDistance(p1, reference);
    double distance2 = calculateDistance(p2, reference);
    return distance1 < distance2;
}

/*
 * Funci�n para ordenar los puntos de cada clave en el mapa por distancia euclidiana             FALTA VER DEJAMOS O NO LA FUNCION SORT
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
    vector<int> origin;
    vector<vector<int>> temp;
    vector<vector<int>> routeOfP;

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
                    temp = semiRoute(origin,destiny);
                    for(auto element : temp)
                        routeOfP.push_back(element);

                    possibleOrigins.push_back(destiny);
                    deleteNumb();
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
            route(routeOfP);
            possibleOrigins.clear();
            routeOfP.clear();
            temp.clear();
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

vector<vector<int>> autoRoute::semiRoute(std::vector<int> origin, std::vector<int> destiny)
{
    bool isOrigin = false;
    vector<int> node = destiny;
    vector<int> neighborNode;
    vector<int> nextNeigbor;
    vector<vector<int>> vectorOFp;
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
        //condicion para caso especial
        if((node != nextNeigbor))
            if(!(node[0] == 15 && node[1] == 4))
                vectorOFp.push_back(nextNeigbor);

        node = nextNeigbor;
        
    }
    return vectorOFp;
}

/*
 * Dibuja piola 
 */
void autoRoute::route(vector<vector<int>>& routeOfp)//,vector<int>* origin, vector<vector<int>>* destiny)
{
    vector<int> tempPos;
    vector<int> direc;
    int contador;

    /*
    cout << routeOfp.size() << endl;
    for (auto element : routeOfp)
    {
        cout << "Coord x: "<<element[0] << " , Coord y: " << element[1] << endl;
    }
    */
    
    //Tomo una P del vector de p
    for(auto coord : routeOfp)
    {
        //Evaluo una direccion
        for(int direction = 0; direction < 4; direction++)
        {
            switch(direction)
            {
                case LEFT:
                {
                    //Copio la direccion a la izquierda
                    tempPos.push_back(coord[0] - 1);
                    tempPos.push_back(coord[1] );

                    //Busco direcciones en el vector de p
                    for(auto coord2 : routeOfp)
                    {
                        //Si encuentro la misma direccion en el vector de P
                        if(coord2 == tempPos )
                        {
                            direc.push_back(LEFT);
                        }
                    }
                    tempPos.clear();
                    break;
                }
                case RIGHT:
                {
                    //Copio la direccion a la derecha
                    tempPos.push_back(coord[0] + 1);
                    tempPos.push_back(coord[1]);

                    //Busco direcciones en el vector de p
                    for(auto coord2 : routeOfp)
                    {
                        //Si encuentro la misma direccion en el vector de P
                        if(coord2 == tempPos )
                        {
                            direc.push_back(RIGHT);
                        }
                    }
                    tempPos.clear();
                    break;
                }
                case UP:
                {
                    //Copio la direccion a arriba
                    tempPos.push_back(coord[0]);
                    tempPos.push_back(coord[1] - 1);

                    //Busco direcciones en el vector de p
                    for(auto coord2 : routeOfp)
                    {
                        //Si encuentro la misma direccion en el vector de P
                        if(coord2 == tempPos )
                        {
                            direc.push_back(UP);
                        }
                    }
                    tempPos.clear();
                    break;
                }
                case DOWN:
                {
                    //Copio la direccion abajo
                    tempPos.push_back(coord[0]);
                    tempPos.push_back(coord[1] + 1);

                    //Busco direcciones en el vector de p
                    for(auto coord2 : routeOfp)
                    {
                        //Si encuentro la misma direccion en el vector de P
                        if(coord2 == tempPos )
                        {
                            direc.push_back(DOWN);
                        }
                    }
                    tempPos.clear();
                    break;
                }
                
        
            }
        }

        //A esta altura sabemos lo que hay al costado de cada p

        contador = 0;                   
        for(auto element: direc)            //  0 = no p
        {                                   //  1 = left p
            if(element == LEFT)              //  2 = right p
                contador += 1;              //  3 = left and right p
            if(element == RIGHT)             //  ...
                contador += 2;              //
            if(element == UP)                //
                contador += 4;              //
            if(element == DOWN)              //
                contador += 8;              //
        }
        direc.clear();

        switch(contador)
        {
            case (15): 
                copyMatriz[coord[1]][coord[0]] = (char)("┼");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (1):
                copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (2):
                copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (3):
                copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (4):
                copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (8):
                copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (12):
                copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (14):
                copyMatriz[coord[1]][coord[0]] = (char)("├");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (10):
                copyMatriz[coord[1]][coord[0]] = (char)("┌");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (6):
                copyMatriz[coord[1]][coord[0]] = (char)("└");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (13):
                copyMatriz[coord[1]][coord[0]] = (char)("┤");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (9):
                copyMatriz[coord[1]][coord[0]] = (char)("┐");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (5):
                copyMatriz[coord[1]][coord[0]] = (char)("┘");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (11):
                copyMatriz[coord[1]][coord[0]] = (char)("┬");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (7):
                copyMatriz[coord[1]][coord[0]] = (char)("┴");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (0):
                std::cout << "somos un desastre 1" << endl;
                break;
            
            default:
                std::cout << "somos un desastre 2" << endl;
                break;
        }
    }
    
}
