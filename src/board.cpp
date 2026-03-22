#include "board.h"
#include <memory>
#include <iostream>
#include "candy.h"
#include <fstream>

Board::Board(int width, int height)
{
    // Implement your code here
    m_width = width;
    m_height = height;
    /*Aqui inicialitzem la matriu del tauler*/
    m_cells.resize(m_height, vector<Candy*>(m_width, nullptr));
}

Board::~Board()
{
}


Candy* Board::getCell(int x, int y) const
{
    if ((x >= 0 && x < m_width) && (y >= 0 && y < m_height))
    {
        return m_cells[x][y];
    }
    else
    {
        return nullptr;
    }
    /* Aqui el que farem es comprobar si la altura i la anchura que donem com a parametras
        pertanyen a la matriu, i si pertanyen returnem el valor que hi ha. */
}

void Board::setCell(Candy* candy, int x, int y)
{
    if ((x >= 0 && x < m_width) && (y >= 0 && y < m_height))
    {
        m_cells[x][y] = candy;
    }
    /* Com ja tenim la matriu cel·les, ara comprovem si els "x" e "y" son valors que pertanyen al tauler,
        i si compleix, doncs al tauler als valors "x" e "y" li donem el valor de candy */
}

int Board::getWidth() const
{
    return m_width;
}


int Board::getHeight() const
{
    return m_height;
}

bool Board::shouldExplode(int x, int y) const
{
    int total = 1;
    Candy* caramel = getCell(x, y);
    if (caramel == nullptr)
    {
        return false;
    }
    else
    {
        CandyType tipusCaramel= caramel->getType();

        //Horitzontal 
        int i = x - 1;
        while (i >= 0 && getCell(i, y) != nullptr && getCell(i, y)->getType() == tipusCaramel)
        {
            i--;
            total++;
        }
        i = x + 1;
        while (i <m_width && getCell(i, y) != nullptr && getCell(i, y)->getType() == tipusCaramel)
        {
            i++;
            total++;
        }
        if (total >= SHORTEST_EXPLOSION_LINE)
        {
            return true;
        }

        //Vertical
        total = 1;
        int j = y - 1;
        while (j >= 0 && getCell(x, j) != nullptr && getCell(x, j)->getType() == tipusCaramel)
        {
            j--;
            total++;
        }
        j = y + 1;
        while (j <m_height && getCell(x, j) != nullptr && getCell(x, j)->getType() == tipusCaramel)
        {
            j++;
            total++;
        }
        if (total >= SHORTEST_EXPLOSION_LINE)
        {
            return true;
        }

        //Diagonal / 
        total = 1;
        i = x - 1;
        j = y - 1;
        while (i >= 0 &&  j>=0 && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipusCaramel)
        {
            i--;
            j--;
            total++;
        }
        i = x + 1;
        j = y + 1;
        while (i < m_width && j < m_height && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipusCaramel)
        {
            i++;
            j++;
            total++;
        }
        if (total >= SHORTEST_EXPLOSION_LINE)
        {
            return true;
        }

        //Diagonal contraria
        total = 1;
        i = x + 1;
        j = y - 1;
        while (i < m_width && j >= 0 && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipusCaramel)
        {
            i++;
            j--;
            total++;
        }
        i = x - 1;
        j = y + 1;
        while (i >= 0 && j <m_height && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipusCaramel)
        {
            i--;
            j++;
            total++;
        }
        if (total >= SHORTEST_EXPLOSION_LINE)
        {
            return true;
        }
    }  
    
    return false;
}           

std::vector<Candy*> Board::explodeAndDrop()
{
    // Implement your code here
    std::vector<std::pair<int, int>> perExplotar; /*Vector que guatrda dues variables enteres, la pos x i la pos y*/
    std::vector<Candy*> explotats; /*Vector dels explotats, del tipus Candy*/
    bool tocaExplosio = true;
    
    while (tocaExplosio)
    {
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                if (!shouldExplode(x, y))
                {
                    tocaExplosio = false;
                }
                else
                {
                    perExplotar.push_back({x,y});/*Afegim les cel·les quan la funció souldexplode es true,
                                                     al vector creat previament amb el nom de perExplotar per 
                                                     emmagatzemar les posicions que haurien d'explotar*/

                    explotats.push_back(getCell(x, y)); /*Aqui afegim la candy que hi ha a la posicio x,y 
                                                           al vector de explotats*/

                    setCell(nullptr, x, y); /*Eliminem la candy que hi ha a la posicio x,y i posem nullptr*/
                }
            }
        }
    }

    int filaBuida = m_height - 1;
    for (int x = 0; x < m_width; x++) /*Bucle per recorre cada columna*/
    {
        for (int y = m_height - 1; y >= 0; y--)/*Bucle per recorre cada fila desde d'alt fins abaix*/
        {
            if (getCell(x, y) != nullptr)
            {
                setCell(getCell(x, y), x, filaBuida);

                if (filaBuida != y)
                {
                    setCell(nullptr, x, y);
                }
                filaBuida--;
            }
        }
        while (filaBuida >= 0)
        {
            setCell(nullptr, x, filaBuida);
            filaBuida--;
        }
    }
   
    return explotats;
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    std::ofstream fitxer(output_path);
    if (!fitxer.is_open())
    {
        return false;
    }
    fitxer << m_width << " " << m_height << "\n";

    for (int x = 0; x < m_width; x++)
    {   
        for (int y = 0; y < m_height; y++)
        {
            Candy* c = getCell(x, y);
            if (c == nullptr)
            {
                fitxer << -1;
            }
            else
            {
                fitxer << static_cast<int>(c->getType());
                
            }

            if (x < m_width - 1)
            {
                fitxer << " ";
            }
        }
        fitxer << "\n";
    }   
    fitxer.close();

    return true;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    std::ifstream fitxer(input_path);

    if (!fitxer.is_open())
    {
        return false;
    }

    int width, height;
    fitxer >> width >> height;

    if (!fitxer)
    {
        return false;
    }

    m_width = width;
    m_height = height;
    return false;
    m_cells.resize(m_height, std::vector<Candy*>(m_width, nullptr));

    for (int x = 0; x < m_width; x++)
    {
        for (int y = 0; y < m_height; y++)
        {
            int valor;
            fitxer >> valor;

            if (!fitxer)
            {
                return false;
            }

            if (valor == -1)
            {
                setCell(nullptr, x, y);
            }
            else
            {
                CandyType tipus = static_cast<CandyType>(valor);

                Candy* novaCandy = new Candy(tipus);

                setCell(novaCandy, x, y);
            }
        }
    }

    fitxer.close();
    return true;
}
