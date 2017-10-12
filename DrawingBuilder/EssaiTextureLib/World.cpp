#include <World.h>

static const float scale = 37.8f; // SCALE is not used
const float margeErreurRadius = 5.f;
const float margeErreurPosition = 5.f;
const float margeErreurWidth = 5.f;
const float margeErreurHeight = 5.f;

World::World()
	: mb2World(b2Vec2::b2Vec2(0.f, 9.8f))
	, mBoisTexture()
	, mBoisTextureVerte()
	, numberOfSelectedBody(0)
	, numberOfLevelBody(3)
	, levelCircleDrawing()
	, levelRectangleDrawing()
	, levelPolygonDrawing()
{

	// Buttom ground
	createGround(0.f, 990.f, 1200.f, 50.f);
	// Top ground
	createGround(0.f, 200.f, 1200.f, 25.f);
	// Left ground
	createGround(-40.f, 400.f, 50.f, 1200.f);
	// Right ground
	createGround(1240.f, 400.f, 50.f, 1200.f);
	// Load texture
	mBoisTexture.loadFromFile("texturebois.jpg");
	mBoisTextureVerte.loadFromFile("textureboisvert.jpg");

	
	// Creation du dessin du niveau manuellement
	sf::CircleShape circle1(50.f);
	circle1.setPosition(sf::Vector2f(450.f, 865.f));
	circle1.setOutlineThickness(2);
	circle1.setOutlineColor(sf::Color::Black);

	sf::CircleShape circle2(50.f);
	circle2.setPosition(sf::Vector2f(600.f, 865.f));
	circle2.setOutlineThickness(2);
	circle2.setOutlineColor(sf::Color::Black);

	levelCircleDrawing.push_back(circle1);
	levelCircleDrawing.push_back(circle2);

	sf::RectangleShape rectangle1(sf::Vector2f(200.f, 100.f));
	rectangle1.setPosition(sf::Vector2f(475.f, 765.f));
	rectangle1.setOutlineThickness(2);
	rectangle1.setOutlineColor(sf::Color::Black);
	levelRectangleDrawing.push_back(rectangle1);
	
	
}


void World::createRectangle(float mouseX, float mouseY, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX , mouseY);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = mb2World.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width / 2), (height / 2));
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.5f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}


void World::createCircle(float mouseX, float mouseY, float radius)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(mouseX, mouseY);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = mb2World.CreateBody(&bodyDef);

	b2CircleShape shape;
	shape.m_radius = radius;
	b2FixtureDef fixtureDef;
	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.5f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}


void World::createTriangle(float mouseX, float mouseY)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2((float) mouseX, (float) mouseY);
	bodyDef.type = b2_dynamicBody;
	b2Body* body = mb2World.CreateBody(&bodyDef);

	b2Vec2 vertices[3];
	vertices[0].Set(0.0f, -50.0f);
	vertices[1].Set(50.0f, 50.0f);
	vertices[2].Set(-50.0f, 50.0f);
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


void World::createGround(float X, float Y, float width, float height)
{
	b2BodyDef bodyDef;
	bodyDef.position = b2Vec2(X, Y);
	bodyDef.type = b2_staticBody;
	b2Body* body = mb2World.CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox((width), (height / 2));
	b2FixtureDef fixtureDef;
	fixtureDef.density = 0.f;
	fixtureDef.shape = &shape;
	body->CreateFixture(&fixtureDef);
}


void World::deleteBodyOnTouch(float mouseXConvert, float mouseYConvert)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseXConvert, (float32)mouseYConvert)))
				{
					if (bodyIterator->isSelected)
					{
						numberOfSelectedBody--;
					}
					bodyIterator->DestroyFixture(bodyIterator->GetFixtureList());
				}
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				if (circleShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseXConvert, (float32)mouseYConvert)))
				{
					if (bodyIterator->isSelected)
					{
						numberOfSelectedBody--;
					}
					bodyIterator->DestroyFixture(bodyIterator->GetFixtureList());
				}
			}
		}
	}
}


void World::deleteAllDynamicBody()
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody)
			{
				bodyIterator->DestroyFixture(bodyIterator->GetFixtureList());
			}
		}
	}
	numberOfSelectedBody = 0;
}


void World::changeBodyPosition(float mouseX, float mouseY)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					bodyIterator->SetTransform(b2Vec2((float32)mouseX, (float32)mouseY), bodyIterator->GetAngle());
				}
			}
		}
	}
}


void World::changeBodyAngle(float mouseX, float mouseY, float angleSpeed)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					bodyIterator->SetTransform(b2Vec2(bodyIterator->GetPosition().x, bodyIterator->GetPosition().y), bodyIterator->GetAngle() + (angleSpeed));
				}
			}
		}
	}
}


void World::changeBodyHeight(float mouseX, float mouseY, float widthUp, float heightUp, float radiusUp, float triangleHeightUp)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			float bodyXPixelConvert = bodyIterator->GetPosition().x;
			float bodyYPixelConvert = bodyIterator->GetPosition().y;
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				int numberOfPoint = polygonShape->m_count;
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					if (numberOfPoint == 4)
					{
						for (int i = 0; i < numberOfPoint; i++)
						{
							if (i == 0)
							{
								polygonShape->m_vertices[i] += b2Vec2(-widthUp, -heightUp);
							}
							else if (i == 1)
							{
								polygonShape->m_vertices[i] += b2Vec2(widthUp, -heightUp);
							}
							else if (i == 2)
							{
								polygonShape->m_vertices[i] += b2Vec2(widthUp, heightUp);
							}
							else if (i == 3)
							{
								polygonShape->m_vertices[i] += b2Vec2(-widthUp, heightUp);
							}
						}
					}
					else if (numberOfPoint == 3)
					{
						for (int i = 0; i < numberOfPoint; i++)
						{
							if (i == 0)
							{
								polygonShape->m_vertices[i] += b2Vec2(0.f, triangleHeightUp);
							}
							else if (i == 1)
							{
								polygonShape->m_vertices[i] += b2Vec2(-triangleHeightUp, -triangleHeightUp);
							}
							else if (i == 2)
							{
								polygonShape->m_vertices[i] += b2Vec2(+triangleHeightUp, triangleHeightUp);
							}
						}
					}
				}
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				if (circleShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					circleShape->m_radius += radiusUp;
				}
			}
		}
	}
}


void World::selectBody(float mouseX, float mouseY)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			float bodyXPixelConvert = bodyIterator->GetPosition().x;
			float bodyYPixelConvert = bodyIterator->GetPosition().y;
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				if (polygonShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					if (!bodyIterator->isSelected)
					{
						if (numberOfSelectedBody < numberOfLevelBody)
						{
							bodyIterator->isSelected = true;
							numberOfSelectedBody++;
						}
					}
					else
					{
						if (numberOfSelectedBody >= 0)
						{
							bodyIterator->isSelected = false;
							numberOfSelectedBody--;
						}
					}
				}
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				b2CircleShape* circleShape = (b2CircleShape*)bodyIterator->GetFixtureList()->GetShape();
				if (circleShape->TestPoint(bodyIterator->GetTransform(), b2Vec2((float32)mouseX, (float32)mouseY)))
				{
					if (!bodyIterator->isSelected)
					{
						if (numberOfSelectedBody < numberOfLevelBody)
						{
							bodyIterator->isSelected = true;
							numberOfSelectedBody++;
						}
					}
					else
					{
						if (numberOfSelectedBody >= 0)
						{
							bodyIterator->isSelected = false;
							numberOfSelectedBody--;
						}
					}
				}
			}
		}
	}
}


void World::drawBody(b2Body* body, sf::RenderWindow& window)
{
	if(body->isSelected)
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
				shape->setPoint(i, sf::Vector2f(polygonShape->m_vertices[i].x, polygonShape->m_vertices[i].y));
			}
			shape->setRotation(body->GetAngle()*(180 / b2_pi));
			shape->setPosition(body->GetPosition().x, body->GetPosition().y);
			shape->setTexture(&mBoisTextureVerte);
			window.draw(*shape);
		}
	}
	else
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
				shape->setPoint(i, sf::Vector2f(polygonShape->m_vertices[i].x, polygonShape->m_vertices[i].y));
			}
			shape->setRotation(body->GetAngle()*(180 / b2_pi));
			shape->setPosition(body->GetPosition().x, body->GetPosition().y);
			shape->setTexture(&mBoisTexture);
			window.draw(*shape);
		}
	}

}


void World::drawCircle(b2Body* body, sf::RenderWindow& window, float radius)
{
	if (body->isSelected)
	{
		if (body->GetFixtureList() != nullptr)
		{
			std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(radius);
			circle->setOutlineThickness(2);
			circle->setOutlineColor(sf::Color::Black);
			circle->setPosition(body->GetPosition().x - radius, body->GetPosition().y - radius);
			circle->setTexture(&mBoisTextureVerte);
			window.draw(*circle);
		}
	}
	else
	{
		if (body->GetFixtureList() != nullptr)
		{
			std::unique_ptr<sf::CircleShape> circle = std::make_unique<sf::CircleShape>(radius);
			circle->setOutlineThickness(2);
			circle->setOutlineColor(sf::Color::Black);
			circle->setPosition(body->GetPosition().x - radius, body->GetPosition().y - radius);
			circle->setTexture(&mBoisTexture);
			window.draw(*circle);
		}
	}

}


void World::draw(sf::RenderWindow& window)
{
	for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
	{
		if (bodyIterator->GetFixtureList() != nullptr)
		{
			if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
			{
				drawBody(bodyIterator, window);
			}
			else if (bodyIterator->GetType() == b2_dynamicBody && bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
			{
				drawCircle(bodyIterator, window, bodyIterator->GetFixtureList()->GetShape()->m_radius);
			}
			else
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
				int numberOfPoint = polygonShape->m_count;

				sf::ConvexShape shape(numberOfPoint);
				shape.setOutlineThickness(2);
				shape.setOutlineColor(sf::Color::Black);
				for (int i = 0; i < numberOfPoint; i++)
				{
					shape.setPoint(i, sf::Vector2f(polygonShape->m_vertices[i].x, polygonShape->m_vertices[i].y));
				}
				shape.setRotation(bodyIterator->GetAngle()*(180 / b2_pi));
				shape.setPosition(bodyIterator->GetPosition().x, bodyIterator->GetPosition().y);
				shape.setTexture(&mBoisTexture);
				window.draw(shape);
			}
		}
	}
}


void World::drawLevel(sf::RenderWindow& window)
{
	for (std::vector<sf::CircleShape>::iterator it = levelCircleDrawing.begin(); it != levelCircleDrawing.end(); it++)
	{
		window.draw(*it);
	}
	
	for (std::vector<sf::RectangleShape>::iterator it = levelRectangleDrawing.begin(); it != levelRectangleDrawing.end(); it++)
	{
		window.draw(*it);
	}
	/*
	for (std::vector<sf::ConvexShape>::iterator it = levelPolygonDrawing.begin(); it != levelPolygonDrawing.end(); it++)
	{
		window.draw(*it);
	}
	*/
}


bool World::isWin() 
{
	int compteur = 0;
	if (numberOfLevelBody == numberOfSelectedBody)
	{
		for (std::size_t i = 0; i < levelCircleDrawing.size(); i++)
		{
			for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
			{
				if (bodyIterator->GetFixtureList() != nullptr)
				{
					if (bodyIterator->isSelected)
					{
						if (bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_circle)
						{
							float x = bodyIterator->GetPosition().x;
							float y = bodyIterator->GetPosition().y;
							float radius = bodyIterator->GetFixtureList()->GetShape()->m_radius;
							float differenceX = levelCircleDrawing[i].getPosition().x - x + radius;
							float differenceY = levelCircleDrawing[i].getPosition().y - y + radius;
							float differenceRadius = levelCircleDrawing[i].getRadius() - radius;
							if ((std::abs(differenceX) < margeErreurPosition) &&
								(std::abs(differenceY) < margeErreurPosition) &&
								(std::abs(differenceRadius) < margeErreurRadius))
							{
								compteur++;
							}
						}
					}
				}
			}
		}
		for (std::size_t j = 0; j < levelRectangleDrawing.size(); j++)
		{
			for (b2Body* bodyIterator = mb2World.GetBodyList(); bodyIterator != 0; bodyIterator = bodyIterator->GetNext())
			{
				if (bodyIterator->GetFixtureList() != nullptr)
				{
					if (bodyIterator->isSelected)
					{
						if (bodyIterator->GetFixtureList()->GetShape()->GetType() == b2Shape::Type::e_polygon)
						{
							float x = bodyIterator->GetPosition().x;
							float y = bodyIterator->GetPosition().y;
							float differenceXRectangle = levelRectangleDrawing[j].getPosition().x - x + 100;
							float differenceYRectangle = levelRectangleDrawing[j].getPosition().y - y + 50;
							if ((std::abs(differenceXRectangle) < margeErreurPosition) &&
								(std::abs(differenceYRectangle) < margeErreurPosition))
							{
								compteur++;
							}
							/*
							// Tentative de gerer les distances entre point de chaque figure
							b2PolygonShape* polygonShape = (b2PolygonShape*)bodyIterator->GetFixtureList()->GetShape();
							int numberOfPoint = polygonShape->m_count;
							int compteurPoint = 0;
							for (int k = 0; k < numberOfPoint; k++)
							{									
									float width = polygonShape->m_vertices[k].x + polygonShape->m_vertices[k+1].x;
									float height = polygonShape->m_vertices[k].y;
									float differenceWidth = levelRectangleDrawing[j].getSize().x - width;
									float differenceHeight = levelRectangleDrawing[j].getSize().y - height;
									if (std::fabs(differenceWidth) < margeErreurWidth ||
										std::fabs(differenceHeight) < margeErreurHeight)
									{
										compteurPoint++;
									}
							}
							if (compteurPoint == numberOfPoint)
							{
								compteur++;
							}
							*/
						}
					}
				}
			}
		}
	}
	if (compteur == numberOfLevelBody)
	{
		return true;
	}

	return false;
}

