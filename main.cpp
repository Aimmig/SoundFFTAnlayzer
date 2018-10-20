#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FFT.h"

int main(){

        sf::RenderWindow window(sf::VideoMode(900,900,32),"Window");

        std::string path;
        int bufferSize;
        std::cout<<"Put an audio file in the Ressources folder (.wav will work, mp3 wont)"<<std::endl;
        std::cout<<"Enter the file name (example.wav) : ";
        std::cin>>path;
        std::cout<<"Enter the buffer size treated by the fft (powers of two works best like 16384)"<<std::endl;
        std::cin>>bufferSize;

        bool useRGB = true;
        bool monoColor = false;

        FFT fft("Ressources/"+path,bufferSize,useRGB,monoColor);

        sf::Event event;

        while(window.isOpen()){
                while(window.pollEvent(event)) {}

                fft.update();

                window.clear();
                fft.draw(window);
                window.display();
        }
        return 0;
}
