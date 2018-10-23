#include "ColorConverter.h"

sf::Color ColorConverter::ScalarToRGBShort(float f){
    //fine tuning for cutting off colours outside
    f+=ColorConverter::SHIFT;
    if (f<ColorConverter::MIN) return blue;
    if (f>ColorConverter::MAX) return red;
    //scale to 0-1
    f = (f-ColorConverter::MIN)/(ColorConverter::MAX-ColorConverter::MIN);
    float a= (1-f)/0.25;
    int x = std::floor(a);
    int y = std::floor(255*(a-x));
    int r,g,b=0;
    switch(x){
        case 0: r=255;g=y;b=0;break;
        case 1: r=255-y;g=255;b=0;break;
        case 2: r=0;g=255;b=y;break;
        case 3: r=0;g=255-y;b=255;break;
        case 4: r=0;g=0;b=255;break;
    }
    return sf::Color(r,g,b);
}

sf::Color ColorConverter::ScalarToRGBLong(float f){
    //fine tuning for cutting off colours outside
    f+=ColorConverter::SHIFT;
    if (f<ColorConverter::MIN) return magenta;
    if (f>ColorConverter::MAX) return red;
    //scale to 0-1
    f = (f-ColorConverter::MIN)/(ColorConverter::MAX-ColorConverter::MIN);
    float a= (1-f)/0.2;
    int x = std::floor(a);
    int y = std::floor(255*(a-x));
    int r,g,b=0;
    switch(x){
        case 0: r=255;g=y;b=0;break;
        case 1: r=255-y;g=255;b=0;break;
        case 2: r=0;g=255;b=y;break;
        case 3: r=0;g=255-y;b=255;break;
        case 4: r=y;g=0;b=255;break;
        case 5: r=255;g=0;b=255;break;
    }
    return sf::Color(r,g,b);
};
