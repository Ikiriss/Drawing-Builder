#include "GameState.h"
#include "iostream"
#include <chrono>
#include <thread>
#include <vector>

#pragma region VARIABLES
static const float scale = 37.8f; // SCALE is the link between the convertion to pixel into cm or cm into pixel
static const float angleRotateSpeed = 0.05f; // Rotation speed
static const float widthUp = 1.f; // Rectangle Width up speed
static const float heightUp = 0.5f; // Rectangle Height up speed
static const float radiusUp = 1.f; // Radius up speed
static const float triangleHeightUp = 0.5f; // Triangle Height up speed
static int fps = 30; // Frame Per Second
const float margeErreurRadius = 5.f;
const float margeErreurPosition = 5.f;
#pragma endregion VARIABLES


GameState::GameState(sf::RenderWindow& window, std::stack<States::stateName>& stack)
	: State(stack, window)
	, mWindow(&window)
	, mStack(&stack)
	, mLevel(0)
	, mWorld()
	, topView(sf::FloatRect(200.f, 0.f, 1000.f, 200.f))
	, buttomView(sf::FloatRect(0.f, 200.f, 1200.f, 800.f))
	, circleGenerator()
	, rectangleGenerator()
	, triangleGenerator()
	, isLevelAlreadyLoaded(false)
{

	// Dessin des generateur de figures
	circleGenerator.setTexture(&mWorld.getBoisTexture());
	circleGenerator.setOutlineThickness(2);
	circleGenerator.setOutlineColor(sf::Color::Black);
	circleGenerator.setRadius(50.f);
	circleGenerator.setPosition(sf::Vector2f(500.f, 50.f));

	rectangleGenerator.setTexture(&mWorld.getBoisTexture());
	rectangleGenerator.setOutlineThickness(2);
	rectangleGenerator.setOutlineColor(sf::Color::Black);
	rectangleGenerator.setSize(sf::Vector2f(200.f, 100.f));
	rectangleGenerator.setPosition(sf::Vector2f(700.f, 50.f));

	triangleGenerator.setTexture(&mWorld.getBoisTexture());
	triangleGenerator.setOutlineThickness(2);
	triangleGenerator.setOutlineColor(sf::Color::Black);
	triangleGenerator.setPointCount(3);
	triangleGenerator.setRadius(70.f);
	triangleGenerator.setPosition(sf::Vector2f(1000.f, 50.f));


	// Emplacement des différentes vues
	topView.setViewport(sf::FloatRect((float)(1 / 6), 0.f, 1.f, 0.2f));
	buttomView.setViewport(sf::FloatRect(0.f, 0.2f, 1.f, 0.8f));
}

void GameState::draw()
{
	mWindow->setView(topView);
	// On dessine la partie top permettant de générer des figures
	mWindow->draw(circleGenerator);
	mWindow->draw(rectangleGenerator);
	mWindow->draw(triangleGenerator);
	
	// On dessine la partie du bas
	mWindow->setView(buttomView);
	// Dessin du level
	mWorld.drawLevel(*mWindow);
	// On dessine les elements du monde
	mWorld.update((float) fps);
	mWorld.draw(*mWindow);
}


void GameState::handleEvent(sf::Event& event)
{
	//Initialisation des variables utiles
	float originCircleX = circleGenerator.getPosition().x;
	float originCircleY = circleGenerator.getPosition().y;
	float originRectangleX = rectangleGenerator.getPosition().x;
	float originRectangleY = rectangleGenerator.getPosition().y;
	float originTriangleX = triangleGenerator.getPosition().x;
	float originTriangleY = triangleGenerator.getPosition().y;
	float mouseX = (float)sf::Mouse::getPosition(*mWindow).x;
	float mouseY = (float)sf::Mouse::getPosition(*mWindow).y;
	int wheelScrollValue = event.mouseWheel.delta;
	float scrollRadiusUp = (float)wheelScrollValue * radiusUp;
	float scrollWidthUp = (float)wheelScrollValue * widthUp;
	float scrollHeightUp = (float)wheelScrollValue * heightUp;
	float scrolltriangleHeightUp = (float)wheelScrollValue * triangleHeightUp;
	
	// liste des actions possibles dans le jeu
	switch (event.type)
	{
	// Modification de la taille de l'objet par un scroll
	case sf::Event::MouseWheelMoved:
		mWorld.changeBodyHeight(mouseX, mouseY, scrollWidthUp, scrollHeightUp, scrollRadiusUp, scrolltriangleHeightUp);
		mWorld.update((float) fps);
		break;

	// Mouse click event
	case sf::Event::MouseButtonPressed:
		switch(event.mouseButton.button)
		{ 
		// Left click event
		case sf::Mouse::Left:
			/* Tentative d'equation du disque
			float normes = sqrt(pow((float)mouseX - originX, 2) + pow((float)mouseY - originY, 2));
			*/
			if (originCircleX - 200.f < mouseX &&
				mouseX < originCircleX &&
				originCircleY < mouseY &&
				mouseY < originCircleY + 100.f)
			{
				mWorld.createCircle(600, 500, 50.f);
			}
			else if (originRectangleX - 100.f < mouseX &&
				mouseX < originRectangleX + 100.f &&
				originRectangleY < mouseY &&
				mouseY < originRectangleY + 100.f)
			{
				mWorld.createRectangle(600, 500, 200.f, 100.f);
			}
			else if (originTriangleX - 100.f < mouseX &&
				mouseX < originTriangleX + 100.f &&
				originTriangleY < mouseY &&
				mouseY < originTriangleY + 100.f)
			{
				mWorld.createTriangle(600, 500);
			}
			break;

		// Right click event
		case sf::Mouse::Right:
			mWorld.selectBody(mouseX, mouseY);
			break;
		}
		break;

	case sf::Event::KeyReleased:
		switch (event.key.code)
		{
		// Rotation gauche
		case sf::Keyboard::Left:
			mWorld.changeBodyAngle(mouseX, mouseY, -angleRotateSpeed);
			break;

		// Rotation droite
		case sf::Keyboard::Right:
			mWorld.changeBodyAngle(mouseX, mouseY, angleRotateSpeed);
			break;

		// Click A to generate a rectangle
		case sf::Keyboard::A:
			mWorld.createRectangle(mouseX, mouseY, 200.f, 100.f);
			break;

		// Click Z to generate a circle
		case sf::Keyboard::Z:
			mWorld.createCircle(mouseX, mouseY, 50.f);
			break;

		// Click E to generate a triangle
		case sf::Keyboard::E:
			mWorld.createTriangle(mouseX, mouseY);
			break;

		// Click on delete to delete a body
		case sf::Keyboard::Delete:
			mWorld.deleteBodyOnTouch(mouseX, mouseY);
			break;

		// Esc event
		case sf::Keyboard::Escape:
			// Delete the world and get back to the level menu
			mWorld.deleteAllDynamicBody();
			//deleteLevelInfo();
			isLevelAlreadyLoaded = false;
			State::requestStackPop();
			break;

		// Click on enter to confirm your drawing and test if you win the level
		case sf::Keyboard::Return:
			if (mWorld.getNumberOfSelectedBody() == mWorld.getNumberOfLevelBody())
			{
				if (isWin())
				{
					std::cout << "You win this level try the next one \n" << std::endl;
				}
				else
				{
					std::cout << "You lose better luck next time try again \n" << std::endl;
				}
				mWorld.deleteAllDynamicBody();
				//deleteLevelInfo();
				isLevelAlreadyLoaded = false;
				State::requestStackPop();
			}
			else
			{
				std::cout << "You have to select " << mWorld.getNumberOfLevelBody() << " bodies to confirm your drawing \n";
			}
		}
		break;
	}
}


bool GameState::isWin()
{
	return mWorld.isWin();
}


void GameState::loadLevelInfo()
{
	// list of the elements name and attribute to recuperate the good one and use it
	std::string circle("circle");
	std::string polygon("polygon");
	std::string xpos("xpos");
	std::string ypos("ypos");
	std::string radius("radius");
	std::string numberOfPoint("numberOfPoint");
	pugi::xml_parse_result result;
	pugi::xml_node elements;
	int numberOfElement = 0;
	switch (mLevel)
	{
	case 1:
		result = mDoc.load_file("Level2.xml", pugi::parse_default | pugi::parse_declaration);
		if (!result)
		{
			std::cout << "Parse error: " << result.description() << ", character pos= " << result.offset;
		}
		elements = mDoc.child("elements");
		for (pugi::xml_node_iterator element = elements.first_child(); element != elements.last_child(); element++)
		{
			if (circle.compare(element->name()))
			{
				float circleX;
				float circleY;
				float circleRadius;
				for (pugi::xml_attribute_iterator attr = element->attributes_begin(); attr != element->attributes_end(); attr++)
				{
					if (xpos.compare(attr->name()))
					{
						circleX = attr->as_float();
					}
					if (ypos.compare(attr->name()))
					{
						circleY = attr->as_float();
					}
					if (radius.compare(attr->name()))
					{
						circleRadius = attr->as_float();
					}
				}
				sf::CircleShape circle(circleRadius);
				circle.setPosition(sf::Vector2f(circleX, circleY));
				circle.setOutlineThickness(2);
				circle.setOutlineColor(sf::Color::Black);
				mWorld.getLevelCircleDrawing()->push_back(circle);
				numberOfElement++;
			}
			if (polygon.compare(element->name()))
			{
				float polygonX;
				float polygonY;
				int nbrPoints = 0;
				for (pugi::xml_attribute_iterator attr = element->attributes_begin(); attr != element->attributes_end(); attr++)
				{
					if (xpos.compare(attr->name()))
					{
						polygonX = attr->as_float();
					}
					if (ypos.compare(attr->name()))
					{
						polygonY = attr->as_float();
					}
					if (numberOfPoint.compare(attr->name()))
					{
						nbrPoints = attr->as_int();
					}
				}
				sf::ConvexShape shape(nbrPoints);
				shape.setPosition(sf::Vector2f(polygonX, polygonY));
				shape.setOutlineThickness(2);
				shape.setOutlineColor(sf::Color::Black);
				int i = 0;
				for (pugi::xml_node_iterator points = element->first_child(); points != element->last_child(); points++)
				{
					float pointX = 0;
					float pointY = 0;
					for (pugi::xml_attribute_iterator it = points->attributes_begin(); it != points->attributes_end(); it++)
					{
						if (xpos.compare(it->name()))
						{
							pointX = it->as_float();
						}
						if (ypos.compare(it->name()))
						{
							pointY = it->as_float();
						}
					}
					shape.setPoint(i, sf::Vector2f(pointX, pointY));
					i++;
				}
				mWorld.getLevelPolygonDrawing()->push_back(shape);
				numberOfElement++;
			}
			else
			{
				std::cout << "Problem at the element name it is not in the accepted list \n" << std::endl;
			}
		}
		mWorld.setNumberOfLevelBody(numberOfElement);
		break;
	default:
		std::cout << "The level you have selected does not exist please try later \n" << std::endl;
		break;
	}
}


void GameState::deleteLevelInfo()
{
	mWorld.getLevelCircleDrawing()->clear();
	mWorld.getLevelRectangleDrawing()->clear();
	mWorld.setNumberOfLevelBody(0);
}