#pragma once
#include<SFML/Graphics.hpp>
#include<iostream>
#include "Ball.h"
using namespace std;
Ball putTextureOnBall( Ball& ball);
sf::Texture prepareTexture(string path);
void collideWithWall(Ball& ball, unsigned int w, unsigned int h);
void moveBall(Ball& ball);
sf::VertexArray makeLine(sf::VertexArray& line, Ball white, sf::Vector2f mousePos);
void workCollision(Ball& a, Ball& b);
bool overlap(Ball a, Ball b);
void workDynamicCollision(Ball& a, Ball& b);
void makeHoles(vector<sf::CircleShape>& holes);
bool isPotted(Ball& ball, vector<sf::CircleShape> holes);