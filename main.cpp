#include <SFML/Graphics.hpp>
#include "Actor.h"
#include "QuadTree.h"
#include <cstdlib>
#include <iostream>
#include <functional>
#include "Header.h"
#include "Behaviors.h"
#include "Button.h"

int main()
{
	srand(time(NULL));
	const int size = 2000;

	sf::Texture tex;
	tex.loadFromFile("boid.png");
	sf::Font font;
	font.loadFromFile("font.ttf");
	float collisionUpdate = 30;
	float behaviorUpdate = 15;
	float agentPerception = 20;



	std::array<Actor, size + 5> * list = new std::array<Actor, size + 5>();

	std::array<Transform, size + 5> * transform = new std::array<Transform, size + 5>();

	sf::Vector2f pos;
	for (int k = 0; k < size; ++k)
	{
		pos = sf::Vector2f(float(std::rand() % 1920), float(std::rand() % 1024));
		transform->operator[](k).friendly = true;
		list->operator[](k).setTransform(&transform->operator[](k));
		list->operator[](k).setPosition(pos);
		list->operator[](k).setTexture(tex);
		list->operator[](k).setMaxSpeed(1.25);
		list->operator[](k).getSprite().scale(0.3f, 0.14f);
		list->operator[](k).getSprite().setColor(sf::Color::White);
		list->operator[](k).setVelocity({ (std::rand() % 150) / 100.f, (std::rand() % 150) / 100.f });
		{
			if (std::rand() % 2 == 0)
				list->operator[](k).setVelocity(sf::Vector2f(list->operator[](k).getVelocity().x * -1, list->operator[](k).getVelocity().y));

			if (std::rand() % 2 == 0)
				list->operator[](k).setVelocity(sf::Vector2f(list->operator[](k).getVelocity().x, list->operator[](k).getVelocity().y * -1));
		}
	}

	for (int k = size; k < transform->size(); ++k)
	{
		pos = sf::Vector2f(float(std::rand() % 1920), float(std::rand() % 1024));
		transform->operator[](k).friendly = false;
		list->operator[](k).setTransform(&transform->operator[](k));
		list->operator[](k).setPosition(pos);
		list->operator[](k).setTexture(tex);
		list->operator[](k).setMaxSpeed(1.25);
		list->operator[](k).getSprite().scale(0.7f, 0.35f);
		list->operator[](k).getSprite().setColor(sf::Color::Red);
		list->operator[](k).setVelocity({ (std::rand() % 150) / 100.f, (std::rand() % 150) / 100.f });
		{
			if (std::rand() % 2 == 0)
				list->operator[](k).setVelocity(sf::Vector2f(list->operator[](k).getVelocity().x * -1, list->operator[](k).getVelocity().y));

			if (std::rand() % 2 == 0)
				list->operator[](k).setVelocity(sf::Vector2f(list->operator[](k).getVelocity().x, list->operator[](k).getVelocity().y * -1));
		}
	}

	sf::Clock CollisionClock;
	sf::Clock BehaviorClock;
	QuadTree<Actor, size + 5, 6> *tree;

	tree = new QuadTree<Actor, size + 5, 6>({ 0,0 }, { 1920,1024});
	tree->insertPoints(list);
	tree->alloc();
	CollisionClock.restart();
	tree->restart();
	tree->start();


	Button button[4];
	
	float cohesionBehavior = 1;
	float alignmentBehavior = 1;
	float separationBehavior = 1;
	float runAwayBehavior = 1;

	for (int k = 0; k < 4; ++k)
	{
		button[k].getShape().setSize({ 100,40 });
		button[k].getText().setFont(font);
		button[k].getShape().setPosition(sf::Vector2f{ 10.f,static_cast<float>((k + 1) * 60 )});
		button[k].getText().setPosition(sf::Vector2f{ 15.f,static_cast<float>((k + 1) * 60 + 10) });
		button[k].getShape().setFillColor(sf::Color::Green);
		button[k].getText().setCharacterSize(10);
		button[k].getShape().setFillColor(sf::Color(255,255,255,158));
	}
	button[0].setString("cohesion");
	button[0].setTarget(&cohesionBehavior);

	button[1].setString("alignment");
	button[1].setTarget(&alignmentBehavior);
	
	button[2].setString("separation");
	button[2].setTarget(&separationBehavior);
	
	button[3].setString("runAway");
	button[3].setTarget(&runAwayBehavior);

	sf::Clock delta;

	sf::RenderWindow window(sf::VideoMode(1920, 1024), "QuadTree");
	window.setFramerateLimit(60);
	//window.setMouseCursorVisible(false);
	//sf::sleep(sf::Time(sf::seconds(2)));
	while (window.isOpen())
	{
		if (CollisionClock.getElapsedTime().asMilliseconds() > collisionUpdate)
		{
			CollisionClock.restart();
			tree->restart();
			tree->start();
		}
		else if (BehaviorClock.getElapsedTime().asMilliseconds() > behaviorUpdate)
		{
			sf::Vector2f calculatedMove = { 0,0 };
			BehaviorClock.restart();
			std::vector<short> indexVector;
			std::vector<Actor*> agents;
			std::vector<sf::Vector2f> velocity;
			std::vector<Transform*> objects;

			std::function<bool(const Transform* ref)> isFriendly = [](const Transform* ref)->bool {
				return ref->friendly;
			};

			for (int k = 0; k < size; ++k)
			{
				tree->find(indexVector, list->operator[](k).getPosition(), agentPerception); 
				for (auto j : indexVector)
				{
					if (j != k)
					{
						objects.push_back(&list->operator[](j).getTransform());

						if(isFriendly(&list->operator[](k).getTransform()))
							velocity.push_back({ list->operator[](j).getVelocity().x, list->operator[](j).getVelocity().y });
					}
				}
				
				calculatedMove += CalculateAlignmentBehavior<>(velocity) * alignmentBehavior;
				
				calculatedMove += CalculateCohesionBehavior<>(
					list->operator[](k).getTransform(),
					objects, isFriendly) * cohesionBehavior;

				calculatedMove += CalculateSeparationBehavior<>(
					list->operator[](k).getTransform(),
					objects, isFriendly) * separationBehavior;

				calculatedMove += CalculateRunAwayBehavior<>(list->operator[](k).getTransform(),
					objects, isFriendly) * runAwayBehavior;

				if (calculatedMove.x != 0 || calculatedMove.y != 0)
					list->operator[](k).setVelocity(calculatedMove);

				objects.clear();
				agents.clear();
				velocity.clear();
				calculatedMove = { 0,0 };
				indexVector.clear();
			}
		}
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
				window.close();
		}



		for (auto &k : *list)
		{
			k.move();
			if (k.getPosition().x > 1920)
			{
				k.setPosition(sf::Vector2f(0, k.getPosition().y));
			}
			else if (k.getPosition().x < 0)
			{
				k.setPosition(sf::Vector2f(1920, k.getPosition().y));
			}

			if (k.getPosition().y > 1024)
			{
				k.setPosition(sf::Vector2f(k.getPosition().x, 0));
			}
			else if (k.getPosition().y < 0)
			{
				k.setPosition(sf::Vector2f(k.getPosition().x, 1024));
			}


		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			auto v = delta.restart().asSeconds();
			auto pos = sf::Mouse::getPosition(window);
			for (int k = 0; k < 4; ++k)
				button[k].update(v,pos);
		}
		else delta.restart();


		window.clear();
#ifdef debug
		tree->draw(&window);
#endif
		for (int k = 0; k < size+5; ++k)
		{
			window.draw(list->operator[](k).getSprite());
		}
		for (int k = 0; k < 4; ++k)
			window.draw(button[k]);

		window.display();
	}
	

	return 0;
}

