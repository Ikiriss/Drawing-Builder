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
#include "leMain.h"
#include "MenuMainState.h"
#include "MenuLevelState.h"


#pragma region VARIABLES
static const float scale = 37.8f; // SCALE is the link between the convertion to pixel into cm or cm into pixel
static const float angleRotateSpeed = 1.f; // Rotation speed
static const float widthUp = 1.f; // Rectangle Width up speed
static const float heightUp = 0.5f; // Rectangle Height up speed
static const float radiusUp = 1.f; // Radius up speed
static const float triangleHeightUp = 0.5f; // Triangle Height up speed 
static int fps = 60; // Frame Per Second
sf::Texture dessin;
sf::Texture bois;
sf::Texture backGroundTexture;
static float margeErreur = 5.f;
static int numberMaxOfMousePosition = 60; // Nombre de max position de la souris retenu avant d'appliquer la force.
static int numberOfMousePositionStored = 0; // Nombre de position de la souris retenu avant d'appliquer la force.
std::vector<sf::Vector2f> mousePosition; // Vecteur retenant les positions de la souris pour gérer les applications de forces
#pragma endregion VARIABLES

enum gameState { MENU, GAME};

#pragma region FONCTION
// Creation of a static body
void createGround(b2World& world, float X, float Y, float width, float height);

// Creation of a dynamic rectangle body
void createRectangle(b2World& world, int mouseX, int mouseY, float width, float height);

// Creation of a dynamic circle body
void createCircle(b2World& world, int mouseX, int mouseY, float radius);

// Creation of a dynamic triangle body
void createTriangle(b2World& world, int mouseX, int mouseY);

// Delete a body on touch
void deleteBodyOnTouch(b2World& world, float mouseXConvert, float mouseYConvert);

// Change the position of the body on click if the mouse is on the body
void changeBodyPosition(b2Body* body, float mouseXConvert, float mouseYConvert);

// Change the angle of the body on click if the mouse is on the body
void changeBodyAngle(b2Body* body, float mouseXConvert, float mouseYConvert, float angleSpeed);

// Draw a rectangle shape
void drawBody(b2Body* body, sf::RenderWindow& window);

// Draw a circle shape
void drawCircle(b2Body* body, sf::RenderWindow& window, float radius);

// change the body height (EN ETAT FAIL)
void changeBodyHeight(b2Body* body, float mouseXConvert, float mouseYConvert, float widthUp, float heightUp, float radiusUp, float triangleHeightUp);

// Know if the player win the game
bool gameIsWin(b2World& world);
#pragma endregion FONCTIONS

int leMain()
{
	#pragma region INITIALISATION
	/** Prepare the window */
	sf::RenderWindow window(sf::VideoMode(1200, 1000, 32), "Test");
	window.setFramerateLimit(fps);


	std::stack<States::stateName> stack;
	const States::stateName menuState = States::stateName::Menu;
	stack.push(menuState);
	
	// Initiatilisation du menu
	MenuMainState menu(window, stack);

	/** Prepare the world */
	b2Vec2 gravity(0.f, 9.8f);
	b2World world(gravity);
	// Buttom ground
	createGround(world, 0.f, 990.f, 1200.f, 50.f);
	// Top ground
	createGround(world, 0.f, 200.f, 1200.f, 25.f);
	// Left ground
	createGround(world, -40.f, 400.f, 50.f, 1200.f);
	// Right ground
	createGround(world, 1240.f, 400.f, 50.f, 1200.f);

	// Initialisation des vues
	sf::View topView(sf::FloatRect(200.f, 0.f, 1000.f, 200.f));
	sf::View buttomView(sf::FloatRect(0.f, 200.f, 1200.f, 800.f));
	sf::View dessinView(sf::FloatRect(0.f, 0.f, 200.f, 200.f));

	// Emplacement des différentes vues
	topView.setViewport(sf::FloatRect((float)(1/6), 0.f, 1.f, 0.2f));
	buttomView.setViewport(sf::FloatRect(0.f, 0.2f, 1.f, 0.8f));
	dessinView.setViewport(sf::FloatRect(0.f, 0.f, 0.2f, 0.2f));
	
	// Chargement des textures
	dessin.loadFromFile("dessinNiveau1.png");
	bois.loadFromFile("texturebois.jpg");
	backGroundTexture.loadFromFile("miniProjet.png");

	// Dessin des generateur de figures
	sf::CircleShape circleGenerator;
	circleGenerator.setTexture(&bois);
	circleGenerator.setOutlineThickness(2);
	circleGenerator.setOutlineColor(sf::Color::Black);
	circleGenerator.setRadius(50.f);
	circleGenerator.setPosition(sf::Vector2f(500.f, 50.f));

	sf::RectangleShape rectangleGenerator;
	rectangleGenerator.setTexture(&bois);
	rectangleGenerator.setOutlineThickness(2);
	rectangleGenerator.setOutlineColor(sf::Color::Black);
	rectangleGenerator.setSize(sf::Vector2f(200.f, 100.f));
	rectangleGenerator.setPosition(sf::Vector2f(700.f, 50.f));

	sf::CircleShape triangleGenerator;
	triangleGenerator.setTexture(&bois);
	triangleGenerator.setOutlineThickness(2);
	triangleGenerator.setOutlineColor(sf::Color::Black);
	triangleGenerator.setPointCount(3);
	triangleGenerator.setRadius(70.f);
	triangleGenerator.setPosition(sf::Vector2f(1000.f, 50.f));

	// Initialisation du game State
	gameState actualGameState = MENU;
	#pragma endregion INITIALISATION

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Up:
					menu.moveUp();
					break;

				case sf::Keyboard::Down:
					menu.moveDown();
					break;

				case sf::Keyboard::Return:
					switch (menu.getPressedItem())
					{
					case 0:
						std::cout << "Play button has been pressed" << std::endl;
						actualGameState = GAME;
						while (actualGameState == GAME)
						{
							sf::Event event;
							while (window.pollEvent(event))
							{
								// Close event
								if (event.type == sf::Event::Closed)
									window.close();


								// Mouse scrool event
								else if (event.type == sf::Event::MouseWheelMoved)
								{
									int wheelScrollValue = event.mouseWheel.delta;
									float scrollRadiusUp = (float)wheelScrollValue * radiusUp;
									float scrollWidthUp = (float)wheelScrollValue * widthUp;
									float scrollHeightUp = (float)wheelScrollValue * heightUp;
									float scrolltriangleHeightUp = (float)wheelScrollValue * triangleHeightUp;
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
									{
										changeBodyHeight(bodyIterator, mouseXConvert, mouseYConvert, scrollWidthUp, scrollHeightUp, scrollRadiusUp, scrolltriangleHeightUp);
									}
								}

							}

							if (gameIsWin(world))
							{
								std::cout << "BRAVO ! VOUS AVEZ GAGNE !!! \n";
								actualGameState = MENU;
								return 0;
							}
							else
							{
								// Event pour revenir dans le menu
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
								{
									actualGameState = MENU;
								}

								
								// Event pour modifier la taille des objets avec clavier
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
								{
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
									{
										changeBodyHeight(bodyIterator, mouseXConvert, mouseYConvert, widthUp, heightUp, radiusUp, triangleHeightUp);
									}
								}

								// Event pour modifier la taille des objets avec clavier
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
								{
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
									{
										changeBodyHeight(bodyIterator, mouseXConvert, mouseYConvert, -widthUp, -heightUp, -radiusUp, -triangleHeightUp);
									}

								}


								// Right Keyboard event
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
								{
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
									{
										changeBodyAngle(bodyIterator, mouseXConvert, mouseYConvert, angleRotateSpeed);
									}
								}


								// Left Keyboard event
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
								{
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
									{
										changeBodyAngle(bodyIterator, mouseXConvert, mouseYConvert, -angleRotateSpeed);
									}
								}

								// A Keyboard event
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
								{
									int mouseX = sf::Mouse::getPosition(window).x;
									int mouseY = sf::Mouse::getPosition(window).y;
									createRectangle(world, mouseX, mouseY, 200.f, 100.f);
									std::this_thread::sleep_for(std::chrono::milliseconds(100));
								}

								// Z Keyboard event
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
								{
									int mouseX = sf::Mouse::getPosition(window).x;
									int mouseY = sf::Mouse::getPosition(window).y;
									createCircle(world, mouseX, mouseY, 50.f);
									std::this_thread::sleep_for(std::chrono::milliseconds(100));
								}

								// E Keyboard event
								if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
								{
									int mouseX = sf::Mouse::getPosition(window).x;
									int mouseY = sf::Mouse::getPosition(window).y;
									createTriangle(world, mouseX, mouseY);
									std::this_thread::sleep_for(std::chrono::milliseconds(100));
								}

								// Left Mouse click event
								if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
								{
									/* Tentative d'equation du disque
									int mouseX = sf::Mouse::getPosition(window).x;
									int mouseY = sf::Mouse::getPosition(window).y;
									float originX = circleGenerator.getPosition().x;
									float originY = circleGenerator.getPosition().y;
									float normes = sqrt(pow((float)mouseX - originX, 2) + pow((float)mouseY - originY, 2));
									*/
									if (circleGenerator.getPosition().x - 200.f < sf::Mouse::getPosition(window).x &&
										sf::Mouse::getPosition(window).x < circleGenerator.getPosition().x &&
										circleGenerator.getPosition().y < sf::Mouse::getPosition(window).y &&
										sf::Mouse::getPosition(window).y < circleGenerator.getPosition().y + 100.f)
									{
										createCircle(world, 600, 500, 50.f);
										std::this_thread::sleep_for(std::chrono::milliseconds(100));
									}
									else if (rectangleGenerator.getPosition().x - 100.f < sf::Mouse::getPosition(window).x &&
										sf::Mouse::getPosition(window).x < rectangleGenerator.getPosition().x + 100.f &&
										rectangleGenerator.getPosition().y < sf::Mouse::getPosition(window).y &&
										sf::Mouse::getPosition(window).y < rectangleGenerator.getPosition().y + 100.f)
									{
										createRectangle(world, 600, 500, 200.f, 100.f);
										std::this_thread::sleep_for(std::chrono::milliseconds(100));
									}
									else if (triangleGenerator.getPosition().x - 100.f < sf::Mouse::getPosition(window).x &&
										sf::Mouse::getPosition(window).x < triangleGenerator.getPosition().x + 100.f &&
										triangleGenerator.getPosition().y < sf::Mouse::getPosition(window).y &&
										sf::Mouse::getPosition(window).y < triangleGenerator.getPosition().y + 100.f)
									{
										createTriangle(world, 600, 500);
										std::this_thread::sleep_for(std::chrono::milliseconds(100));
									}
									else
									{
										float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
										float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
										for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
										{
											changeBodyPosition(bodyIterator, mouseXConvert, mouseYConvert);
										}
									}
								}

								// Right Mouse click event
								if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
								{
									float mouseXConvert = (float)sf::Mouse::getPosition(window).x / (float)scale;
									float mouseYConvert = (float)sf::Mouse::getPosition(window).y / (float)scale;
									deleteBodyOnTouch(world, mouseXConvert, mouseYConvert);
								}


								// Simule l'avancement des objets du monde en 60 images par seconde
								world.Step(1 / (float)fps, 8, 3);

								// Clean la windows pour redessiner (60 fois par seconde)
								window.clear(sf::Color::White);

								// On applique la texture dessin au coin droit de la fenêtre
								window.setView(dessinView);
								sf::Sprite dessinSprite;
								dessinSprite.setOrigin(0.f, 0.f);
								dessinSprite.setPosition(0.f, 0.f);
								dessinSprite.setTexture(dessin);
								window.draw(dessinSprite);

								// On dessine la partie top permettant de générer des figures
								window.setView(topView);
								window.draw(circleGenerator);
								window.draw(rectangleGenerator);
								window.draw(triangleGenerator);

								// On dessine les figures du monde
								window.setView(buttomView);
								int bodyCount = 0;
								for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
								{
									if (bodyIterator->GetFixtureList() != nullptr)
									{
										if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
										{
											drawBody(bodyIterator, window);
											++bodyCount;
										}
										else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
										{
											drawCircle(bodyIterator, window, bodyIterator->GetFixtureList()->GetShape()->m_radius*scale);
											++bodyCount;
										}
										else
										{
											drawBody(bodyIterator, window);
										}
									}
								}
								window.display();
							}
						}
						break;
					case 1:
						window.close();
						break;
					}

					break;
				}

				break;
			case sf::Event::Closed:
				window.close();

				break;

			}
		}

		window.clear();
		sf::Sprite backGround;
		backGround.setOrigin(0.f, 0.f);
		backGround.setPosition(0.f, 0.f);
		backGround.setTexture(backGroundTexture);
		window.draw(backGround);
		menu.draw();
		window.display();
	}
	

	return 0;
}

#pragma region FONCTION
void createRectangle(b2World& world, int mouseX, int mouseY, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX / scale, mouseY / scale);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2) / scale, (height / 2) / scale);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.5f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

void createCircle(b2World& world, int mouseX, int mouseY, float radius)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX / scale, mouseY / scale);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius / scale;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.5f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

void createTriangle(b2World& world, int mouseX, int mouseY) 
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX / scale, mouseY / scale);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2Vec2 vertices[3];
	vertices[0].Set(0.0f, -2.0f);
	vertices[1].Set(2.0f, 2.0f);
	vertices[2].Set(-2.0f, 2.0f);
	int32 count = 3;
	b2PolygonShape polygon;
	polygon.Set(vertices, count);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.3f;
	fixtureDef.restitution = 0.1f;
	fixtureDef.shape = &polygon;
	body->CreateFixture(&fixtureDef);
	body->ResetMassData();
}

void createGround(b2World& world, float x, float y, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(x / scale, y / scale);
	bodyDef.type = b2_staticBody;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width) / scale, (height / 2) / scale);
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}

void deleteBodyOnTouch(b2World& world, float mouseX, float mouseY)
{
	for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					bodyIterator->DestroyFixture(bodyIterator->GetFixtureList());
				}
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				if (circleShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					bodyIterator->DestroyFixture(bodyIterator->GetFixtureList());
				}
			}
		}
	}
}

void changeBodyPosition(b2Body* body, float mouseX, float mouseY)
{
	if (body->GetFixtureList() != nullptr)
	{
		if (body->GetType() == b2_dynamicBody)
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
			if (polygonShape->TestPoint(body->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
			{
				if (numberOfMousePositionStored == numberMaxOfMousePosition)
				{
					b2Vec2 vecteurForce = b2Vec2((mousePosition[numberMaxOfMousePosition-1].x - mousePosition[0].x), (mousePosition[numberMaxOfMousePosition-1].y - mousePosition[0].y));
					body->ApplyLinearImpulse(vecteurForce, body->GetWorldCenter() ,true);
					mousePosition.erase(mousePosition.begin(), mousePosition.begin() + numberMaxOfMousePosition);
					numberOfMousePositionStored = 0;
				}
				else if (numberOfMousePositionStored < numberMaxOfMousePosition)
				{
					mousePosition.push_back(sf::Vector2f(mouseX, mouseY));
					numberOfMousePositionStored++;
					body->SetTransform(b2Vec2((float32)mouseX, (float32)mouseY), body->GetAngle());
				}
			}
		}
	}
}


void changeBodyAngle(b2Body* body, float mouseX, float mouseY, float angleSpeed)
{
	if (body->GetFixtureList() != nullptr)
	{
		if (body->GetType() == b2_dynamicBody && body->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
			if (polygonShape->TestPoint(body->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
			{
				body->SetTransform(b2Vec2(body->GetPosition().x, body->GetPosition().y), body->GetAngle() + (angleSpeed / scale));
			}
		}
	}
}


void drawBody(b2Body* body, sf::RenderWindow& window)
{
	if (body->GetFixtureList() != nullptr)
	{
		b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
		int numberOfPoint = polygonShape->m_count;

		std::unique_ptr<sf::ConvexShape> shape = std::make_unique<sf::ConvexShape>(numberOfPoint);
		shape->setOutlineThickness(2);
		shape->setOutlineColor(sf::Color::Black);
		for (int i = 0; i < numberOfPoint; i++)
		{
			shape->setPoint(i, sf::Vector2f(polygonShape->m_vertices[i].x*scale, polygonShape->m_vertices[i].y*scale));
		}
		shape->setRotation(body->GetAngle()*(180/b2_pi));
		shape->setPosition(scale*body->GetPosition().x, scale*body->GetPosition().y);
		shape->setTexture(&bois);
		window.draw(*shape);
	}
}

void drawCircle(b2Body* body, sf::RenderWindow& window, float radius)
{
	if (body->GetFixtureList() != nullptr)
	{
		std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(radius);
		circle->setFillColor(sf::Color::White);
		circle->setOutlineThickness(2);
		circle->setOutlineColor(sf::Color::Black);
		circle->setPosition(scale*body->GetPosition().x - radius, scale*body->GetPosition().y - radius);
		circle->setTexture(&bois);
		window.draw(*circle);
	}
}


// Allow to UP the body's height
void changeBodyHeight(b2Body* body, float mouseXConvert, float mouseYConvert, float widthUp, float heightUp, float radiusUp, float triangleHeightUp)
{
	if (body->GetFixtureList() != nullptr)
	{
		float bodyXPixelConvert = body->GetPosition().x * scale;
		float bodyYPixelConvert = body->GetPosition().y * scale;
		if (body->GetType() == b2_dynamicBody && body->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
		{
			b2PolygonShape* polygonShape = (b2PolygonShape*)body->GetFixtureList()->GetShape();
			int numberOfPoint = polygonShape->m_count;
			if (polygonShape->TestPoint(body->GetTransform(), b2Vec2((float32)mouseXConvert, (float32)mouseYConvert)))
			{
				if (numberOfPoint == 4)
				{
					for (int i = 0; i < numberOfPoint; i++)
					{
						if (i == 0)
						{
							polygonShape->m_vertices[i] += b2Vec2(-widthUp / scale, -heightUp / scale);
						}
						else if (i == 1)
						{
							polygonShape->m_vertices[i] += b2Vec2(widthUp / scale, -heightUp / scale);
						}
						else if (i == 2)
						{
							polygonShape->m_vertices[i] += b2Vec2(widthUp / scale, heightUp / scale);
						}
						else if (i == 3)
						{
							polygonShape->m_vertices[i] += b2Vec2(-widthUp / scale, heightUp / scale);
						}
					}
				}
				else if (numberOfPoint == 3)
				{
					for (int i = 0; i < numberOfPoint; i++)
					{
						if (i == 0)
						{
							polygonShape->m_vertices[i] += b2Vec2(0.f, triangleHeightUp / scale);
						}
						else if (i == 1)
						{
							polygonShape->m_vertices[i] += b2Vec2(-triangleHeightUp / scale, -triangleHeightUp / scale);
						}
						else if (i == 2)
						{
							polygonShape->m_vertices[i] += b2Vec2(+triangleHeightUp / scale, triangleHeightUp / scale);
						}
					}
				}
			}
		}
		else if (body->GetType() == b2_dynamicBody && body->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
		{
			b2CircleShape* circleShape = (b2CircleShape*)body->GetFixtureList()->GetShape();
			if (circleShape->TestPoint(body->GetTransform(), b2Vec2((float32)mouseXConvert, (float32)mouseYConvert)))
			{
				circleShape->m_radius += radiusUp / scale;
			}
		}
	}
}


bool gameIsWin(b2World& world)
{
	std::vector<std::unique_ptr<sf::CircleShape>> circles;
	std::vector<std::unique_ptr<sf::ConvexShape>> rectangles;
	for (b2Body* bodyIterator = world.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				int numberOfPoint = polygonShape->m_count;
				if (numberOfPoint == 4)
				{
					std::unique_ptr<sf::ConvexShape> shape = std::make_unique<sf::ConvexShape>(numberOfPoint);
					for (int i = 0; i < numberOfPoint; i++)
					{
						shape->setPoint(i, sf::Vector2f(polygonShape->m_vertices[i].x*scale, polygonShape->m_vertices[i].y*scale));
					}
					shape->setRotation(bodyIterator->GetAngle()*scale);
					shape->setPosition(scale*bodyIterator->GetPosition().x, scale*bodyIterator->GetPosition().y);
					rectangles.push_back(std::move(shape));
				}
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(circleShape->m_radius);
				circle->setPosition(scale*bodyIterator->GetPosition().x - circleShape->m_radius, scale*bodyIterator->GetPosition().y - circleShape->m_radius);
				circles.push_back(std::move(circle));
			}
		}
	}
	for (auto &c1 : circles)
	{
		for (auto &c2 : circles)
		{
			for (auto &r : rectangles)
			{
				if ((c1->getPosition().x - c2->getPosition().x) < 5 * c1->getRadius() &&
					(c1->getPosition().x - c2->getPosition().x) > 3 * c1->getRadius() &&
				    abs((c1->getRadius() - c2->getRadius())) < margeErreur &&
					abs(c1->getPosition().y - c2->getPosition().y) < margeErreur &&
					abs((c1->getPosition().x - r->getPosition().x) - (c2->getPosition().x - r->getPosition().x)) < margeErreur &&
					abs((c1->getPosition().y - r->getPosition().x) - (c2->getPosition().y - r->getPosition().x)) < margeErreur)
				{
					return true;
				}
			}
		}
	}
	return false;
}
#pragma endregion FONCTIONS
