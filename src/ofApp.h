#pragma once

#include "ofMain.h"
#include "ofxLtcReader.h"
#include "ofxHapPlayer.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void load(std::string movie);
        void audioIn(float * input, int bufferSize, int nChannels);
    
		ofSoundStream snd;
		ofxLtcReader reader;
		ofxHapPlayer player;
		ofxHapPlayer player2;

		unsigned long ltcFrame;
		unsigned long playerFrame;
		unsigned long totalSecs;
		unsigned int numResets;
		unsigned long totalFrames;
		float drift;
};
