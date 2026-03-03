#pragma once
#include <SFML/Graphics.hpp>
class Ball
{
public:
	Ball(float x, float y, sf::Color color)
	{
		const float radius = 40.0f;
		this->shape = (sf::CircleShape)radius;
		this->shape.setOrigin(this->shape.getGeometricCenter());
		shape.setPosition({ x,y });
		shape.setFillColor(color);
		shape.setOutlineThickness(4.0f);
		shape.setOutlineColor(sf::Color::Black);
		this->vel = { 0.0f, 0.0f };
		this->acc = { 0.98f, 0.98f };
		this->mass = radius;
	}

	sf::CircleShape shape;
	sf::Vector2f vel;
	sf::Vector2f acc;
	float mass;


};
