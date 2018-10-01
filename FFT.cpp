#include "FFT.h"

FFT::FFT(string const& _path,int const& _bufferSize)
{
        /*std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
        for (auto dev: availableDevices){
            std::cout<<dev<<std::endl;
        }*/
	path = _path ;
	if(!buffer.loadFromFile(path)) cout<<"Unable to load buffer"<<endl ;

	sound.setBuffer(buffer) ;
	sound.setLoop(true);
	sound.play();

	VA1.setPrimitiveType(LineStrip) ;
	VA2.setPrimitiveType(Lines) ;
	VA3.setPrimitiveType(LineStrip) ;

	sampleRate = buffer.getSampleRate()*buffer.getChannelCount() ;
	sampleCount = buffer.getSampleCount() ;
	if(_bufferSize < sampleCount) bufferSize = _bufferSize ;
	else bufferSize = sampleCount ;
	mark = 0 ;

	for(int i(0) ; i < bufferSize ; i++) window.push_back(0.54-0.46*cos(2*PI*i/(float)bufferSize)) ;

	sample.resize(bufferSize) ;
	VA1.resize(bufferSize) ;
}

void FFT::hammingWindow()
{
	mark = sound.getPlayingOffset().asSeconds()*sampleRate ;
	if(mark+bufferSize < sampleCount)
	{
		for(int i(mark) ; i < bufferSize+mark ; i++)
		{
			sample[i-mark] = Complex(buffer.getSamples()[i]*window[i-mark],0) ;
			VA1[i-mark] = Vertex(Vector2f(20,250)+Vector2f((i-mark)/(float)bufferSize*700,sample[i-mark].real()*0.005),Color(255,0,0,50)) ;
		}
	}
}

//fourier transformation DONT TOUCH THIS!!
void FFT::fft(CArray &x)
{
	const int N = x.size();
	if(N <= 1) return;

	CArray even = x[slice(0,N/2,2)];
	CArray  odd = x[slice(1,N/2,2)];

	fft(even);
	fft(odd);

	for(int k = 0 ; k < N/2 ; k++)
	{
		Complex t = polar(1.0,-2 * PI * k / N) * odd[k];
		x[k] = even[k] + t;
		x[k+N/2] = even[k] - t;
	}
}

void FFT::update()
{
	hammingWindow() ;

	VA2.clear() ;
	VA3.clear() ;

	bin = CArray(sample.data(),bufferSize) ;
	fft(bin) ;
        float max = 100000000 ;

	lines(max) ;
	bars(max);
}

Color FFT::ScalarToRGBShort(float f){
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
    return Color(r,g,b);
}


Color FFT::ScalarToRGBLong(float f){
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
    return Color(r,g,b);
}


void FFT::bars(float const& max)
{
	VA2.setPrimitiveType(Lines) ;
        for(float i(3) ; i < min(bufferSize/2.f,scale) ; i*=1.01)
        {
                Vector2f samplePosition(log(i)/log(min(bufferSize/2.f,scale)),abs(bin[(int)i])) ;

                VA2.append(Vertex(position+Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ScalarToRGBLong(samplePosition.x)));
                VA2.append(Vertex(position+Vector2f(samplePosition.x*xScale,0),ScalarToRGBLong(samplePosition.x)));

                VA2.append(Vertex(position+Vector2f(samplePosition.x*xScale,0),Color(255,255,255,100))) ;
                VA2.append(Vertex(position+Vector2f(samplePosition.x*xScale,samplePosition.y/max*yScale/2.f),Color(255,255,255,0))) ;
        }
}

void FFT::prepareCascade(){
    if (useRGB){
        for(float i(std::max((double)0,cascade.size()-3e5)) ; i < cascade.size() ; i++){
		cascade[i].position -= Vector2f(-0.8,1) ;
                if (cascade[i].color.a !=0) cascade[i].color.a = transperency;
	}
    }
    else{
        for(float i(std::max((double)0,cascade.size()-3e5)) ; i < cascade.size() ; i++){
                cascade[i].position -= Vector2f(-0.8,1) ;
		if(cascade[i].color.a != 0) cascade[i].color = Color(255,255,255,transperency);
	}
    }
}

void FFT::lines(float const& max)
{
	VA3.setPrimitiveType(LineStrip) ;
	Vector2f samplePosition ;
	float colorDecay = 1 ;
        prepareCascade();
	samplePosition = Vector2f(log(3)/log(min(bufferSize/2.f,scale)),abs(bin[(int)3])) ;
	cascade.push_back(Vertex(position+Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ScalarToRGBLong(samplePosition.x))) ;
	for(float i(3) ; i < bufferSize/2.f ; i*=1.02)
	{
		samplePosition = Vector2f(log(i)/log(min(bufferSize/2.f,scale)),abs(bin[(int)i])) ;
		cascade.push_back(Vertex(position+Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ScalarToRGBLong(samplePosition.x))) ;
	}
	cascade.push_back(Vertex(position+Vector2f(samplePosition.x*xScale,-samplePosition.y/max*yScale),ScalarToRGBLong(samplePosition.x))) ;

	VA3.clear() ;
	for(int i(std::max((double)0,cascade.size()-3e5)) ; i < cascade.size() ; i++) VA3.append(cascade[i]) ;
}

void FFT::draw(RenderWindow &window)
{
	window.draw(VA1) ;
	window.draw(VA3) ;
	window.draw(VA2) ;
}
