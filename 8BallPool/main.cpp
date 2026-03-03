#include<SFML/Graphics.hpp>
#include<iostream>
#include "Ball.h"
#include"ballFuncs.h"
using namespace std;
int main()
{
	unsigned int w = 1500;
	unsigned int h = 750;
	
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ w,h }), "8 Ball Pool");

	window->setFramerateLimit(60);
	Ball white(w / 3.0f, h / 2.0f, sf::Color::White);
//sf::Texture whiteTexture;
	/*whiteTexture = prepareTexture("BallTextures/fixed_texture_15.png");
	
white.shape.setTexture(&whiteTexture);
white = putTextureOnBall(white);
*/
	vector<Ball> balls;
	vector<sf::CircleShape> holes(6);
	makeHoles(holes);
	holes[0].setPosition({ 0.0f,0.0f });
	holes[1].setPosition({ w/2.0f,0.0f });
	holes[2].setPosition({ w*1.0f,0.0f });
	holes[3].setPosition({ 0.0f,h*1.0f });
	holes[4].setPosition({ w/2.0f,h*1.0f });
	holes[5].setPosition({ w*1.0f,h*1.0f });
	vector<bool> potted(15,false);
	int startX = w/3*2;
	int startY = h/2;
	int spacing = 88;
	int id = 1;
	bool pressed = false;
	for (int row = 0;row < 5;row++)
	{
		for (int col = 0;col <= row;col++)
		{
			int ballx = startX + row * (spacing-15);
			int bally = startY - (row * spacing / 2) + col * spacing;
			balls.push_back(Ball((float)ballx, (float)bally,{(uint8_t)ballx, (uint8_t)bally, (uint8_t)(ballx*bally)}));
			id++;
			if (id > 15)break;
		}

		if (id > 15)break;
	}
	sf::VertexArray line;
	line.setPrimitiveType(sf::PrimitiveType::Lines);
	line.resize(2);
	while (window->isOpen())
	{
		while (const std::optional event = window->pollEvent())
		{
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window->close();
				}
			}
		}
		//Render
		window->clear(sf::Color({ 9, 128, 13 }));
		for (int i = 0;i < 6;i++)
		{
			window->draw(holes[i]);
		}
			//collisions beteween white and other ball
		for (int i = 0;i < 15;i++)
		{
			if (potted[i])continue;
			if (isPotted(balls[i], holes))potted[i] = true;
		}
		for (int i = 0;i < 15;i++)
		{
			//balls[i].shape.setFillColor({ (uint8_t)(balls[i].shape.getPosition().x ), (uint8_t)(balls[i].shape.getPosition().y), (uint8_t)(25) });
			if (potted[i])continue;
			if (overlap(white, balls[i]))//white.shape.getGlobalBounds().findIntersection(balls[i].shape.getGlobalBounds()))
			{
				workCollision(white, balls[i]);
				workDynamicCollision(white, balls[i]);
			}
		}

			//	collisions between non-white balls
		for (int i = 0;i < 15;i++)
		{
			if (potted[i])continue;
			for (int j = 0;j < 15;j++)
			{
				if (i == j)continue;
				if (potted[j])continue;
				if (overlap(balls[i],balls[j]))
				{
					workCollision(balls[i], balls[j]);
					workDynamicCollision(balls[i], balls[j]);
				}

			}
	}
		
		
		for (int i = 0;i<15;i++)
		{
			if (potted[i])continue;
			moveBall(balls[i]);
			collideWithWall(balls[i], w, h);
			window->draw(balls[i].shape);
		}
		//Make line between white ball and cursor


		sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(*window);
		line = makeLine(line, white, mousePos);
		//white ball specifics
		sf::Vector2f whiteMove = line[1].position - line[0].position;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !pressed && abs(white.vel.x) == 0.0f && abs(white.vel.y) == 0.0f)
		{
			white.vel = whiteMove / 10.0f;
			//white.shape.setPosition(mousePos);
			pressed = true;
		}
		else if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) pressed = false;
		//ball movement
		moveBall(white);
	//white ball wall collision
		collideWithWall(white, w, h);
		window->draw(white.shape);
		if (abs(white.vel.x) + abs(white.vel.y) < 0.1f)window->draw(line);
		window->display();
	}
	delete window;
	return 0;
}