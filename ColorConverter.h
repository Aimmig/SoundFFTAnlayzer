#include <SFML/Graphics.hpp>
#include <math.h>

using namespace std;
using namespace sf;

class ColorConverter{

public:
    static Color ScalarToRGBShort(float f);
    static Color ScalarToRGBLong(float f);
};
