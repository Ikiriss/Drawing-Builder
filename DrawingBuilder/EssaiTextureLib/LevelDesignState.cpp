#include "LevelDesignState.h"
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
static const int maxBodyPerLevel = 30;
#pragma endregion VARIABLES


LevelDesignState::LevelDesignState(sf::RenderWindow& window, std::stack<States::stateName>& stack)
	: State(stack, window)
	, mWindow(&window)
	, mStack(&stack)
	, mWorld()
	, topView(sf::FloatRect(200.f, 0.f, 1000.f, 200.f))
	, buttomView(sf::FloatRect(0.f, 200.f, 1200.f, 800.f))
	, circleGenerator()
	, rectangleGenerator()
	, triangleGenerator()
{
	// Allow to create a level with maxBodyPerLevel bodies max
	mWorld.setNumberOfLevelBody(maxBodyPerLevel);

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


void LevelDesignState::draw()
{
	mWindow->setView(topView);
	// On dessine la partie top permettant de générer des figures
	mWindow->draw(circleGenerator);
	mWindow->draw(rectangleGenerator);
	mWindow->draw(triangleGenerator);

	// On dessine la partie du bas
	mWindow->setView(buttomView);
	// On dessine les elements du monde
	mWorld.update((float)fps);
	mWorld.draw(*mWindow);
}


void LevelDesignState::handleEvent(sf::Event& event)
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
		mWorld.update((float)fps);
		break;

	// Mouse click event
	case sf::Event::MouseButtonPressed:
		switch (event.mouseButton.button)
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
			State::requestStackPop();
			break;

		// Click on enter to save your drawing and create a new level
		case sf::Keyboard::Return:
			saveConstructionInfo();
			mWorld.deleteAllDynamicBody();
			State::requestStackPop();
			break;
		}
		break;
	}
}


void LevelDesignState::saveConstructionInfo()
{

	std::string const XML_FILE_PATH = "C://Users/admin/Documents//Visual Studio 2017/Projects/essaiTexture/essaiTexture/levelConstructor.xml";

	/// [create new xml file]
	// Generate new XML document within memory
	pugi::xml_document doc;
	// Generate XML declaration
	auto declarationNode = doc.append_child(pugi::node_declaration);
	declarationNode.append_attribute("version") = "1.0";
	declarationNode.append_attribute("encoding") = "UTF-8";
	declarationNode.append_attribute("standalone") = "yes";
	// Create the single root name
	auto root = doc.append_child("elements");
	root.append_attribute("level") = "Generated one";
	
	for (b2Body* bodyIterator = mWorld.getmb2World()->GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->isSelected)
		{
			if (bodyIterator->GetFixtureList() != nullptr &&
				bodyIterator->GetType() == b2_dynamicBody &&
				bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				pugi::xml_node element = root.append_child("polygon");
				element.append_attribute("xpos") = bodyIterator->GetPosition().x;
				element.append_attribute("ypos") = bodyIterator->GetPosition().y;
				element.append_attribute("numberOfPoint") = polygonShape->m_count;
				for (int i = 0; i < polygonShape->m_count; i++)
				{
					pugi::xml_node point = element.append_child("point");
					point.append_attribute("xpos") = polygonShape->m_vertices[i].x;
					point.append_attribute("ypos") = polygonShape->m_vertices[i].y;
				}
			}
			if (bodyIterator->GetFixtureList() != nullptr &&
				bodyIterator->GetType() == b2_dynamicBody &&
				bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				pugi::xml_node element = root.append_child("circle");
				element.append_attribute("xpos") = bodyIterator->GetPosition().x;
				element.append_attribute("ypos") = bodyIterator->GetPosition().y;
				element.append_attribute("radius") = circleShape->m_radius;
			}
		}
	}

	// Save XML tree as file
	bool saveSucceeded = doc.save_file(XML_FILE_PATH.c_str(), PUGIXML_TEXT("  "));
	assert(saveSucceeded);
	/// [create new xml file]
}
