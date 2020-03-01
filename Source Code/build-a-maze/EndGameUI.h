#ifndef END_GAME_UI
#define END_GAME_UI

#include <SFML/Graphics.hpp>
#include <functional>
#include "Button.h"
#include "Cursor.h"

class Game;

class EndGameUI
{
public:
	EndGameUI(sf::View const & t_guiView);

	void update(Cursor const & t_cursor, Game* t_game, std::function<void(Game*)> t_restartFunc, std::function<void(Game*)> t_menuFunc);

	void draw(sf::RenderWindow & t_window) const;

	const bool isAnimating() const;
	void setAnimating(int t_roundNumber);

private:

	sf::Font m_font;
	sf::Text m_message;
	sf::Text m_roundNumberText;

	sf::RectangleShape m_background;

	const float m_SECONDS_TO_ANIMATE;

	bool m_animating;

	sf::Clock m_animationClock;

	sf::Texture m_buttonTexture;

	GUI::Button m_restartButton;
	GUI::Button m_menuButton;
};

#include "Game.h"

#endif // !END_GAME_UI