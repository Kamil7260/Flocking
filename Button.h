#pragma once
#include <SFML/Graphics.hpp>

class Button : public sf::Drawable {
public:
	Button() = default;
	Button(float* target);

	void setTarget(float* target) { m_target = target; }

	sf::RectangleShape& getShape() { return shape; };

	void setString(const std::string& ref) { 
		stext = ref;
		text.setString(ref);
	}
	sf::Text& getText() { return text; }
	void update(const float delta, const sf::Vector2i& mousePos);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
private:
	sf::RectangleShape shape;
	float* m_target;
	sf::Text text;
	std::string stext;
};

