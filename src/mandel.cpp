#include "mandel.h"
#include <thread>
#include <iostream>
#include <cmath>

const int numThreads = 12;  // Number of threads to use

struct ThreadData
{
    int width;
    int height;
    int maxIterations;
    sf::Image * image;
    double xOffset;
    double yOffset;
    double zoom;
    int threadIndex;

    ThreadData() = default;

    ThreadData(int w, int h, int maxIter, sf::Image* img, double xOffset, double yOffset, double z, int id)
        : width(w), height(h), maxIterations(maxIter), image(img), xOffset(xOffset), yOffset(yOffset), zoom(z), threadIndex(id) {
        // Initialize other members if needed.
    }
};

void calculateMandelbrotThreaded(ThreadData threadData)
{
    int width = threadData.width;
    int height = threadData.height;
    int maxIterations = threadData.maxIterations;
    sf::Image* image = threadData.image;
    double xOffset = threadData.xOffset;
    double yOffset = threadData.yOffset;
    double zoom = threadData.zoom;
    int threadIndex = threadData.threadIndex;

    for (int y = threadIndex; y < height; y += numThreads)
    {
        for (int x = 0; x < width; ++x)
        {
            double normal_x = x / double(width) * 2.0 - 1.0; 
            double normal_y = y / double(height) * 2.0 - 1.0;

            double zx = 0.0;
            double zy = 0.0;

            double size_x = 2 / zoom;
            double size_y = size_x * (double(height) / double(width));

            double cx = normal_x * (size_x / double(width)) + xOffset;
            double cy = normal_y * (size_y / double(width)) + yOffset;

            int iterations = 0;

            while (zx * zx + zy * zy < 4.0 && iterations < maxIterations)
            {
                double xtemp = zx * zx - zy * zy + cx;
                zy = 2.0 * zx * zy + cy;
                zx = xtemp;

                ++iterations;
            }

            if (iterations == maxIterations || iterations < 0) {
                sf::Color color(0, 0, 0);
                image->setPixel(x, y, color);
            } else {
                double normalizedIterations = static_cast<double>(iterations) / maxIterations;
                
                //hsv hws_color = {std::modf(std::pow(normalizedIterations * 360, 1.5), &idk), 1, 1};
                //rgb rgb_color = hsv2rgb(hws_color);

                sf::Color color((1 / std::abs(0.9 - normalizedIterations)) * 255, (1 / std::abs(0.3 - normalizedIterations)) * 255, (1 / std::abs(0.6 - normalizedIterations)) * 255);
                image->setPixel(x, y, color);
            }
        }
    }
}

Mandelbrot::Mandelbrot(int width, int height, int maxIterations) {
    this->xOffset = -0.5;
    this->yOffset = 0;

    this->defaultZoom = 0.0007;
    this->zoom = this->defaultZoom;

    this->moveSpeed = 0.0001;
    this->zoomFactor = 1.1;
    this->coordsMul = 1000;

    this->width = width;
    this->height = height;
    this->maxIterations = maxIterations;
}

sf::Image Mandelbrot::calculateMandelbrot() const
{
    std::vector<std::thread> threads;
    std::vector<ThreadData> threadDataList(numThreads);
    sf::Image image;
    image.create(this->width, this->height);

    for (int i = 0; i < numThreads; ++i)
    {
        threadDataList[i] = {this->width, this->height, this->maxIterations, &image, this->xOffset, this->yOffset, this->zoom, i};
    }

    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(calculateMandelbrotThreaded, threadDataList[i]);
    }

    for (auto& thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    return image;
}

int Mandelbrot::moveUp() {
    if(this->yOffset - (this->moveSpeed / this->zoom) < -1.5)
        return -1;

    this->yOffset -= this->moveSpeed / this->zoom;
    return 0;
}

int Mandelbrot::moveDown() {
    if(this->yOffset + (this->moveSpeed / this->zoom) > 1.5)
        return -1;

    this->yOffset += this->moveSpeed / this->zoom;
    return 0;
}

int Mandelbrot::moveRight() {
    if(this->xOffset + (this->moveSpeed / this->zoom) > 1.0)
        return -1;

    this->xOffset += this->moveSpeed / this->zoom;
    return 0;
}

int Mandelbrot::moveLeft() {
    if(this->xOffset - (this->moveSpeed / this->zoom) < -2.0)
        return -1;

    this->xOffset -= this->moveSpeed / this->zoom;
    return 0;
}

int Mandelbrot::zoomIn() {
    this->zoom *= this->zoomFactor;
    return 0;
}

int Mandelbrot::zoomOut() {
    if(this->zoom / this->zoomFactor < 0.0007)
        return -1;

    this->zoom /= this->zoomFactor;
    return 0;
}

void Mandelbrot::resetView() {
    this->xOffset = -0.5;
    this->yOffset = 0;

    this->zoom = this->defaultZoom;
}

double Mandelbrot::getX() {
    return this->xOffset * this->coordsMul;
}

double Mandelbrot::getY() {
    return this->yOffset * this->coordsMul;
}

double Mandelbrot::getZoom() {
    return this->zoom * (1 / this->defaultZoom);
}