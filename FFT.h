#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <complex>
#include <valarray>
#include <math.h>
#include "ColorConverter.h"

const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

class FFT{

public:
	FFT(std::string const& _path,int const& _bufferSize, bool rgb, bool mono);

	void hammingWindow();
	void fft(CArray &x);
	void update();

        void addVerticesToBars(sf::Vector2f samplePosition, sf::Color rgb, float const& max);
	void bars(float const& max);
	void lines(float const& max);

        sf::Vector2f getSamplePosition(int index);

        void prepareCascade();
	void draw(sf::RenderWindow &window);

private:
	std::string path;
	sf::SoundBuffer buffer;
	sf::Sound sound;

	std::vector<Complex> sample;
	std::vector<float> window;
	CArray bin;

        const sf::Vector2f position = sf::Vector2f(0,800);
        const int xScale = 900;
        const int yScale = 650;
        const int transperency = 35;
        const float scale = 20000.f;
        bool useRGB;
        bool monoColor;
        
        const float granularityLines = 1.02;
        const float granularityBars = 1.01;

	sf::VertexArray VA1;
	sf::VertexArray VA2;
	sf::VertexArray VA3;
	std::vector<sf::Vertex> cascade;
        std::vector<sf::Color> colorCascade;

	int sampleRate;
	int sampleCount;
	int bufferSize;
	int mark;
};

