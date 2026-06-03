#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"
#include <cstdlib>
#include "controller.h"
#include "util.h"

Game::Game()
{
    // Implement your code here
    m_frameCounter = 0;
    m_gameOver = false;
    m_score = 0;
    m_pause = false;

    m_blockX = m_board.getWidth() / 2;
    m_blockY = 0;
    for (int i = 0; i < 3; i++)
    {
        m_blockCandies[i] = nullptr;
    }
    createNewBlock();
}

Game::~Game()
{
    // Implement your code here
    for (int i = 0; i < 3; i++)
    {
        delete m_blockCandies[i];

        m_blockCandies[i] = nullptr;
    }
}

void Game::update(const Controller& controller)
{
    // Implement your code here

    if (m_gameOver)
    {
        filesystem::remove(getDataDirPath() + "/save.txt");
        return;
    }

    if (controller.isUpPressed())
    {
        m_pause = !m_pause;
    }

    if (m_pause)
    {
        return;
    }
    

    if (controller.isLeftPressed() && canMoveBlock(-1))
    {
        m_blockX--;
    }
    if (controller.isRightPressed() && canMoveBlock(1))
    {
        m_blockX++;
    }   
   
    if (controller.isKey1Pressed())
    {
        rotateBlock();
    }
    if (controller.isKey2Pressed())
    {
        dump(getDataDirPath() + "/save.txt");
    }    
    if (controller.isKey3Pressed())
    {
       load(getDataDirPath() + "/save.txt");
    }

    bool tieneQueCaer = false;

    m_frameCounter++;

    if (m_frameCounter >= 60)
    {
        tieneQueCaer = true;
        m_frameCounter = 0;
    }

    if (controller.isDownPressed())
    {
        tieneQueCaer = true;
    }

    if (tieneQueCaer)
    {
        if (canFall())
        {
            m_blockY++;
        }
        else
        {
            landBlock();
            vector <Candy*> explotades =m_board.explodeAndDrop();
            m_score += explotades.size() * 10;
            for (int i = 0; i < explotades.size(); i++)
            {
                delete explotades[i];
            }
            createNewBlock();

            if (!canFall())
            {
                m_gameOver = true;
            }
        }
    }
}

void Game::render(GraphicManager& graphics)
{
    const int board_padding = 3;
    int boardX = CANDY_IMAGE_WIDTH * board_padding;
    int boardY = CANDY_IMAGE_HEIGHT * board_padding;
    graphics.drawRectangle(
        boardX,
        boardY,
        CANDY_IMAGE_HEIGHT*10,
        CANDY_IMAGE_WIDTH*10, 5, 150, 150, 150);
    for (int x = 0; x < m_board.getWidth(); x++)
    {
        for (int y = 0; y < m_board.getHeight(); y++)
        {
            Candy *candy = m_board.getCell(x, y);
            if (candy != nullptr)
            {
                int celaX = boardX + x*CANDY_IMAGE_WIDTH;
                int celaY = boardY + y*CANDY_IMAGE_HEIGHT;
                graphics.drawImage(candy->getResourceName(), celaX, celaY);
            }
        }
    }
    if (!m_gameOver)
    {
        for (int i = 0; i < 3; i++)
        {
            int x = m_blockX;
            int y = m_blockY + i;
            if (m_blockCandies[i] != nullptr && y >= 0 && y < m_board.getHeight())
            {
                int celaX = boardX + x*CANDY_IMAGE_WIDTH;
                int celaY = boardY + y * CANDY_IMAGE_HEIGHT;
                graphics.drawImage(m_blockCandies[i]->getResourceName(), celaX, celaY);
            }
        }
    }
    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [drawtext]
    graphics.drawText("Movement: [Down] [Left] [Right]  --  "
        "Buttons: [Q] [W] [E]  --  Exit [ESC] -- Pause [UP]",
        25, 700, 15, 100, 100, 100);
    graphics.drawText("Score: " + to_string(m_score), 250, 10, 50, 125, 200, 125);
    //meterle boton de pausa.
    if (m_gameOver)
    {
        graphics.drawText("GAME OVER", 200, 120, 55,0, 0, 0);
    }
    if (m_pause)
    {
        graphics.drawText("PAUSE", 275, 120, 55, 0, 0, 0);

    }

}

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const std::string& output_path) const
{
    // Implement your code here
    bool correcte = false;
    ofstream fitxer(output_path);
    if (fitxer.is_open())
    {
        fitxer << m_board.getWidth() << " " << m_board.getHeight() << endl;

        for (int x = 0; x < m_board.getWidth(); x++)
        {
            for (int y = 0; y < m_board.getHeight(); y++)
            {
                Candy* candy = m_board.getCell(x, y);
                if (candy != nullptr)
                {
                    fitxer << static_cast<int>(candy->getType()) << " ";
                }
                else
                {
                    fitxer << -1 << " ";
                }
            }
            fitxer << endl;
        }
        fitxer << m_blockX << " " << m_blockY << endl;
        for (int i = 0; i < 3; i++)
        {
            if (m_blockCandies[i] != nullptr)
            {
                fitxer << static_cast<int>(m_blockCandies[i]->getType()) << " ";
            }
            else
            {
                fitxer << -1 << " ";
            }
        }
        fitxer << endl;
        fitxer << m_frameCounter << " " << m_gameOver <<  " " << m_score << endl;
        fitxer.close();
        correcte = true;
    }
    return correcte;
}

bool Game::load(const std::string& input_path)
{
    bool correcte = false;
    // Implement your code here
    ifstream fitxer(input_path);
    if (fitxer.is_open())
    {
        int width;
        int height;
        fitxer >> width >> height;
        if (width != m_board.getWidth() || height != m_board.getHeight())
        {
            return false;
        }
        for (int x = 0; x < m_board.getWidth(); x++)
        {
            for (int y = 0; y < m_board.getHeight(); y++)
            {
                int tipusCandy;
                fitxer >> tipusCandy;
                if (tipusCandy == -1)
                {
                    m_board.setCell(nullptr, x, y);
                }
                else
                {
                    Candy* candy = new Candy(static_cast<CandyType>(tipusCandy));
                    m_board.setCell(candy, x, y);
                }
            }
        }

        fitxer >> m_blockX >> m_blockY;

        for (int i = 0; i < 3; i++)
        {
            delete m_blockCandies[i];
            m_blockCandies[i] = nullptr;
        }

        for (int i = 0; i < 3; i++)
        {
            int tipusCandy;
            fitxer >> tipusCandy;
            if (tipusCandy == -1)
            {
                m_blockCandies[i] = nullptr;
            }
            else
            {
                m_blockCandies[i] = new Candy(static_cast<CandyType>(tipusCandy));
            }
        }
        fitxer >> m_frameCounter >> m_gameOver >> m_score;
        fitxer.close();
        correcte = true;
    }

    return correcte;
}

bool Game::operator==(const Game& other) const
{
    // Implement your code here

    if (m_board.getWidth() != other.m_board.getWidth() || m_board.getHeight() != other.m_board.getHeight())
    {
        return false;
    }
    
    for (int x = 0; x < m_board.getWidth(); x++)
    {
        for (int y = 0; y < m_board.getHeight(); y++)
        {
            Candy* c1 = m_board.getCell(x, y);
            Candy* c2 = other.m_board.getCell(x, y);
            if ((c1 == nullptr || c2 == nullptr) && (c1 != c2))
            {
                return false;
            }
            else if(c1 != nullptr && c2 !=nullptr)
            {
                if (c1->getType() != c2->getType())
                {
                    return false;
                }
            }

        }
    }

    if (m_blockX != other.m_blockX || m_blockY != other.m_blockY)
    {
        return false;
    }


    for (int i = 0; i < 3; i++)
    {
        Candy* c1 = m_blockCandies[i];
        Candy* c2 = other.m_blockCandies[i];
        if ((c1 == nullptr || c2 == nullptr ) && (c1 != c2))
        {
            return false;
        }
        else if (c1 != nullptr && c2 != nullptr)
        {
            if (c1->getType() != c2->getType())
            {
                return false;
            }
        }
    }

    if (m_frameCounter != other.m_frameCounter)
    {
        return false;
    }

    if (m_gameOver != other.m_gameOver)
    {
        return false;
    }

    if (m_score != other.m_score)
    {
        return false;
    }
    return true;
}

void Game::rotateBlock()
{
    Candy* aux = m_blockCandies[2];

    m_blockCandies[2] = m_blockCandies[1];
    m_blockCandies[1] = m_blockCandies[0];
    m_blockCandies[0] = aux;
}
bool Game::canFall() const
{
    int nextY = m_blockY + 3;

    if (nextY >= m_board.getHeight())
    {
        return false;
    }

    if (m_board.getCell(m_blockX, nextY) != nullptr)
    {
        return false;
    }

    return true;
}
void Game::landBlock()
{
    for (int i = 0; i < 3; i++)
    {
        int y = m_blockY + i;

        if (y >= 0 && y < m_board.getHeight())
        {
            m_board.setCell(m_blockCandies[i], m_blockX, y);
            m_blockCandies[i] = nullptr;
        }
        else if(y<0)
        {
            m_gameOver = true;
        }
    }
}
void Game::createNewBlock()
{
   
    static bool randomInici = false;

    if (!randomInici)
    {
        srand(time(nullptr));
        randomInici = true;
    }
   
    m_blockX = m_board.getWidth() / 2;
    m_blockY = 0;

    m_blockCandies[0] = new Candy(static_cast<CandyType>(rand() % static_cast<int>(CandyType::COUNT)));
    m_blockCandies[1] = new Candy(static_cast<CandyType>(rand() % static_cast<int>(CandyType::COUNT)));
    m_blockCandies[2] = new Candy(static_cast<CandyType>(rand() % static_cast<int>(CandyType::COUNT)));
}

bool Game::canMoveBlock(int dx) const
{
    int newX = m_blockX + dx;

    if (newX < 0 || newX >= m_board.getWidth())
    {
        return false;
    }

    for (int i = 0; i < 3; i++)
    {
        int y = m_blockY + i;

        if (y >= 0 && y < m_board.getHeight())
        {
            if (m_board.getCell(newX, y) != nullptr)
            {
                return false;
            }
        }
    }

    return true;
}