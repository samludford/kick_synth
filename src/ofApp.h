#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    // Audio output and input methods
    
        void audioOut(float * output, int bufferSize, int nChannels);
        void audioIn(float * input, int bufferSize, int nChannels);

    
        int		bufferSize; /* buffer size */
        int		sampleRate;
		
    
        const float padding {40.0};
        vector<double> display_buffer;
    
        maxiOsc myOsc;
        maxiEnv env;
        maxiEnv pitch;
    
        double amp_current;
        double ramp_val;
        double wave;

    
        ofxMaxiFFT fft;
        ofxMaxiFFTOctaveAnalyzer oct;
    
        ofxPanel gui;
        ofParameterGroup params;
        ofParameter<double> attack {20.0};
        ofParameter<double> release {60.0};
        ofParameter<double> pitch_sweep {20.0};
        ofParameter<double> pitch_max {7000.0};
        ofParameter<double> pitch_min {100};
    
        void attackChanged(double &p);
        void releaseChanged(double &p);
        void pitchSweepChanged(double &p);

        bool held {false};
    
};
