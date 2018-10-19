#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <complex>
#include <valarray>
#include <math.h>

const double PI = 3.141592653589793238460;

using namespace std;
using namespace sf;

typedef complex<double> Complex;
typedef valarray<Complex> CArray;

class FFT{

public:
	FFT(string const& _path,int const& _bufferSize);

	void hammingWindow();
	void fft(CArray &x);
	void update();

	void bars(float const& max, bool monoColor);
	void lines(float const& max);

        Vector2f getSamplePosition(int index);

        void prepareCascade();
	void draw(RenderWindow &window);

        Color ScalarToRGBShort(float f);
        Color ScalarToRGBLong(float f);

private:
	string path;
	SoundBuffer buffer;
	Sound sound;

	vector<Complex> sample;
	vector<float> window;
	CArray bin;

        const Vector2f position = Vector2f(0,800);
        const int xScale = 900;
        const int yScale = 650;
        const int transperency = 35;
        const float scale = 20000.f;
        const bool useRGB = true;
        
        const float granularityLines = 1.02;
        const float granularityBars = 1.01;

	VertexArray VA1;
	VertexArray VA2;
	VertexArray VA3;
	vector<Vertex> cascade;
        vector<Color> colorCascade;

	int sampleRate;
	int sampleCount;
	int bufferSize;
	int mark;
};

