#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"
#include <cstdlib>

Game::Game()
{
    // Implement your code here
    m_frameCounter = 0;
    m_gameOver = false;

    m_blockX = m_board.getWidth() / 2;
    m_blockY = 0;

    createNewBlock();
}

Game::~Game()
{
    // Implement your code here
}

void Game::update(const Controller& controller)
{
    // Implement your code here

    if (m_gameOver)
        return;

    if (controller.isLeftPressed() && canMoveBlock(-1))
    {
        m_blockX--;
    }
    else if (controller.isRightPressed() && canMoveBlock(1))
    {
        m_blockX++;
    }
    else if (controller.isKey1Pressed())
    {
        rotateBlock();
    }
    else if (controller.isKey2Pressed())
    {
        dump("data/save.txt");
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
            m_board.explodeAndDrop();
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
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
        "Buttons: [Q] [W] [E]  --  Exit [ESC]",
        25, 700, 20, 100, 100, 100);
    graphics.drawText("Score: ", 450, 10, 70, 125, 200, 125);



    if (m_gameOver)
    {
        graphics.drawText("GAME OVER", 230, 350, 60, 0, 0, 0);
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
    return false;
}

bool Game::load(const std::string& input_path)
{
    // Implement your code here
    return false;
}

bool Game::operator==(const Game& other) const
{
    // Implement your code here
    return false;
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
        }
    }
}
void Game::createNewBlock()
{
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