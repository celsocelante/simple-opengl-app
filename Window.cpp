#include "Window.h"

Window::Window(GLint w, GLint h, std::string t) {
    width = w;
    height = h;
    title = t;
}

Window::Window() {}

void Window::setWidth(GLint w) {
    width = w;
}

void Window::setHeight(GLint h) {
    height = h;
}

void Window::setTitle(std::string t) {
    title = t;
}

void Window::setRGB(GLfloat r, GLfloat g, GLfloat b) {
    red = r;
    green = g;
    blue = b;
}