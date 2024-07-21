#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Enemy
{
public:
	sf::CircleShape shape;  // Por ahora es un circulo
	bool isActive;
	Enemy(): isActive(false)
	{
		float radio = 50.f;
		shape.setRadius(radio);
		shape.setOrigin(radio, radio);
		shape.setFillColor(sf::Color::Blue);
	}
	float health = 50.f;
	std::vector<Enemy> enemies;  // Vector que almacena enemigos
	void update(sf::RenderWindow& window)
	{
		
	}
	void draw(sf::RenderWindow& window) const
	{
		if (isActive)
		{
				std::cout << "funciona\n";
				
				window.draw(shape);
				
		}
		
	}
	
private:
	void enemiesSpawn(sf::RenderWindow& window)
	{
		Enemy newEnemy;
		bool spawnButton = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
		if (spawnButton)
		{
			sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
			shape.setPosition(mousePos);
			isActive = true;
		}
	}
};
