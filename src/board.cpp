#include "board.h"
#include <memory>
#include <iostream>
#include "candy.h"
#include <fstream>

/// Constructor del tauler.
/// Inicialitza l'amplada i l'alçada i crea la matriu de cel·les buides.
Board::Board(int width, int height)
{
    m_width = width;
    m_height = height;
    ///Inicialitzem la matriu del tauler
    m_cells.resize(m_height, vector<Candy*>(m_width, nullptr));
}

/// Destructor del tauler.
/// En aquest cas no allibera memòria, perquè depèn de com es gestioni
/// la propietat dels Candys en el projecte.
Board::~Board()
{
}

/// Retorna el punter al caramel que hi ha a la posició (x,y).
/// Si la posició està fora del tauler retorna nullptr.
Candy* Board::getCell(int x, int y) const
{
    if ((x >= 0 && x < m_width) && (y >= 0 && y < m_height))
    {
        return m_cells[y][x];
    }
    else
    {
        return nullptr;
    }
}

/// Assigna un caramel a la posició (x,y).
/// Si les coordenades no són vàlides no fa res.
void Board::setCell(Candy* candy, int x, int y)
{
    if ((x >= 0 && x < m_width) && (y >= 0 && y < m_height))
    {
        m_cells[y][x] = candy;
    }
}

/// Retorna l'amplada del tauler.
int Board::getWidth() const
{
    return m_width;
}

/// Retorna l'alçada del tauler.
int Board::getHeight() const
{
    return m_height;
}

/// Comprova si el caramel de la posició (x,y) ha d'explotar.
/// Un caramel explota si forma una línia horitzontal, vertical
/// o diagonal de com a mínim SHORTEST_EXPLOSION_LINE caramels del mateix tipus.
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

        /// Comprovació horitzontal
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

        /// Comprovació vertical
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

        /// Comprovació diagonal principal (\)
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

        /// Comprovació diagonal secundària (/)
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

/// Fa explotar tots els caramels que compleixin la condició
/// i després fa caure els caramels superiors per omplir els buits.
/// Es repeteix fins que ja no hi hagi més explosions possibles.
std::vector<Candy*> Board::explodeAndDrop()
{
    /// Vector dels caramels explotats.
    std::vector<Candy*> explotats;
    bool tocaExplosio = true;

    /// Continuem mentre encara hi hagi explosions.
    while (tocaExplosio)
    {
        tocaExplosio = false;

        /// Vector que guarda les posicions (x,y) dels caramels que han d'explotar.
        std::vector<std::pair<int, int>> perExplotar;

        /// Busquem totes les cel·les que han d'explotar.
        for (int x = 0; x < m_width; x++)
        {
            for (int y = 0; y < m_height; y++)
            {
                if (shouldExplode(x, y))
                {
                    /// Afegim la posició al vector de caramels que han d'explotar.
                    perExplotar.push_back({ x, y });
                    tocaExplosio = true;
                }
            }
        }

        /// Fem explotar els caramels trobats.
        for (const auto& pos : perExplotar)
        {
            int x = pos.first;
            int y = pos.second;

            /// Afegim el caramel al vector d'explotats.
            explotats.push_back(getCell(x, y));

            /// Eliminem el caramel del tauler.
            setCell(nullptr, x, y);
        }

        /// Fem caure els caramels columna per columna.
        for (int x = 0; x < m_width; x++)
        {
            int filaBuida = m_height - 1;

            for (int y = m_height - 1; y >= 0; y--)
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
    }

    return explotats;
}
/// Guarda l'estat del tauler en un fitxer.
/// Primer guarda les dimensions i després el contingut.
/// Les cel·les buides es guarden amb -1.
bool Board::dump(const std::string& output_path) const
{
    std::ofstream fitxer(output_path);
    if (!fitxer.is_open())
    {
        return false;
    }
    fitxer << m_width << " " << m_height << "\n";

    for (int y = 0; y < m_height; y++)
    {   
        for (int x = 0; x < m_width; x++)
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

/// Carrega l'estat del tauler des d'un fitxer generat amb dump().
/// Si la càrrega és correcta substitueix l'estat actual.
bool Board::load(const std::string& input_path)
{
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

    m_cells.clear();
    m_cells.resize(m_height, std::vector<Candy*>(m_width, nullptr));

    for (int y = 0; y < m_height; y++)
    {
        for (int x = 0; x < m_width; x++)
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