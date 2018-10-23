#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "FFT.h"

int main(int argc, char** argv){

        if (argc!=5){
            std::cout<<"Put an audio file in the Ressources folder (.wav will work, mp3 wont)"<<std::endl;
            std::cout<<"Enter the buffer size treated by the fft (powers of two works best like 16384)"<<std::endl;
            std::cout<<"Programm parameters: file.wav bufferSize useRGB(true/false) monoColor(true/false)"<<std::endl;
        }
        else{
            std::string path(argv[1]);
            int bufferSize = atoi(argv[2]);
            std::stringstream s1(argv[3]);
            std::stringstream s2(argv[4]);
            bool useRGB;
            bool monoColor;
            if(!(s1 >> std::boolalpha >> useRGB) || !(s2 >> std::boolalpha >> monoColor)) {
                std::cout<<"Error parsing boolean variables"<<std::endl;
                exit(1);
            }
            sf::RenderWindow window(sf::VideoMode(900,900,32),"Window");

            FFT fft("Ressources/"+path,bufferSize,useRGB,monoColor);

            sf::Event event;

            while(window.isOpen()){
                    while(window.pollEvent(event)) {}

                    fft.update();

                    window.clear();
                    fft.draw(window);
                    window.display();
            }
        }
        return 0;
}
