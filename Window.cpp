#include "Window.h"

Window::Window(int w, int h, std::string t) {
    width = w;
    height = h;
    title = t;
}

Window::Window() {}

void Window::setWidth(int w) {
    width = w;
}

void Window::setHeight(int h) {
    height = h;
}

void Window::setTitle(std::string t) {
    title = t;
}

void Window::setRGB(double r, double g, double b) {
    red = r;
    green = g;
    blue = b;
}