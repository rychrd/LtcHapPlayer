#include "ofApp.h"

    
//---------------------------------------------------------
void ofApp::setup(){
    
    //Setup an audio device with ofSoundStream
    ofSoundStreamSettings settings;

    snd.printDeviceList();
    vector<ofSoundDevice> devices = snd.getDeviceList();
    snd.setDeviceID(5); //'3' is my device ID, you have to replace with yours
    /*
    settings.setApi(ofSoundDevice::Api::MS_DS);
    settings.setInListener(this);
    settings.sampleRate = 48000;
    settings.numInputChannels = 2;
    settings.numOutputChannels = 0;
    settings.bufferSize = 256;
    settings.numBuffers = 4;
    settings.setInDevice(devices[6]);
    */
    snd.setup(this,0,2,48000,256,4); //0 outputs, 2 inputs, sample rate, buffer of 256, nb of buffer 4
    
    //Setup a ltc reader by passing a pointer to this ofSoundStream, expected frame rate of ltc = 30
    reader.setup(&snd, 30);

    totalFrames = player.getTotalNumFrames();
    load("sample-1080p30-Hap.avi");
    player.setLoopState(OF_LOOP_NONE);
   
    ofSetVerticalSync(true);
    ofSetBackgroundColor(0);

    numResets = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

    totalSecs = reader.ltcHour() * 3600 
              + reader.ltcMinute() * 60 
              + reader.ltcSecond();
    
    ltcFrame  = totalSecs * 30 
              + reader.ltcFrame();

    totalFrames = static_cast<float>(player.getTotalNumFrames());

    float ltcPos = ltcFrame * (1.0 / totalFrames);

   // player.setPosition(floor(fmod(currentFrame, totalFrames)) * 1.0/totalFrames)

    drift = (abs(ltcPos - player.getPosition()));
 //   std::cout << drift << endl;

    if(drift > 0.01)
    {
        player.setPosition((ltcPos));
        numResets++;
    }
      
    if(ltcFrame > 0 && !(player.isPlaying()))
    {
        player.play();
    }
    else
    {
      //  player.stop();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){

  
    ofSetColor(255, 255, 255);
    ofRectangle videoRect(0, 0, player.getWidth(), player.getHeight());
    videoRect.scaleTo(ofGetWindowRect());
  //  player.draw(videoRect.x-960, videoRect.y, videoRect.width, videoRect.height);
    player.draw(videoRect.x, videoRect.y, videoRect.width, videoRect.height);
    

    //Print ltc values
    
    int h = reader.ltcHour();
    int m = reader.ltcMinute();
    int s = reader.ltcSecond();
    int f = reader.ltcFrame();
    string ltc = ofToString(h,2,'0') + ":" + ofToString(m,2,'0') + ":" + ofToString(s,2,'0') + ":" + ofToString(f,2,'0');
    ofSetColor(255);
    ofDrawBitmapString("LTC in : " + ltc, 20, 50);
    ofDrawBitmapString("fps : " + ofToString(ofGetFrameRate()), 220, 50);
    ofDrawBitmapString("frame : " + ofToString(ltcFrame), 20, 70);
    ofDrawBitmapString("total : " + ofToString(player.getTotalNumFrames()), 20, 90);
    ofDrawBitmapString("total resets:" + ofToString(numResets), 20, 110);
    ofDrawBitmapString("drift:" + ofToString(drift), 20, 130);
    }
//--------------------------------------------------------------

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    //Each time audioIn is called, call readLtc to update ltc values
    reader.readLtc(input, nChannels, 2);
    
}
//--------------------------------------------------------------
void ofApp::load(std::string movie)
{
    ofSetWindowTitle(ofFilePath::getBaseName(movie));
    player.load(movie);
   // player.play();

}