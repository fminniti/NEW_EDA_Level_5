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
    std::vector<int> origin;
    vector<vector<int>> routeToSucces;

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
                //Aca hay que llamar a una funcion que elimine todos los valores numericos dentro de copyMatriz
                deleteNumb();

                isFind = findDestiny(&origin, &destiny);
                //cout << "\n\nSALIO DE FINDESTINY\n\n";
                if (isFind == true)
                {
                    //se llama a una funcion (llamemosma semiroute() ) que debe ir desde el desnio hasta el origen anterios completando con "P" en las casillas.
                    //La funcion semiroute debe ademas a�adir los puntos en los que pone "p" a la lista de possibleOrigins.

                    cout << "Estoy aca 00"<< endl;
                    routeToSucces = semiroute(&origin, &destiny);
                    cout << "Estoy aca 11"<< endl;

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
            route(routeToSucces);//, &(entry.first), &(entry.second));
            cout << "Sali de route" << endl;
            //se llama a la funcion route() que completa con la PISTA CORRESPONDIENTE en las casillas que alla una "P"
        }

    }

    return copyMatriz;
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
            if( element < 35)
                element = '.';
        }
    }
}

/*
 * Funcion que realiza el tramo de posibilidades y establece posibles origenes
 */
vector<vector<int>> autoRoute::semiroute(vector<int>* origin, vector<int>* destiny)
{
    vector<int> currentPoint = *destiny;
    vector<int> neighbor;
    vector<int> neighborMinCoord;
    vector<vector<int>> posibilities;
    int minValue = 10000;

    cout <<  (*destiny)[0] << " "<< (*destiny)[1]<<endl;
    cout <<  (*origin)[0] << " "<< (*origin)[1]<<endl;

    cout << "semiroute 00" << endl;
    while(currentPoint != *origin)
    {
        //For determina en que direccion se encuentra el menor valor
        for(int direction = 0; direction < 4; ++direction)
        {
            neighbor = getNeighborNode(&currentPoint, direction);

            if(static_cast<int>(copyMatriz[neighbor[1]][neighbor[0]]) < 35)
            {
                if(minValue > static_cast<int>(copyMatriz[neighbor[1]][neighbor[0]]))
                {
                    minValue = static_cast<int>(copyMatriz[neighbor[1]][neighbor[0]]);
                    neighborMinCoord = neighbor;
                }
            }
            else if(neighbor == *origin)
            {
                minValue = -1;
                neighborMinCoord = *origin;
                break;
            }
        }
        if(minValue > 0)
            posibilities.push_back(neighborMinCoord);

        currentPoint = neighborMinCoord;
        //cout <<  minValue <<endl;
        //cout <<  neighborMinCoord[1]<< " "<< neighborMinCoord[0]<<endl;
    }
    cout << "semiroute 11" << endl;

    for(auto coords : posibilities )
    {
        copyMatriz[coords[1]][coords[0]] = 'p';
        possibleOrigins.push_back(coords);

    }

    return  posibilities;
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
                    //std::cout << "PAD:\n";
                    if ( (neighborNode[0] == (*destiny)[0]) && (neighborNode[1] == (*destiny)[1]) ) return true;
                }
            }
        }
        /*
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
        */
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
    //std::cout << "NODO: " << (*origin)[0] << "-" << (*origin)[1] << std::endl;
    if (orientation == 0) //UP
    {
        if ((*origin)[1] > 0)
        {
            //std::cout << "10.0:\n";
            node.push_back((*origin)[0]);
            node.push_back((*origin)[1] - 1); 
            //std::cout << "Vecino: " << node[0] << "-" << node[1] << std::endl;
        }
    }
    if (orientation == 1) //DOWN
    {
        if ((*origin)[1] < (matrixLimit[1]-1))
        {
            //std::cout << "10.1:\n";
            node.push_back((*origin)[0]);
            node.push_back((*origin)[1] + 1);
            //std::cout << "Vecino: " << node[0] << "-" << node[1] << std::endl;
        }
    }
    if (orientation == 2) //Left
    {
        if ((*origin)[0] > 0)
        {
            //std::cout << "10.2:\n";
            node.push_back((*origin)[0] - 1);
            node.push_back((*origin)[1]);
            //std::cout << "Vecino: " << node[0] << "-" << node[1] << std::endl;
        }
    }
    if (orientation == 3) //Right
    {
        if ((*origin)[0] < (matrixLimit[0]-1))
        {
            //std::cout << "10.3:\n";
            node.push_back((*origin)[0] + 1);
            node.push_back((*origin)[1]);
            //std::cout << "Vecino: " << node[0] << "-" << node[1] << std::endl;
        }
    }
    return node;
}


/*
 * Dibuja piola 
 */
void autoRoute::route(vector<vector<int>>& routeOfp)//,vector<int>* origin, vector<vector<int>>* destiny)
{
    vector<int> tempPos;
    vector<int> direc;
    int contador;
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

        cout << "Test de Florian" <<endl;
        for(auto element : direc)
        {
            cout << element <<endl;
        }

        contador = 0;                   
        for(auto element: direc)            //  0 = no p
        {                                   //  1 = left p
            if(element = LEFT)              //  2 = right p
                contador += 1;              //  3 = left and right p
            if(element = RIGHT)             //  ...
                contador += 2;              //
            if(element = UP)                //
                contador += 4;              //
            if(element = DOWN)              //
                contador += 8;              //
        }

        cout << "Estoy aca 1" << endl;

        switch(contador)
        {
            case (15): 
                copyMatriz[coord[1]][coord[0]] = (char)('┼');
                cout << "Entro" <<endl;
                break;

            case (1):
                copyMatriz[coord[1]][coord[0]] = (char)('─');
                cout << "Entro" <<endl;
                break;
            
            case (2):
                copyMatriz[coord[1]][coord[0]] = (char)('─');
                cout << "Entro" <<endl;
                break;
            
            case (3):
                copyMatriz[coord[1]][coord[0]] = (char)('─');
                cout << "Entro" <<endl;
                break;

            case (4):
                copyMatriz[coord[1]][coord[0]] = (char)('│');
                cout << "Entro" <<endl;
                break;
            
            case (8):
                copyMatriz[coord[1]][coord[0]] = (char)('│');
                cout << "Entro" <<endl;
                break;
            
            case (12):
                copyMatriz[coord[1]][coord[0]] = (char)('│');
                cout << "Entro" <<endl;
                break;

            case (14):
                copyMatriz[coord[1]][coord[0]] = (char)('├');
                cout << "Entro" <<endl;
                break;

            case (10):
                copyMatriz[coord[1]][coord[0]] = (char)('┌');
                cout << "Entro" <<endl;
                break;

            case (6):
                copyMatriz[coord[1]][coord[0]] = (char)('└');
                cout << "Entro" <<endl;
                break;

            case (13):
                copyMatriz[coord[1]][coord[0]] = (char)('┤');
                cout << "Entro" <<endl;
                break;

            case (9):
                copyMatriz[coord[1]][coord[0]] = (char)('┐');
                cout << "Entro" <<endl;
                break;

            case (5):
                copyMatriz[coord[1]][coord[0]] = (char)('┘');
                cout << "Entro" <<endl;
                break;
            
            case (11):
                copyMatriz[coord[1]][coord[0]] = (char)('┬');
                cout << "Entro" <<endl;
                break;

            case (7):
                copyMatriz[coord[1]][coord[0]] = (char)('┴');
                cout << "Entro" <<endl;
                break;
            
            case (0):
                cout << "somos un desastre" << endl;
                break;
        }
        direc.clear();
        cout << "antes de salir" << endl;
    }
    cout << "Al salir" << endl;
}

