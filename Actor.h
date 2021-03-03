#pragma once
#include <SFML/Graphics.hpp>
#include "Header.h"
#include "Transform.h"

class Actor {
public:
	Actor();

	const sf::Vector2f& getPosition() const;
	void setPosition(const sf::Vector2f& ref);

	void setTransform(Transform* transform);
	Transform& getTransform();

	void setTexture(const sf::Texture& tex);
	sf::Sprite& getSprite();

	void move();

	void setMaxSpeed(const float speed);

	void setRotation(const float rotation);

	void reMagnitudeVelocity();

	void setVelocity(const sf::Vector2f& ref);
	const sf::Vector2f& getVelocity() const;
private:
	Transform* m_transform;
	sf::Vector2f m_velocity;
	sf::Sprite m_sprite;
	float m_maxSpeed;
};

