#pragma once
#include <SFML/Graphics.hpp>

class FPSCounter
{
public:
	FPSCounter(sf::Font* font);
	FPSCounter(const int size = 15, const sf::Color fillColor = sf::Color::White, const float thickness = 0.f, const sf::Color outline = sf::Color::White, sf::Font* font = nullptr);

	template<typename ...Args>
	void setPosition(Args&&... args){m_text.setPosition(std::forward<Args>(args)...);}
	
	void setFont(sf::Font* font);
	void update();
	void draw(sf::RenderTarget& renderer);
private:
	sf::Text m_text;
	sf::Font m_font;
	sf::Clock m_delayTimer;
	sf::Clock m_fpsTimer;
	float m_fps = 0;
	int m_frameCount = 0;
};

