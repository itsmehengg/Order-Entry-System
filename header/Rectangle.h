#pragma once
#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
private:
    int width, height;

public:
    Rectangle(int w, int h);
    int area();
};

#endif
