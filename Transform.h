#pragma once
#include <SFML/Graphics.hpp>
struct Transform {
	sf::Vector2f position;
	float rotation;
	bool friendly;
	bool operator==(const Transform& ref)
	{
		return position == ref.position && rotation == ref.rotation;
	}
};