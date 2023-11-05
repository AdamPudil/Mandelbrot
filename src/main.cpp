#include <SFML/Graphics.hpp>
#include "mandel.h"
#include <iostream>
#include <iomanip>

void drawInfoTab(sf::RenderWindow & window, sf::Font & font, double x, double y, double zoom, double fps) {
    // Create an sf::RectangleShape to represent the square
    sf::RectangleShape square;
    square.setSize(sf::Vector2f(250, 100)); // Size of the square
    square.setPosition(10, 10);           // Position of the square's top-left corner
    square.setFillColor(sf::Color::Green);    // Color of the square

        // Create an sf::Text object and set its properties
    sf::Text text;
        
    text.setFont(font); // Use the default font

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << "FPS: "<< fps << std::fixed << std::setprecision(14) << "\nX: " << x << "\nY: " << y << "\nZoom: " << std::fixed << std::setprecision(0) << zoom;

    text.setString(oss.str()); // Set the text content
    text.setCharacterSize(16); // Set the font size
    text.setFillColor(sf::Color::Black); // Set the text color
    text.setPosition(20, 20); // Set the text position

    window.draw(square);
    window.draw(text);
}

int main()
{
    int width = 1280;
    int height = 720;
    int maxIterations = 400;
    int targetFPS = 100; // Set your desired target frame rate
    int frameCount = 0;
    double actualFPS = 0;

    sf::RenderWindow window(sf::VideoMode(width, height), "Mandelbrot Set");
    sf::Image image;
    image.create(width, height);

    sf::Font font;
    if (!font.loadFromFile("/usr/share/fonts/TTF/FiraCodeNerdFontMono-Regular.ttf"))
    {
        std::cout << "Can't load file" << std::endl;
    }

    Mandelbrot mandelbrot = Mandelbrot(width, height, maxIterations);

    sf::Clock clock;
    sf::Clock fpsClock; // For measuring FPS
    sf::Time timePerFrame = sf::seconds(1.0f / static_cast<float>(targetFPS));

    while (window.isOpen())
    {
        // Calculate elapsed time since the last FPS update
        sf::Time elapsed = fpsClock.getElapsedTime();
        frameCount++;

        // Update FPS every 1 second
        if (elapsed.asSeconds() >= 1.0) {
            actualFPS = static_cast<double>(frameCount) / elapsed.asSeconds();
            frameCount = 0;
            fpsClock.restart();
        }

        // Wait until time for one frame has passed
        sf::Time elapsedTime = clock.getElapsedTime();
        if (elapsedTime < timePerFrame) {
            sf::sleep(timePerFrame - elapsedTime);
        }
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Handle keyboard input for moving and zooming
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            mandelbrot.moveUp();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            mandelbrot.moveDown();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            mandelbrot.moveLeft();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            mandelbrot.moveRight();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            mandelbrot.zoomIn();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            mandelbrot.zoomOut();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            exit(0);
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            mandelbrot.resetView();
            
        image = mandelbrot.calculateMandelbrot();

        sf::Texture texture;
        texture.loadFromImage(image);

        sf::Sprite sprite(texture);

        window.clear();
        window.draw(sprite);
        drawInfoTab(window, font, mandelbrot.getX(), mandelbrot.getY(), mandelbrot.getZoom(), actualFPS);
        window.display();
    }

    return 0;
}
