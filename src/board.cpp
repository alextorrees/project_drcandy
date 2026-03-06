#include "board.h"
#include <memory>
#include <iostream>

Board::Board(int width, int height)
{
    // Implement your code here
    m_width = width;
    m_height = height;
    /*Aqui inicialitzem la matriu del tauler*/
    m_cells.resize(m_height, vector<Candy*>(m_width, nullptr))
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
    // Implement your code here
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
