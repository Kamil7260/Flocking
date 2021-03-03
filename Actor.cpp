#include "Actor.h"
#include "Functions.h"

Actor::Actor()
	:m_maxSpeed(0),m_transform(nullptr),m_velocity(0,0)
{}

const sf::Vector2f& Actor::getPosition() const {
	return m_transform->position;
}

void Actor::setPosition(const sf::Vector2f& ref) {
	m_transform->position = ref;
	m_sprite.setPosition(m_transform->position);
}

void Actor::setTransform(Transform* transform) {
	m_transform = transform;
}

Transform& Actor::getTransform() {
	return *m_transform;
}

void Actor::setTexture(const sf::Texture& tex) {
	m_sprite.setOrigin(sf::Vector2f({ tex.getSize().x / 2.f, tex.getSize().y / 2.f }));
	m_sprite.setTexture(tex);
}

sf::Sprite& Actor::getSprite() {
	return m_sprite;
}

void Actor::move() {
	m_transform->position += m_velocity;
	m_sprite.setPosition(m_transform->position);
}

void Actor::setMaxSpeed(const float speed) {
	m_maxSpeed = speed;
}

void Actor::setRotation(const float rotation) {
	m_transform->rotation = rotation;
}

void Actor::reMagnitudeVelocity() {
	float mag = sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
	m_velocity.x *= m_maxSpeed / mag;
	m_velocity.y *= m_maxSpeed / mag;
}

void Actor::setVelocity(const sf::Vector2f& ref) {
	m_velocity = ref;
	reMagnitudeVelocity();
	m_transform->rotation = vectorAngle(m_velocity);
	m_sprite.setRotation(m_transform->rotation);
}

const sf::Vector2f& Actor::getVelocity() const {
	return m_velocity;
}
