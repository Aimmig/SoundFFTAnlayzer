#include <SFML/Graphics.hpp>
#include <math.h>

class ColorConverter{
private:
    static std::array<int,2> computeFloors(float f, int n);
public:
    static float MIN,MAX,SHIFT;
    static sf::Color ScalarToRGBShort(float f);
    static sf::Color ScalarToRGBLong(float f);
};
