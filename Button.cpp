#include "Button.h"

Button::Button(float* target)
	:m_target(target)
{}

void Button::update(const float delta, const sf::Vector2i& mousePos) {
	if (mousePos.y > shape.getPosition().y && mousePos.y < shape.getPosition().y + shape.getSize().y)
	{
		if (mousePos.x > shape.getPosition().x && mousePos.x < shape.getPosition().x + shape.getSize().x / 2)
		{
			*m_target -= 0.5 * delta;
			const std::string n = stext + " " + std::to_string(*m_target);
			text.setString(n);
		}
		if (mousePos.x > shape.getPosition().x + shape.getSize().x / 2 && mousePos.x < shape.getPosition().x + shape.getSize().x)
		{
			*m_target += 0.5 * delta;
			const std::string n = stext + " " + std::to_string(*m_target);
			text.setString(n);
		}

	}
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(shape,states);
	target.draw(text,states);
}
