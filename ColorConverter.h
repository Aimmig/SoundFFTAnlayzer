#include <SFML/Graphics.hpp>
#include <math.h>

class ColorConverter{

public:
    static sf::Color ScalarToRGBShort(float f);
    static sf::Color ScalarToRGBLong(float f);
};
