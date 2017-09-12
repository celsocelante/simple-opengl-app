#ifndef DATE_H
#define DATE_H

class Circle {
    private:
        double x, y, z;
        double radius;
        bool displayed;
    
    public:
        Circle(double c_x, double c_y, double c_z, double c_r);

        void setX(double c_x);
        void setY(double c_y);
        void setZ(double c_z);
        void setCoord(double c_x, double c_y, double c_z);
        void setRadius(double c_r);
        void setDisplayed(bool d);
        void draw();

        double getX() { return x; }
        double getY() { return y; }
        double getZ() { return z; }
        double getRadius() { return radius; }
        bool getDisplayed() { return displayed; }
};
#endif