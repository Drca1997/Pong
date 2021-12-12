#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

class Paddle
{
	private:
		int m_team;
		RectangleShape m_paddleRect;
		Vector2f m_velocity;
	public:
		Paddle(int team, Vector2f initialPos, Vector2f size);
		void Move();
		RectangleShape& getPaddleRect();
};

