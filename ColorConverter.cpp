#include "ColorConverter.h"

/*
 * Scales intervall [Min,Max] that includes f to [0,1]
 * Computes coefficients for linear interpolation
 */
std::array<int,2> ColorConverter::computeFloors(float f, int n){
    f = (f-ColorConverter::MIN)/(ColorConverter::MAX-ColorConverter::MIN);
    float a= (1-f)*n;
    int x = std::floor(a);
    int y = std::floor(255*(a-x));
    return std::array<int,2>{x,y};
}

/*
 * Linear interpolates f to rgb Color blue-green-yellow-red
 * Cuts of min and max to blue/red
 */
sf::Color ColorConverter::ScalarToRGBShort(float f){
    f+=ColorConverter::SHIFT;
    if (f<ColorConverter::MIN) return blue;
    if (f>ColorConverter::MAX) return red;
    std::array<int,2> val = ColorConverter::computeFloors(f,4);
    int r,g,b=0;
    switch(val[0]){
        case 0: r=255;g=val[1];b=0;break;
        case 1: r=255-val[1];g=255;b=0;break;
        case 2: r=0;g=255;b=val[1];break;
        case 3: r=0;g=255-val[1];b=255;break;
        case 4: return blue;
    }
    return sf::Color(r,g,b);
}

/*
 * Linear interpolates f to rgb Color magenta-blue-green-yellow-red
 * Cuts of min and max magenta/red
 */
sf::Color ColorConverter::ScalarToRGBLong(float f){
    f+=ColorConverter::SHIFT;
    if (f<ColorConverter::MIN) return magenta;
    if (f>ColorConverter::MAX) return red;
    std::array<int,2> val = ColorConverter::computeFloors(f,5);
    int r,g,b=0;
    switch(val[0]){
        case 0: r=255;g=val[1];b=0;break;
        case 1: r=255-val[1];g=255;b=0;break;
        case 2: r=0;g=255;b=val[1];break;
        case 3: r=0;g=255-val[1];b=255;break;
        case 4: r=val[1];g=0;b=255;break;
        case 5: return magenta;
    }
    return sf::Color(r,g,b);
};
