#include "FFT.h"

float ColorConverter::MIN = 0;
float ColorConverter::MAX = 0.8;
float ColorConverter::SHIFT = -0.1;

FFT::FFT(std::string const& _path,int const& _bufferSize, bool rgb, bool mono){
	path = _path ;
	if(!buffer.loadFromFile(path)) std::cout<<"Unable to load buffer"<<std::endl;

	sound.setBuffer(buffer);
	sound.setLoop(true);
	sound.play();

	VA1.setPrimitiveType(sf::LineStrip);
	VA2.setPrimitiveType(sf::Lines);
	VA3.setPrimitiveType(sf::LineStrip);

	sampleRate = buffer.getSampleRate()*buffer.getChannelCount();
	sampleCount = buffer.getSampleCount();
	if(_bufferSize < sampleCount) bufferSize = _bufferSize;
	else bufferSize = sampleCount;
	mark = 0 ;

        //hamming function
	for(int i(0); i < bufferSize; i++) window.push_back(0.54-0.46*cos(2*PI*i/(float)bufferSize));

	sample.resize(bufferSize);
	VA1.resize(bufferSize);

        useRGB = rgb;
        monoColor = mono;
}

//create hamming window
void FFT::hammingWindow(){
	mark = sound.getPlayingOffset().asSeconds()*sampleRate;
	if(mark+bufferSize < sampleCount){
		for(int i(mark) ; i < bufferSize+mark ; i++){
                        //read buffer into sample Array
			sample[i-mark] = Complex(buffer.getSamples()[i]*window[i-mark],0);
                        //shows raw audio
			VA1[i-mark] = sf::Vertex(sf::Vector2f(20,250)+sf::Vector2f((i-mark)/(float)bufferSize*700,sample[i-mark].real()*0.005),sf::Color(255,0,0,50));
		}
	}
}

//fourier transformation DONT TOUCH THIS!!
void FFT::fft(CArray &x){
	const int N = x.size();
	if(N <= 1) return;

	CArray even = x[std::slice(0,N/2,2)];
	CArray  odd = x[std::slice(1,N/2,2)];

	fft(even);
	fft(odd);

	for(int k = 0; k < N/2; k++){
		Complex t = std::polar(1.0,-2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k+N/2] = even[k] - t;
	}
}

void FFT::update(){
	hammingWindow();

	VA2.clear();
	VA3.clear();

	bin = CArray(sample.data(),bufferSize);
	fft(bin);

        //maximal y value for bars (peaks)
        float max = 100000000;

	lines(max);
	bars(max);
}

//logaritmich x-Scale
sf::Vector2f FFT::getSamplePosition(int index){
    return sf::Vector2f(log(index)/log(std::min(bufferSize/2.f,scale)),abs(bin[(int)index]));
}


void FFT::bars(float const& max){
	VA2.setPrimitiveType(sf::Lines);
        
        if (monoColor){
            float peakFrequeny = 0;
            float peak = 0;
            for(float i(3) ; i < std::min(bufferSize/2.f,scale); i*=granularityBars){
                sf::Vector2f samplePosition = getSamplePosition(i);
                if (samplePosition.y > peak){
                    peakFrequeny = samplePosition.x;
                    peak = samplePosition.y;
                }
            }
            sf::Color rgb = ColorConverter::ScalarToRGBLong(peakFrequeny);
            for(float i(3) ; i < std::min(bufferSize/2.f,scale); i*=granularityBars){
                addVerticesToBars(getSamplePosition(i), rgb, max);
            }
        }
        else{
            for(float i(3) ; i < std::min(bufferSize/2.f,scale); i*=granularityBars){
                sf::Vector2f samplePosition = getSamplePosition(i);
                addVerticesToBars(samplePosition,ColorConverter::ScalarToRGBLong(samplePosition.x), max);
            }
        }     
}

void FFT::addVerticesToBars(sf::Vector2f samplePosition, sf::Color rgb, float const& max){
        VA2.append(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),rgb));
        VA2.append(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,0),rgb));
        VA2.append(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,0),rgb));
        rgb.a = 0;
        VA2.append(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,samplePosition.y/max*yScale/2.f),rgb));
}

void FFT::prepareCascade(){
    if (useRGB){
        for(float i(std::max((double)0,cascade.size()-3e5)); i < cascade.size() ; i++){
		cascade[i].position -= sf::Vector2f(-0.8,1);
                if (cascade[i].color.a !=0) cascade[i].color.a = transperency;
	}
    }
    else{
        for(float i(std::max((double)0,cascade.size()-3e5)); i < cascade.size() ; i++){
                cascade[i].position -= sf::Vector2f(-0.8,1);
		if(cascade[i].color.a != 0) cascade[i].color = sf::Color(255,255,255,transperency);
	}
    }
}

void FFT::lines(float const& max){
	VA3.setPrimitiveType(sf::LineStrip);
	sf::Vector2f samplePosition;
	//float colorDecay = 1;
        prepareCascade();
        if (monoColor){
            float peakFrequeny = 0;
            float peak = 0;
            for(float i(3) ; i < std::min(bufferSize/2.f,scale); i*=granularityBars){
                sf::Vector2f samplePosition = getSamplePosition(i);
                if (samplePosition.y > peak){
                    peakFrequeny = samplePosition.x;
                    peak = samplePosition.y;
                }
            }
            sf::Color rgb = ColorConverter::ScalarToRGBLong(peakFrequeny);
            for(float i(3) ; i < std::min(bufferSize/2.f,scale); i*=granularityBars){
                samplePosition = getSamplePosition(i);
                cascade.push_back(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),rgb));
            }
        }
        else{
            samplePosition = getSamplePosition(3);
            cascade.push_back(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ColorConverter::ScalarToRGBLong(samplePosition.x)));
            for(float i(3) ; i < bufferSize/2.f; i*=granularityLines){
		samplePosition = getSamplePosition(i);
		cascade.push_back(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ColorConverter::ScalarToRGBLong(samplePosition.x)));
            }
            cascade.push_back(sf::Vertex(position+sf::Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ColorConverter::ScalarToRGBLong(samplePosition.x)));
        }
	VA3.clear();
	for(int i(std::max((double)0,cascade.size()-3e5)) ; i < cascade.size() ; i++) VA3.append(cascade[i]);
}

void FFT::draw(sf::RenderWindow &window){
	window.draw(VA1);
	window.draw(VA3);
	window.draw(VA2);
}
