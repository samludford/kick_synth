/* This is an example of how to integrate maximilain into openFrameworks,
 including using audio received for input and audio requested for output.
 
 
 You can copy and paste this and use it as a starting example.
 
 */

#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    
    
    ofSetFrameRate(60);
    ofBackground(0,0,0);
    
    // audio setup
    sampleRate 	= 44100;
    bufferSize	= 512;
    display_buffer.resize(bufferSize);
    
    ofxMaxiSettings::setup(sampleRate, 2, bufferSize);
    
    ofSetVerticalSync(true);
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    ofSoundStreamSetup(2,2,this, sampleRate, bufferSize, 4); /* this has to happen at the end of setup - it switches on the DAC */
    
    env.setAttack(attack);
    env.setDecay(1.0);
    env.setSustain(1.0);
    env.setRelease(release);
    
    pitch.setAttack(pitch_sweep);
    pitch.setDecay(1.0);
    pitch.setSustain(1.0);
    pitch.setRelease(10000.0);
    
    // setup the FFT and octave analyser
    fft.setup(1024, 512, 256);
    oct.setup(44100, 1024, 10);
    
    // gui
    params.add(attack.set("attack", 20.0, 1.0, 100.0));
    params.add(release.set("release", 60.0, 1.0, 900.0));
    params.add(pitch_sweep.set("sweep", 20.0, 1.0, 200.0));
    params.add(pitch_max.set("max pitch", 7000.0, 400.0, 10000.0));
    params.add(pitch_min.set("min pitch", 100.0, 20.0, 200.0));
    
    attack.addListener(this, &ofApp::attackChanged);
    release.addListener(this, &ofApp::releaseChanged);
    pitch_sweep.addListener(this, &ofApp::pitchSweepChanged);
    
    gui.setup(params);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    float start_x = padding;
    float start_y = ofGetHeight() / 2.0;
    
    float oct_spacing = (ofGetWidth() - padding*2.0) / (float) oct.nAverages;
    float amp_spacing = (ofGetWidth() - padding*2.0)  / (float) display_buffer.size();
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0, ofGetHeight()/2.0);
    for(int i=0 ; i < oct.nAverages ; i++) {
        
            ofNoFill();
            ofSetColor(ofMap(oct.averages[i], 0.0, 150.0, 0.0, 255.0));
            float w = ofMap(oct.averages[i], 0.0, 150.0, 0.0, ofGetWidth()-padding*2.0);
            ofDrawEllipse(0, 0, w, i * 25.0);
        
    }
    ofPopMatrix();
    
    // draw amplitude
    ofSetColor(170);
    for(int i=0 ; i < display_buffer.size() ; i+=10.0) {
        float x = start_x + amp_spacing * i;
        float y = ofMap(display_buffer[i], -1.0, 1.0, padding, ofGetHeight() - padding);
        ofDrawLine(x, start_y, x, y);
    }
    
    // draw gui
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::audioOut(float * output, int bufferSize, int nChannels) {
    
    for (int i = 0; i < bufferSize; i++){
        
        // get the current amplitude based on the amp envelope
        amp_current = env.adsr(1.0, env.trigger);
        
        // if envelope is triggered, untrigger it off immediately
        if(env.trigger == 1) {
            env.trigger = 0.0;
        }
        
        // get the value of the pitch envelope, based on trigger
        ramp_val = pitch.adsr(1.0, pitch.trigger);
        if(amp_current <= 0.001 ) {
            // untrigger the pitch env only once amplitude is low enough
            pitch.trigger = 0.0;
        }
        
        // get current frequency based on the pitch envelope and the max/min pitch
        float freq = ofMap(ramp_val, 0.0, 0.9, pitch_max, pitch_min);
        
        // get the signal of a sinewave at the frequency determined by the pitch envelope,
        // and shape its volume with the amp envelope
        wave = myOsc.sinewave(freq) * amp_current;
        
        // calculate fft and octave analyser
        if (fft.process(wave)) {
            fft.magsToDB();
            oct.calculate(fft.magnitudes);
        }

        output[i*nChannels    ] = wave;
        output[i*nChannels + 1] = wave;
        
        // store audio data in buffer for drawing
        display_buffer[i] = wave;
        
    }
    
    
}

//--------------------------------------------------------------
void ofApp::audioIn(float * input, int bufferSize, int nChannels){
    
    for(int i = 0; i < bufferSize; i++){
        /* you can also grab the data out of the arrays*/
        
    }
    
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    // when spacebar is pressed trigger amp and pitch envelopes
    if(key == ' ') {
        if(!held) {
            env.trigger = 1;
            pitch.trigger = 1;
            held = true;
        }
        
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    held = false;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

// Listeners

void ofApp::attackChanged(double &p) {
    env.setAttack(p);
}

void ofApp::releaseChanged(double &p) {
    env.setRelease(p);
}

void ofApp::pitchSweepChanged(double &p) {
    pitch.setAttack(p);
}

