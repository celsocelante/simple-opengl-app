#include <string>

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/glut.h>
#endif

class Window {
    private:
        GLint width;
        GLint height;
        std::string title;
        GLfloat red = 1;
        GLfloat green = 0;
        GLfloat blue = 0;

    public:
        Window(GLint w, GLint h, std::string t);
        Window();
        void setWidth(GLint w);
        void setHeight(GLint h);
        void setTitle(std::string t);
        void setRGB(GLfloat r, GLfloat g, GLfloat b);

        GLint getWidth() { return width; }
        GLint getHeight() { return height; }
        std::string getTitle() { return title; }

        GLfloat getRed() { return red; }
        GLfloat getGreen() { return green; }
        GLfloat getBlue() { return blue; }
};