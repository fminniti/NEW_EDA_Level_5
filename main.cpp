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
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <windows.h>

#include "autoRoute.h"

// Código para representar las conexiones de un nodo a otros nodos con caracteres Unicode

string unicodeBlockChars[] = {
    " ", "╵", "╶", "└", "╷", "│", "┌", "├", 
    "╴", "┘", "─", "┴", "┐", "┤", "┬", "┼",
};

string getBlockString(bool up, bool right, bool down, bool left)
{
    int index = (up << 0) | (right << 1) | (down << 2) | (left << 3);
    return unicodeBlockChars[index];
}

void processFile(const std::string& nameFile,
    std::vector<std::vector<char>>& matriz,
    std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections)
{
    std::ifstream archivo(nameFile);

    if (!archivo) {
        std::cerr << "No se pudo abrir el archivo: " << nameFile << std::endl;
        return;
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
}

void printPCB(std::vector<std::vector<char>>& matriz,
    std::map<std::vector<int>, std::vector<std::vector<int>>>& interconnections)
{
    // Imprimir los valores de la matriz
    std::cout << "Matriz:\n";
    for (const auto& row : matriz)
    {
        for (const auto& value : row)
        {
            std::cout << value << ' ';
        }
        std::cout << '\n';
    }

    // Imprimir los valores de las interconexiones
    std::cout << "\nInterconexiones:\n";
    for (const auto& entry : interconnections)
    {
        const std::vector<int>& pin = entry.first;
        const std::vector<std::vector<int>>& connections = entry.second;

        std::cout << "Pin: (" << pin[0] << ", " << pin[1] << ")\n";
        std::cout << "Conexiones:\n";
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

    // Imprimir caracteres Unicode
    std::cout << getBlockString(true, false, true, true)<< std::endl;  // Carácter: ━

    std::vector<std::vector<char>> matriz;
    std::map<std::vector<int>, std::vector<std::vector<int>>> interconnections;

    autoRoute router = autoRoute();

    processFile("C:/Codes/EDA/TP5/NEW_EDA_Level_5/DC_DC_Buck_Converter.txt", matriz, interconnections);

    matriz = router.initAutoRoute(matriz, interconnections);

    printPCB(matriz, interconnections);

    return 0;
}
