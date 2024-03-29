/*          EDA_Level_5.cpp
*
* Alumnos:  - Minniti 63286
*           - Sammartino 63053
*           - Mathé 63420
*
* Enrutador automático de pistas para PCB
*
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <windows.h>

#include "autoRoute.h"

#define FILE "C:/Codes/EDA/TP5/NEW_EDA_Level_5/DC_DC_Buck_Converter.txt"

autoRoute router = autoRoute();

// Código para representar las conexiones de un nodo a otros nodos con caracteres Unicode
int processFile(const string& nameFile,
    vector<std::vector<char>>& matriz,
    map<std::vector<int>, std::vector<std::vector<int>>>& interconnections)
{
    std::ifstream archivo(nameFile);

    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo: " << nameFile << std::endl;
        return 1;
    }

    matriz.clear();
    interconnections.clear();

    std::string linea;
    bool readTable = false;
    bool firstOfGrup = true;
    int conter = 0;

    std::vector<int> key, point;

    while (std::getline(archivo, linea))
    {
        if (!readTable)
        {
            if (linea.empty())
            {
                readTable = true;
                continue;
            }

            std::vector<char> fila;

            for (char c : linea)
            {
                fila.push_back(c);
            }

            matriz.push_back(fila);
        }
        else
        {
            int X, Y;
            if (linea.empty())
            {
                firstOfGrup = true;
                key.clear();
                continue;
            }
            else
            {
                sscanf_s(linea.c_str(), "%d,%d", &X, &Y);
                if (firstOfGrup)
                {
                    key.push_back(X);
                    key.push_back(Y);
                    firstOfGrup = false;
                }
                else
                {
                    point.push_back(X);
                    point.push_back(Y);
                    interconnections[key].push_back(point);
                    point.clear();
                }
            }
        }
    }
    archivo.close();
    return 0;
}

/*
 * Funcion encargada de imprimir matriz final
*/
void printPCB(vector<vector<char>>& matriz,
    map<vector<int>, vector<vector<int>>>& interconnections)
{
    // Imprimir los valores de la matriz
    std::cout << "Matriz:\n";
    for (const auto& row : matriz)
    {
        for (const auto& value : row)
        {
            if(value >= 'A')
            {
                //Se Evaluan caracteres especiales
                router.printSpecialValue(value);
                std::cout << ' ';
            }
            else
                std::cout << value << ' ';
        }
        std::cout << '\n';
    }

    // Imprimir los valores de las interconexiones
    std::cout << "\nInterconexiones:\n";
    for (const auto& entry : interconnections)
    {
        const vector<int>& pin = entry.first;
        const vector<std::vector<int>>& connections = entry.second;

        cout << "Pin: (" << pin[0] << ", " << pin[1] << ")\n";
        cout << "Conexiones:\n";
        for (const auto& connection : connections)
        {
            std::cout << "(" << connection[0] << ", " << connection[1] << ")\n";
        }
        std::cout << '\n';
    }
}

int main()
{
    // Establecer el código de página a UTF-8
    SetConsoleOutputCP(CP_UTF8);

    vector<vector<char>> matriz;
    map<vector<int>, vector<vector<int>>> interconnections;

    if( processFile(FILE, matriz, interconnections))
        return 1;

    matriz = router.initAutoRoute(matriz, interconnections);

    printPCB(matriz, interconnections);

    return 0;
}
