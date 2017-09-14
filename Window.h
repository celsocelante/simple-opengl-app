#include <string>

class Window {
    private:
        int width;
        int height;
        std::string title;
        double red;
        double green;
        double blue;

    public:
        Window(int w, int h, std::string t);
        Window();
        void setWidth(int w);
        void setHeight(int h);
        void setTitle(std::string t);
        void setRGB(double r, double g, double b);

        int getWidth() { return width; }
        int getHeight() { return height; }
        std::string getTitle() { return title; }

        double getRed() { return red; }
        double getGreen() { return green; }
        double getBlue() { return blue; }
};