#pragma once
#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <vector>

class World
{
public:
	World();

	void drawBody(b2Body* body, sf::RenderWindow& window);

	void drawCircle(b2Body* body, sf::RenderWindow& window, float radius); 	// Draw a circle shape
	
	void draw(sf::RenderWindow& window);

	void createGround(float X, float Y, float width, float height); // Creation of a static body

	void createRectangle(float mouseX, float mouseY, float width, float height); // Creation of a dynamic rectangle body

	void createCircle(float mouseX, float mouseY, float radius); // Creation of a dynamic circle body

	void createTriangle(float mouseX, float mouseY); // Creation of a dynamic triangle body

	void deleteBodyOnTouch(float mouseXConvert, float mouseYConvert); // Delete a body on touch
	
	void deleteAllDynamicBody(); // Delete all bodies

	void changeBodyPosition(float mouseXConvert, float mouseYConvert); // Change the position of the body on click if the mouse is on the body

	void changeBodyAngle(float mouseX, float mouseY, float angleSpeed); // Change the angle of the body on click if the mouse is on the body

	void changeBodyHeight(float mouseX, float mouseY, float widthUp, float heightUp, float radiusUp, float triangleHeightUp);  // change the body height
	
	void World::selectBody(float mouseX, float mouseY);

	sf::Texture getBoisTexture() { return mBoisTexture; };

	b2World* getmb2World() { return &mb2World; };

	int getNumberOfSelectedBody() { return numberOfSelectedBody; };

	int getNumberOfLevelBody() { return numberOfLevelBody; };

	void setNumberOfLevelBody(int number) { numberOfLevelBody = number; };

	std::vector<sf::CircleShape>* getLevelCircleDrawing() { return &levelCircleDrawing; };

	std::vector<sf::RectangleShape>* getLevelRectangleDrawing() { return &levelRectangleDrawing; };

	std::vector<sf::ConvexShape>* getLevelPolygonDrawing() { return &levelPolygonDrawing; };

	void update(float fps) { mb2World.Step(1 / fps, 8, 3); }

	void drawLevel(sf::RenderWindow & window);

	bool isWin();

private:
	b2World								mb2World;
	sf::Texture							mBoisTexture;
	sf::Texture							mBoisTextureVerte;
	int									numberOfLevelBody;
	int									numberOfSelectedBody;

	std::vector<sf::CircleShape>		levelCircleDrawing;
	std::vector<sf::RectangleShape>		levelRectangleDrawing;
	std::vector<sf::ConvexShape>		levelPolygonDrawing;
};