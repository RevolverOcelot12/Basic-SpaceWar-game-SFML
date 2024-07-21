#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include <iostream>


int main()
{
    std::cout << "juego corriendo...\n";
    // Este es la ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ventana :D");

    // Esta es la vision que se tiene para cuando se altere el tama;o de la ventana
    sf::View view(sf::FloatRect(0, 0, 800, 600));
    window.setView(view);

    // La clase del player
    Player player;
    Bullet bullet;

    // Clase del enemigo
    Enemy enemy;

    // Este es un circulo para que no se vea tan vacio
    sf::CircleShape circle(50.f);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(100, 100);

    int contador = 0;
    
    std::vector<Enemy> enemies;
    
    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            // Este bloque hace que cuando el evento sf::Event::Resized ocurre, que es cuando el usuario
            // cambia de tamaño la ventana, otra vista se crea basada en el alto y ancho que el usuario altero
            // para luego reemplazar la vista anterior con la nueva, asi el aspecto no se pierde y la imagen no se deforma
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }



        }

        player.update(window, view, enemy.shape.getGlobalBounds());
        enemy.update(window);

        // Esto es solo para probar una cosa del player
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            player.cooldownDuration = 0.1f;
        }
        else
        {
            player.cooldownDuration = 0.25f;
        }

        for (auto& bullet : player.bullets)
        {
            enemy.update(window);
            bool enemyCollide = bullet.shape.getGlobalBounds().intersects(enemy.shape.getGlobalBounds());
            if (enemyCollide)
            {
                enemy.health -= 1;
                //std::cout << enemy.health << "\n";
            }
        }
        
        

        window.clear();
        
        
        player.draw(window);
        player.drawBullets(window);

        for (const auto& enemy : enemies)
        {
            enemy.draw(window); 
        }

        window.draw(circle);
        window.display();
    }

    return 0;
}