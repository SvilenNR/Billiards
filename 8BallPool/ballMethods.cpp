#include<SFML/Graphics.hpp>
#include<iostream>
#include "Ball.h"
#include "BallFuncs.h"
using namespace std;
/*Ball putTextureOnBall(Ball& ball)
{
	sf::Texture txt = *ball.shape.getTexture();
	sf::Vector2u textsize = txt.getSize();
	int tx = textsize.x;
	int ty = textsize.y;
	ball.shape.setTextureRect({ {tx / 3 * 2, 50},{(int)ball.shape.getRadius()*2,(int)ball.shape.getRadius()*2}});
	return ball;
}
sf::Texture prepareTexture(string path)
{
	sf::Texture txt;
	if (!txt.loadFromFile(path))
	{
		cerr << "error loading white ball texture" << endl;
	}
	txt.setRepeated(true);
	txt.setSmooth(false);
	return txt;
}*/
void collideWithWall(Ball& ball, unsigned int w, unsigned int h)
{
	if (ball.shape.getPosition().x <= ball.shape.getRadius() || ball.shape.getPosition().x >= w - ball.shape.getRadius())
	{
		if (ball.vel.x < 0)ball.vel.x += abs(ball.vel.x) * 2;
		else ball.vel.x = -ball.vel.x;
	}
	if (ball.shape.getPosition().y <= ball.shape.getRadius() || ball.shape.getPosition().y >= h - ball.shape.getRadius())
	{
		if (ball.vel.y < 0)ball.vel.y += abs(ball.vel.y) * 2;
		else ball.vel.y = -ball.vel.y;
	}
	if (ball.vel.x == 0.0f && ball.vel.y == 0.0f)
	{
		if (ball.shape.getPosition().x <= ball.shape.getRadius())
		{
			ball.shape.setPosition({ ball.shape.getRadius()+5, ball.shape.getPosition().y });
		}
		if (ball.shape.getPosition().x >= w - ball.shape.getRadius())
		{
			ball.shape.setPosition({ w - ball.shape.getRadius()-5, ball.shape.getPosition().y });
		}
		if (ball.shape.getPosition().y <= ball.shape.getRadius())
		{
			ball.shape.setPosition({ ball.shape.getPosition().x, ball.shape.getRadius()+5 });
		}
		if (ball.shape.getPosition().y >= h - ball.shape.getRadius())
		{
			ball.shape.setPosition({ ball.shape.getPosition().x,h - ball.shape.getRadius()-5 });

		}


	}
}
void moveBall(Ball& ball)
{
	ball.shape.move(ball.vel);
	//ball.shape.setTextureRect({ ball.shape.getTextureRect().position - (sf::Vector2i)ball.vel, { (int)ball.shape.getRadius() * 2,(int)ball.shape.getRadius() * 2} });
	ball.vel.x *= ball.acc.x;
	ball.vel.y *= ball.acc.y;
	if (abs(ball.vel.x) + abs(ball.vel.y) < 0.1f) {
		ball.vel.x = 0.0f;
		ball.vel.y = 0.0f;
	}
}
sf::VertexArray makeLine(sf::VertexArray& line, Ball white, sf::Vector2f mousePos)
{
	
	line[0].position = mousePos;
	line[0].color = sf::Color::Black;
	line[1].position = white.shape.getPosition();
	line[1].color = sf::Color::Black;
	return line;
}
bool overlap(Ball a, Ball b)
{
	float ax = a.shape.getPosition().x, bx = b.shape.getPosition().x;
	float ay = a.shape.getPosition().y, by = b.shape.getPosition().y;
	return (ax - bx) * (ax - bx) + (ay - by) * (ay - by) <= (a.shape.getRadius() + b.shape.getRadius()) * (a.shape.getRadius() + b.shape.getRadius());
}
void workCollision(Ball& a, Ball& b)
{
	float ax = a.shape.getPosition().x, bx = b.shape.getPosition().x;
	float ay = a.shape.getPosition().y, by = b.shape.getPosition().y;
	float dist = sqrtf((ax - bx)*(ax-bx)+ (ay - by)* (ay - by));
	float overlap = 0.5f * (dist - a.shape.getRadius()-b.shape.getRadius());
	ax -= overlap * (ax - bx) / dist;
	ay -= overlap * (ay - by) / dist;
	bx += overlap * (ax - bx) / dist;
	by += overlap * (ay - by) / dist;
	a.shape.setPosition({ ax ,ay});
	b.shape.setPosition({ bx,by});

}
void workDynamicCollision(Ball& a, Ball& b)
{
	float ax = a.shape.getPosition().x, bx = b.shape.getPosition().x;
	float ay = a.shape.getPosition().y, by = b.shape.getPosition().y;
	float dist = sqrtf((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
	float nx = (bx - ax) / dist;
	float ny = (by - ay) / dist;
	float tx = -ny;
	float ty = nx;
	float dpTan1 = a.vel.x * tx + a.vel.y * ty;
	float dpTan2 = b.vel.x * tx + b.vel.y * ty;
	float dpNorm1 = a.vel.x * nx + a.vel.y * ny;
	float dpNorm2 = b.vel.x * nx + b.vel.y * ny;
	float m1 = (dpNorm1 * (a.mass - b.mass) + 2.0f * b.mass * dpNorm2) / (a.mass + b.mass);
	float m2 = (dpNorm2 * (b.mass - a.mass) + 2.0f * a.mass * dpNorm1) / (a.mass + b.mass);

	a.vel.x = tx * dpTan1+nx*m1;
	a.vel.y = ty * dpTan1+ny*m1;
	b.vel.x = tx * dpTan2+nx*m2;
	b.vel.y = ty * dpTan2+ny*m2;

}
void makeHoles(vector<sf::CircleShape>& holes)
{
	for (int i = 0;i < holes.size();i++)
	{
		
			const float radius = 40.0f;
			holes[i] = (sf::CircleShape)radius;
			holes[i].setOrigin(holes[i].getGeometricCenter());
			holes[i].setFillColor(sf::Color::Black);
			holes[i].setOutlineThickness(4.0f);
			holes[i].setOutlineColor(sf::Color::Black);
	}
}
bool isPotted(Ball& ball, vector<sf::CircleShape> holes)
{
	for (int i = 0;i < 6;i++)
	{
		float ax = ball.shape.getPosition().x, ay = ball.shape.getPosition().y;
		float bx = holes[i].getPosition().x, by = holes[i].getPosition().y;
		if((ax - bx) * (ax - bx) + (ay - by) * (ay - by) <= (ball.shape.getRadius() + holes[i].getRadius()) * (ball.shape.getRadius() + holes[i].getRadius()))
			return true;


	}
	return false;

}