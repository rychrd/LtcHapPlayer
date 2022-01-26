#include "ofApp.h"

    
//---------------------------------------------------------
void ofApp::setup(){
    
    //Setup an audio device with ofSoundStream
    ofSoundStreamSettings settings;

    snd.printDeviceList();
    vector<ofSoundDevice> devices = snd.getDeviceList();
    snd.setDeviceID(6); // set to local device 
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
    snd.setup(this, 0, 2, 48000, 256, 4); //outputs, inputs, sample rate, bufSize 256, num bufs
    
    //Setup a ltc reader by passing a pointer to this ofSoundStream and FR
    reader.setup(&snd, 30);

    load("video/sample-1080p30-Hap.avi");
    while (!player.isLoaded()) { ; }

    totalFrames = player.getTotalNumFrames();
   
    player.setLoopState(OF_LOOP_NORMAL);
   
    player.play();

    ofSetVerticalSync(true);
    ofSetBackgroundColor(0);

    numResets = 0;
}

//--------------------------------------------------------------
void ofApp::update() {

    totalSecs = reader.ltcHour() * 3600 
              + reader.ltcMinute() * 60 
              + reader.ltcSecond();
    
    ltcFrame  = totalSecs * 30.0 
              + reader.ltcFrame();

   // totalFrames = static_cast<float>(player.getTotalNumFrames());
  //  if (ltcFrame >= totalFrames)
 //   {
        float modFrame = fmod(ltcFrame, totalFrames);    // stops drift increasing each frame compared to int type
        ltcFrame = floor(modFrame);
 //   }
    //else 
    {
        //  ltcFrame = 0;
    }

    double ltcPos = ltcFrame * (1.0 / totalFrames);
    //player.setPosition(floor(fmod(ltcFrame, totalFrames)) * 1.0 / totalFrames);
    //player.setPosition(ltcPos);
    drift = (ltcPos - player.getPosition());
    //   std::cout << drift << endl;

    float speed = player.getSpeed();

    // if (player.isFrameNew())
   //  {
    if (abs(drift) > 0.1 && speed == 1)
    {
        // player.setPosition((ltcPos + 0.01));
        if (signbit(drift))
        {
            player.setSpeed(1 / abs(drift));
            numResets++;
        }
        else
        {
            player.setSpeed(1 * abs(drift));
        }
    }
    else if (abs(drift <= 0.05) && speed != 1)
    {
        player.setSpeed(1);
    }
    // }
   
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
    ofDrawBitmapString("speed:" + ofToString(player.getSpeed()), 20, 150);
    ofDrawBitmapString("player frame:" + ofToString(player.getCurrentFrame()), 20, 170);
    
}
//--------------------------------------------------------------

void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    //Each time audioIn is called, call readLtc to update ltc values - nChannels is the input index, 1 for mono
    reader.readLtc(input, nChannels, 1);
    
}
//--------------------------------------------------------------
void ofApp::load(std::string movie)
{
    ofSetWindowTitle(ofFilePath::getBaseName(movie));
    player.load(movie);
   // player.play();

}