#include "board.h"
#include <memory>
#include <iostream>
#include "candy.h"

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
    return {};
}

bool Board::dump(const std::string& output_path) const
{
    // Implement your code here
    return false;
}

bool Board::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}
