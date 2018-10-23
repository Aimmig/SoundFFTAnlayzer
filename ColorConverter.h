#include <SFML/Graphics.hpp>
#include <math.h>

#define red sf::Color(255,0,0);
#define blue sf::Color(0,0,255);
#define magenta sf::Color(255,0,255);

class ColorConverter{
private:
    static std::array<int,2> computeFloors(float f, int n);
public:
    static float MIN,MAX,SHIFT;
    static sf::Color ScalarToRGBShort(float f);
    static sf::Color ScalarToRGBLong(float f);
};
