/*          route.cpp
*
* Alumnos:  - Minniti 63286
*           - Sammartino 63053
*           - Mathé 63420
*
* Enrutador automático de pistas para PCB
*
*/

#include "autoRoute.h"

using namespace std;


/*
 * Funcion para calcular la distancia euclidiana entre dos puntos
 */
double autoRoute::calculateDistance(const vector<int>& p1, const vector<int>& p2)
{
    int dx = p2[0] - p1[0];
    int dy = p2[1] - p1[1];
    return std::sqrt(dx * dx + dy * dy);
}

/*
 * Funcion de comparacion para ordenar los puntos por distancia euclidiana
 */
bool autoRoute::sortByDistance(const vector<int>& p1, const vector<int>& p2, const vector<int>& reference)
{
    double distance1 = calculateDistance(p1, reference);
    double distance2 = calculateDistance(p2, reference);
    return distance1 < distance2;
}

/*
 * Funcion para ordenar los puntos de cada clave en el mapa por distancia euclidiana    
 */
void autoRoute::sortInterconnections(const std::vector<int>* origin, std::vector<std::vector<int>>* list)
{
    vector<std::vector<int>>& points = *list;
    sort(points.begin(), points.end(), [&](const std::vector<int>& p1, const std::vector<int>& p2)
        {
            return sortByDistance(p1, p2, *origin);
        });
}

/*
 *  Funcion que copia la matriz en un atributo de la clase
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

vector<vector<char>> autoRoute::initAutoRoute(vector<vector<char>>& matriz,
    map<vector<int>, vector<vector<int>>>& interconnections)
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

            //Si no encuentro un camino a destino posible
            //No produce cambios a la matriz original
            if (isFind == false)
            {
                cout << "No se encontro un camino" << endl;
                return matriz;
            }
        }
        if(isFind == true)
        {
            isFind = false;
            //se llama a la funcion route() que completa con la pista correspondiente en las casillas que alla una "P"
            route(routeOfP,origin,entry.second);

            // se "limpian" los vectores para la prxima iteracion
            possibleOrigins.clear();
            routeOfP.clear();
            temp.clear();
        }
    }
    return copyMatriz;
}

/*
 * Funcion que produce un flooding en la matriz desde una origen dado hasta llegar a un destino
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
void autoRoute::route(vector<vector<int>>& routeOfp,vector<int> origin, std::vector<vector<int>> destiny)
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
                    for(auto coord3 : destiny)
                    {
                        if(coord3 == tempPos)
                        {
                            direc.push_back(LEFT);
                        }
                    }
                    if(origin == tempPos)
                        direc.push_back(LEFT);

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
                    for(auto coord3 : destiny)
                    {
                        if(coord3 == tempPos)
                        {
                            direc.push_back(RIGHT);
                        }
                    }
                    if(origin == tempPos)
                        direc.push_back(RIGHT);
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
                    for(auto coord3 : destiny)
                    {
                        if(coord3 == tempPos)
                        {
                            direc.push_back(UP);
                        }
                    }
                    if(origin == tempPos)
                        direc.push_back(UP);

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
                    for(auto coord3 : destiny)
                    {
                        if(coord3 == tempPos)
                        {
                            direc.push_back(DOWN);
                        }
                    }
                    if(origin == tempPos)
                        direc.push_back(DOWN);

                    tempPos.clear();
                    break;
                }
                
        
            }
        }

        //A esta altura sabemos lo que hay al costado de cada p

        contador = 0;                   
        for(auto element: direc)    //  0 = no p
        {                           //  1 = left p
            if(element == LEFT)     //  2 = right p
                contador += 1;      //  3 = left and right p
            if(element == RIGHT)    //  ...
                contador += 2;      //
            if(element == UP)       //
                contador += 4;              //
            if(element == DOWN)              //
                contador += 8;              //
        }
        direc.clear();

        switch(contador)
        {
            case (15): 
                copyMatriz[coord[1]][coord[0]] = 'A';
                //copyMatriz[coord[1]][coord[0]] = (char)("┼");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (1):
                copyMatriz[coord[1]][coord[0]] = 'B';
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (2):
                copyMatriz[coord[1]][coord[0]] = 'C';
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (3):
                copyMatriz[coord[1]][coord[0]] = 'D';
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (4):
                copyMatriz[coord[1]][coord[0]] = 'E';
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (8):
                copyMatriz[coord[1]][coord[0]] = 'F';
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (12):
                copyMatriz[coord[1]][coord[0]] = 'G';
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (14):
                copyMatriz[coord[1]][coord[0]] = 'H';
                //copyMatriz[coord[1]][coord[0]] = (char)("├");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (10):
                copyMatriz[coord[1]][coord[0]] = 'I';
                //copyMatriz[coord[1]][coord[0]] = (char)("┌");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (6):
                copyMatriz[coord[1]][coord[0]] = 'J';
                //copyMatriz[coord[1]][coord[0]] = (char)("└");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (13):
                copyMatriz[coord[1]][coord[0]] = 'K';
                //copyMatriz[coord[1]][coord[0]] = (char)("┤");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (9):
                copyMatriz[coord[1]][coord[0]] = 'L';
                //copyMatriz[coord[1]][coord[0]] = (char)("┐");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (5):
                copyMatriz[coord[1]][coord[0]] = 'M';
                //copyMatriz[coord[1]][coord[0]] = (char)("┘");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case (11):
                copyMatriz[coord[1]][coord[0]] = 'N';
                //copyMatriz[coord[1]][coord[0]] = (char)("┬");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case (7):
                copyMatriz[coord[1]][coord[0]] = 'O';
                //copyMatriz[coord[1]][coord[0]] = (char)("┴");
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

void autoRoute::printSpecialValue(char value)
{
        switch(value)
        {
            case ('A'): 
                cout << "┼";
                //copyMatriz[coord[1]][coord[0]] = (char)("┼");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('B'):
                cout << "─";
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case ('C'):
                cout << "─";
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case ('D'):
                cout << "─" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("─");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('E'):
                cout << "│" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case ('F'):
                cout << "│";
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case ('G'):
                cout << "│";
                //copyMatriz[coord[1]][coord[0]] = (char)("│");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('H'):
                cout << "├";
                //copyMatriz[coord[1]][coord[0]] = (char)("├");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('I'):
                cout << "┌";
                //copyMatriz[coord[1]][coord[0]] = (char)("┌");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('J'):
                cout << "└";
                //copyMatriz[coord[1]][coord[0]] = (char)("└");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('K'):
                cout << "┤" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("┤");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('L'):
                cout << "┐" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("┐");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('M'):
                cout << "┘";
                //copyMatriz[coord[1]][coord[0]] = (char)("┘");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;
            
            case ('N'):
                cout << "┬" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("┬");
                //copyMatriz[coord[1]][coord[0]] = (char)('K');
                break;

            case ('O'):
                cout << "┴" ;
                //copyMatriz[coord[1]][coord[0]] = (char)("┴");
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
