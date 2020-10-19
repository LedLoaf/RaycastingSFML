#include "FPSCounter.h"

FPSCounter::FPSCounter(const int size, const sf::Color fillColor, const float thickness, const sf::Color outline, sf::Font* font) {
    m_text.move(0, 0);
    m_text.setOutlineColor (outline);
    m_text.setFillColor(fillColor);
    m_text.setOutlineThickness  (thickness);
    m_text.setCharacterSize(size);
    m_text.setFont(*font);
}

FPSCounter::FPSCounter(sf::Font* font)
{
    m_text.move(0, 0);
    m_text.setOutlineColor (sf::Color::Black);
    m_text.setFillColor({255,255,255});
    m_text.setOutlineThickness  (2);
    m_text.setFont(*font);
    m_text.setCharacterSize(15);
}

void FPSCounter::setFont(sf::Font* font) {
    m_text.setFont(*font);
}

//updates the FPS variable
void FPSCounter::update()
{
    m_frameCount++;

    if (m_delayTimer.getElapsedTime().asSeconds() > 0.2f) {
        m_fps = m_frameCount / m_fpsTimer.restart().asSeconds();
        m_frameCount = 0;
        m_delayTimer.restart();
    }
}

//Draws the FPS display to the window
void FPSCounter::draw(sf::RenderTarget& renderer)
{
    m_text.setString("FPS " + std::to_string(static_cast<int>(m_fps)));
    renderer.draw(m_text);
}