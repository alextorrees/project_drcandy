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
        std::cout << "GetCell i SetCell CORRECTE" << std::endl;
        return true;

    }
    else
    {
        std::cout << "GetCell i SetCell INCORRECTE" << std::endl;
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

bool testShouldExplodeCorrecteVertical()
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
        std::cout << "ShouldExplodeVertical CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeVertical INCORRECTE" << std::endl;
        return false;

    }
}

bool testShouldExplodeTrueHoritzontal()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 3, 2);
    b.setCell(&c2, 4, 2);
    b.setCell(&c3, 5, 2);

    if (b.shouldExplode(3, 2))
    {
        std::cout << "ShouldExplodeHoritzontal CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeHoritzontal INCORRECTE" << std::endl;
        return false;

    }
}

bool testShouldExplodeDiagonal()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 2, 2);
    b.setCell(&c2, 3, 3);
    b.setCell(&c3, 4, 4);

    if (b.shouldExplode(3, 3))
    {
        std::cout << "ShouldExplodeDiagonal CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeDiagonal INCORRECTE" << std::endl;
        return false;

    }
}

bool testShouldExplodeFalseHoritzontal()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 3, 2);
    b.setCell(&c2, 4, 2);
    b.setCell(&c3, 6, 2);
    if (!(b.shouldExplode(3, 2)))
    {
        std::cout << "ShouldExplodeFalseHoritzontal CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeVerticalFalseHoritzontal INCORRECTE" << std::endl;
        return false;

    }
}

bool testShouldExplodeFalseVertical()
{
    Board b(10, 10);

    Candy c1(CandyType::TYPE_ORANGE);
    Candy c2(CandyType::TYPE_ORANGE);
    Candy c3(CandyType::TYPE_ORANGE);

    b.setCell(&c1, 2, 3);
    b.setCell(&c2, 2, 4);
    b.setCell(&c3, 2, 7);

    if (!b.shouldExplode(2, 3))
    {
        std::cout << "ShouldExplodeFalseVertical CORRECTE" << std::endl;
        return true;
    }
    else
    {
        std::cout << "ShouldExplodeFalseVertical INCORRECTE" << std::endl;
        return false;

    }
}

bool testDumpAndLoad()
{
    Board b2(10, 10);
    Candy c(CandyType::TYPE_BLUE);
    Board b(10, 10);
    b.setCell(&c, 0, 0); /*Hem creat una variable "candy" del tipus "blue" i la hem posat en el (0,0) en el board b per  utilitzarla en aquest test.*/

    if (!b.dump(getDataDirPath() + "dump_board.txt")) /*Comprovació per saber si el "b" inicial 
                                                           es pot guarda bé en un fitxer adicional.*/
    {
        std::cout << "DumpandLoad INCORRECTE" << std::endl;
        return false;
    }

    if (!b2.load(getDataDirPath() + "dump_board.txt")) /*Comprovació per saber si el "b2" pot carregar bé la informació d'un fitxer adicional.*/
    {
        std::cout << "DumpandLoad INCORRECTE" << std::endl;
        return false;
    }

    if (b2.getCell(0, 0) == nullptr) /*Verifiquem la posicio (0,0) del tauler després d'haver carregat el fitxer.*/
    {
        std::cout << "DumpandLoad INCORRECTE" << std::endl;
        return false;
    }

    if (b2.getCell(0, 0)->getType() != c.getType()) /*Comprovem que ha guardat correctament la "candy" que es troba en la posició (0,0).*/
    {
        std::cout << "DumpandLoad INCORRECTE" << std::endl;
        return false;
    }

    std::cout << "DumpandLoad CORRECTE" << std::endl;/*Si no hi ha hagut cap error mostrem missatge per pantalla.*/
    std::filesystem::remove(getDataDirPath() + "dump_board.txt");/*Eliminem el fitxer que hem utilitzat per aquesta prova.*/
    return true;
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
        testShouldExplodeCorrecteVertical() &&
        testShouldExplodeTrueHoritzontal() &&
        testShouldExplodeDiagonal() &&
        testShouldExplodeFalseHoritzontal() &&
        testShouldExplodeFalseVertical() &&
        testDumpAndLoad(); /*Aquest return comprova tots els tests fets per comprovar si funciona cada funció aïllada, i retorna true si es compleixen tots.*/

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

