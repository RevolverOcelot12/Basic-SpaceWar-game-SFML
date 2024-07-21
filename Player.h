#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Enemy.h"

Enemy enemy;

// Esta es la bala estandar del player
class Bullet {
public:
    sf::RectangleShape shape;
    sf::Vector2f direction;
    bool isActive;
    float largo = 15;
    float ancho = 2.5;
    void draw(sf::RenderWindow& window) const
    {
        if (isActive)
        {
            window.draw(shape);
        }
    }
    Bullet() : isActive(false) {
        shape.setSize(sf::Vector2f(largo, ancho));
        shape.setFillColor(sf::Color::White);
        shape.setOrigin(sf::Vector2f(largo / 2, ancho / 2));
    }
};

// Aqui esta todo lo que tenga que ver con el player
class Player
{
public:
    // Lo que esta comentado es la forma basica del player (triangulo) para usar un sprite en su lugar,
    // no elimino esta forma en caso de tener que testear alguna cosa 
    
    //sf::ConvexShape playerShape;  // Forma del player

    sf::Texture playerTexture;     // Textura del player
    sf::Sprite playerShape;  // Sprite del player, este es el que se usa como forma
    Player()
    {
        // Esta es la forma del player, es un triangulo asi que tiene 3 puntos y el origen esta en el centro
        /*float lado = 25;   // Esto define el tama;o
        playerShape.setPointCount(3);
        playerShape.setPoint(0, sf::Vector2f(0.0f, 0.0f));
        playerShape.setPoint(1, sf::Vector2f(lado / 2, lado));
        playerShape.setPoint(2, sf::Vector2f(lado, 0.0f));
        playerShape.setFillColor(sf::Color::White);
        playerShape.setOrigin(lado / 2, lado / 2);*/

        float anchoSprite = 25;
        float largoSprite = 28;
        playerShape.setOrigin(anchoSprite / 2, largoSprite / 2);

        float positionX = 400.f;
        float positionY = 300.f;
        sf::Vector2f playerPosition(positionX, positionY); // Vector posicion
        playerShape.setPosition(playerPosition); // establece la posicion

        
        // Esto es para cargar la textura
        if (!playerTexture.loadFromFile("PlayerSpacewarProject.png"))
        {
            std::cout << "error al cargar la textura";
        }
        playerShape.setTexture(playerTexture);   // Se le asigna la textura al sprite

    }
    
    //IMPORTANTE: Para acceder a las balas en conjunto, se tiene que acceder al vector que las 
    // almacena, el que esta aca abajo, y ADEMAS el codigo para hacerlo tiene que estar dentro
    // del ciclo for que recorre el vector para verificar distancia, colisiones, posicion, de cada una de las balas
    Bullet bullet;
    std::vector<Bullet> bullets;  // Vector que almacena balas
    float cooldownDuration = 0.25f;  // Tiempo entre disparos

    void update(sf::RenderWindow& window, sf::View& view, sf::FloatRect enemigo) // Esta actualiza el estado del player
    {
        sf::Vector2f playerPosition(playerShape.getPosition());
        //std::cout << playerPosition.x << ", " << playerPosition.y << "\n";
        
        movement(window);
        handleBulletsInput(window, view);
        moveBullets(enemigo);
        
        playerRotationTowardsMouse(window, view);
        viewFollowPlayer(window, view, playerPosition);
    }

    void draw(sf::RenderWindow& window) // Esta dibuja cosas
    {
        window.draw(playerShape);
        //window.draw(sprite);
    }
    // Dibuja balas
    void drawBullets(sf::RenderWindow& window)
    {
        for (const auto& bullet : bullets)
        {
            bullet.draw(window);
        }
    }
    
    int contador = 0;
    void moveBullets(sf::FloatRect enemigo)
    {
        
        for (auto& bullet : bullets)
        {
            bool enemyCollide = bullet.shape.getGlobalBounds().intersects(enemigo);
            
            if (bullet.isActive)
            {
                
                bullet.shape.move(bullet.direction * bulletSpeed);

                // Para calcular la distancia entre el player y la bala
                sf::Vector2f currentBulletPosition = bullet.shape.getPosition();
                sf::Vector2f playerPosition = playerShape.getPosition();
                float distanceToPlayer = std::sqrt(
                    (currentBulletPosition.x - playerPosition.x) * (currentBulletPosition.x - playerPosition.x) +
                    (currentBulletPosition.y - playerPosition.y) * (currentBulletPosition.y - playerPosition.y)
                );
                if (distanceToPlayer > maxBulletDistance || enemyCollide)
                {
                    bullet.isActive = false;
                }
                
            }
        }
        // Esto remueve las balas del vector segun se cumpla la condicion, en este caso la condicion esta
        // arriba, cuando bullet.isActive = false
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
            [](const Bullet& b) { return !b.isActive; }),
            bullets.end());

    }

private:
    //sf::ConvexShape playerShape;  // Forma del player
    
    float bulletSpeed = 25;
    float maxBulletDistance = 500.f;
    bool isShooting = false;
    void playerRotationTowardsMouse(sf::RenderWindow& window, sf::View view)  // Esta es la funcion que hace que el player mire el cursor
    {
        float angle = angleTowardsMouse(window, view);

        playerShape.setRotation(angle + 270); // +270 to adjust the initial orientation
        
        //std::cout << "rotacion: " << playerShape.getRotation() << "\n";
    }
    float angleTowardsMouse(sf::RenderWindow& window, sf::View view)  // Esta devuelve el angulo entre el player y el cursor
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f playerPos = playerShape.getPosition();

        // Convert mouse position to world coordinates based on the view
        sf::Vector2f worldMousePosition = window.mapPixelToCoords(mousePosition, view);
        //Esta function es nueva para mi, hace lo siguiente:
        //Here's an explanation of window.mapPixelToCoords(pixelPosition, view):
        //    window: This is your SFML window where your game is being displayed.
        //    pixelPosition : It represents the position in pixel coordinates, usually obtained from functions 
        //    like sf::Mouse::getPosition(window), giving you the mouse's position in screen pixels.
        //    view : This parameter is an optional view used for the transformation.If omitted, it 
        //    defaults to the default view of the window.

        float dx = worldMousePosition.x - playerPos.x;
        float dy = worldMousePosition.y - playerPos.y;
        float angle = static_cast<float>(atan2(dy, dx)) * 180 / static_cast<float>(3.14159);
        return angle;
    }
    void movement(sf::RenderWindow& window)  // Revisa el teclado para mover al player
    {
        float velocityX = 0;
        float velocityY = 0;
        float velocity = 5.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            velocityX = velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            velocityX = -velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            velocityY = -velocity;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            velocityY = velocity;
        }

        // Este bloque normaliza la velocidad cuando se mueve en diagonal porque
        // por alguna razon se mueve mas rapido sin esto 
        if (velocityX != 0 && velocityY != 0) {
            velocityX *= 0.7071f; // Approximately 1/sqrt(2)
            velocityY *= 0.7071f; // Approximately 1/sqrt(2)
        }

        playerShape.move(velocityX, velocityY);
    }
    sf::Clock shootCooldown;
    
    void handleBulletsInput(sf::RenderWindow& window, sf::View view)
    {
        //std::cout << isShooting << "\n";
        Bullet newBullet;   // Nueva bala con las propiedades de la clase Bullet
        bool mouseBotonIzquierdo = sf::Mouse::isButtonPressed(sf::Mouse::Left);
        if (mouseBotonIzquierdo && shootCooldown.getElapsedTime().asSeconds() >= cooldownDuration)
        {
            isShooting = true;
            shootCooldown.restart();  // Resetea el reloj
            newBullet.shape.setRotation(angleTowardsMouse(window, view));  // Rota la bala en direccion del cursor
        }
        else
        {
            isShooting = false;
        }
        if (isShooting)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Vector2f playerPos = playerShape.getPosition();

            newBullet.shape.setPosition(playerPos);

            // Calculate direction vector from player to mouse position
            sf::Vector2f direction = mousePos - playerPos;

            // Normalize the direction vector
            float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            direction /= length;

            newBullet.direction = direction;
            newBullet.isActive = true;
            bullets.push_back(newBullet);
        }
    }
    void viewFollowPlayer(sf::RenderWindow& window, sf::View& view, 
        const sf::Vector2f& playerPosition)   // Esta hace que la ventana se centre en el player
    {
        // Set the position of the view to be centered on the player
        view.setCenter(playerPosition);
        window.setView(view);
    }
};