#define _HAS_STD_BYTE 0
#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"
#include <iostream>
bool testGetCellandSetCell()
{
    Board b(10, 10);
    Candy c(CandyType::TYPE_ORANGE);

    b.setCell(&c, 0, 0);

    if (b.getCell(0, 0) == &c)
    {
        std::cout << "Get i Set CORRECTE" << std::endl;
        return true;

    }
    else
    {
        std::cout << "Get i Set INCORRECTE" << std::endl;
        return false;
    }
}

bool testGetWidth()
{
    Board b(10, 10);

    if (b.getWidth() == 10)
    {
        std::cout << "GetWidth CORRECTE" << std::endl;
        return true;

    }
    else
    {
        std::cout << "GetWidth INCORRECTE" << std::endl;
        return false;

    }
}

bool testGetHeight()
{
    Board b(10, 10);

    if (b.getHeight() == 10)
    {
        std::cout << "GetHeight CORRECTE" << std::endl;
        return true;

    }
    else
    {
        std::cout << "GetHeight INCORRECTE" << std::endl;
        return false;

    }
}

bool testShouldExplodeCorrecte()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 2, 3);
    b.setCell(&c2, 2, 4);
    b.setCell(&c3, 2, 5);

    if (b.shouldExplode(2, 3))
    {
        std::cout << "ShouldExplodeTrue CORRECTE" << std::endl;
        return true;

    }
    else
    {
        std::cout << "ShouldExplodeTrue INCORRECTE" << std::endl;
        return false;

    }
}


bool testShouldExplodeIncorrecte()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 2, 3);
    b.setCell(&c2, 2, 4);
    b.setCell(&c3, 2, 7);

    if (b.shouldExplode(2, 3))
    {
        std::cout << "ShouldExplodeFalse CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeFalse INCORRECTE" << std::endl;
        return false;

    }
}
bool test()
{
    // Test board 2D container
    Candy c(CandyType::TYPE_ORANGE);
    Board b(10, 10);
    b.setCell(&c, 0, 0);
    if (b.getCell(0, 0) != &c)
    {
        return false;
    }
    
     return testGetCellandSetCell() &&
            testGetWidth() &&
            testGetHeight() &&
            testShouldExplodeCorrecte() &&
            testShouldExplodeIncorrecte();
   


    // Dump and load board
    {
        Board b2(10, 10);
        if (!b.dump(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (!b2.load(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (b2.getCell(0, 0)->getType() != c.getType())
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_board.txt");
    }

    // Dump and load game
    {
        Game g;
        Controller cont;
        g.update(cont);
        if (!g.dump(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        Game g2;
        if (!g2.load(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        if (!(g == g2))
        {
            return false;
        }
        std::filesystem::remove(getDataDirPath() + "dump_game.txt");
    }

    return true;
}

