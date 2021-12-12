#include "Paddle.h"

Paddle::Paddle(int team, Vector2f initialPos, Vector2f size)
	:
	m_team(team),
	m_velocity({ 0.12f, 0.12f }),
	m_paddleRect(size)
{
	m_paddleRect.setPosition(initialPos);
	if (m_team == 0)
	{
		m_paddleRect.setFillColor(Color::Red);
	}
	else if (m_team == 1)
	{
		m_paddleRect.setFillColor(Color::Blue);
	}
	std::cout << "Constructing Paddle " << m_team << "\n";
}

/*
Paddle::~Paddle()
{
	std::cout << "Destroying Paddle " << m_team << "\n";
}
*/

void Paddle::Move()
{
	m_paddleRect.setPosition(m_paddleRect.getPosition() + m_velocity);
	std::cout << "Moving Paddle " << m_team << " to (" << m_paddleRect.getPosition().x << ", " << m_paddleRect.getPosition().y << ")" << "\n";
}

RectangleShape & Paddle::getPaddleRect()
{
	return m_paddleRect;
}
