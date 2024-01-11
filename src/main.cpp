#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

	ofGLFWWindowSettings settings;
	settings.windowMode = OF_FULLSCREEN;
	//settings.setSize(3712, 1920);
	settings.multiMonitorFullScreen = false;
	ofCreateWindow(settings);
	//ofSetupOpenGL(1280,720,OF_GAME_MODE);			// <-------- setup the GL context
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
