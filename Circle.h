#ifndef DATE_H
#define DATE_H

class Circle {
    private:
        int x, y, z;
        bool displayed;
    
    public:
        void Circle(int c_x, int c_y, int c_z);

        void setX(int c_x);
        void setY(int c_y);
        void setZ(int c_z);
        void setCoord(int c_x, int c_y, int c_z);
        void setDisplayed(bool d);

        int getX() { return c_x; }
        int getY() { return c_y; }
        int getZ() { return c_z; }
        bool getDisplayed() { return displayed; }
}
#endif