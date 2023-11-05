#ifndef MANDEL_H
#define MANDEL_H

#include <SFML/Graphics.hpp>

class Mandelbrot
{
public:
    Mandelbrot(int width, int height, int maxIterations = 100);
    sf::Image calculateMandelbrot() const;

    int moveUp();
    int moveDown();
    int moveRight();
    int moveLeft();
    int zoomIn();
    int zoomOut();
    void resetView();
    void log();

    double getX();
    double getY();
    double getZoom();
private:
    int width, height, maxIterations;
    double xOffset, yOffset, zoom, moveSpeed, zoomFactor, defaultZoom, coordsMul;

};

#endif // MANDEL_H
