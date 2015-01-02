#include "ofApp.h"

const int N = 256;		//Number of bands in spectrum
float spectrum[ N ];	//Smoothed spectrum values
float Rad = 500;		//Cloud raduis parameter
float Vel = 0.1;		//Cloud points velocity parameter
int bandRad = 0;		//Band index in spectrum, affecting Rad value
int bandVel = 100;		//Band index in spectrum, affecting Vel value

const int pointRadiusNumberOfBands = 10;
int bandPointRadius[pointRadiusNumberOfBands];
int pointRadius = 2;
int lineWidth = 1;

const int n = 300;		//Number of cloud points

//Offsets for Perlin noise calculation for points
float tx[n], ty[n];
ofPoint p[n];			//Cloud's points positions

float time0 = 0;		//Time value, used for dt computing

//--------------------------------------------------------------
void ofApp::setup(){
    //Set up sound sample
    sound.loadSound( "zis.mp3" );
    sound.setLoop( true );
    sound.play();
    
    
    for (int i=0; i<pointRadiusNumberOfBands; i++)
        bandPointRadius[i] = i;
    
    //Set spectrum values to 0
    for (int i=0; i<N; i++) {
        spectrum[i] = 0.0f;
    }
    
    //Initialize points offsets by random numbers
    for ( int j=0; j<n; j++ ) {
        tx[j] = ofRandom( 0, 1000 );
        ty[j] = ofRandom( 0, 1000 );
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    //Update sound engine
    ofSoundUpdate();
    
    //Get current spectrum with N bands
    float *val = ofSoundGetSpectrum( N );
    //We should not release memory of val,
    //because it is managed by sound engine
    
    //Update our smoothed spectrum,
    //by slowly decreasing its values and getting maximum with val
    //So we will have slowly falling peaks in spectrum
    for ( int i=0; i<N; i++ ) {
        spectrum[i] *= 0.97;	//Slow decreasing
        spectrum[i] = max( spectrum[i], val[i] );
    }
    
    //Update particles using spectrum values
    
    //Computing dt as a time between the last
    //and the current calling of update()
    float time = ofGetElapsedTimef();
    float dt = time - time0;
    dt = ofClamp( dt, 0.0, 0.1 );
    time0 = time; //Store the current time
    
    //Update Rad and Vel from spectrum
    //Note, the parameters in ofMap's were tuned for best result
    //just for current music track
    Rad = ofMap( spectrum[ bandRad ], 5, 20, 400, 800, true );
    Vel = ofMap( spectrum[ bandVel ], 0, 0.01, 0.05, 0.5 );
    
    // for every pointRadius band in spectrum, average them all out
    float avg = 0.0;
    for (int i=0; i<pointRadiusNumberOfBands; i++) {
        avg += spectrum[bandPointRadius[i]];
    }
    avg /= pointRadiusNumberOfBands;
    
    //stupid os x values are too high so I decrease them here
//    avg /= 2;
    Vel /= 2;
    
    pointRadius = ofMap( avg, 0, 1, 2, 10);
    lineWidth = pointRadius / 4;

    // pointRadius = int(avg);
    // std::cout << "the value is " << pointRadius << std::endl;
    
    //Update particles positions
    for (int j=0; j<n; j++) {
        tx[j] += Vel * dt;	//move offset
        ty[j] += Vel * dt;	//move offset
        //Calculate Perlin's noise in [-1, 1] and
        //multiply on Rad
        p[j].x = ofSignedNoise( tx[j] ) * Rad;
        p[j].y = ofSignedNoise( ty[j] ) * Rad;
    }

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground( 95,112,130);	//Set up the background
    
    //Draw background rect for spectrum
//    ofSetColor( 185,113,101 );
//    ofFill();
//    ofRect( 10, 700, N * 6, -100 );
    
    //Draw spectrum
//    ofSetColor( 0, 0, 0 );
//    for (int i=0; i<N; i++) {
//        //Draw bandRad and bandVel by black color,
//        //and other by gray color
//        if ( i == bandRad || i == bandVel ) {
//            ofSetColor( 255, 255, 255 ); //Black color
//        }
//        else {
//            ofSetColor( 128, 128, 128 ); //Gray color
//        }
//        ofRect( 10 + i * 5, 700, 3, -spectrum[i] * 100 );
//    }
    
    //Draw cloud
    
    //Move center of coordinate system to the screen center
    ofPushMatrix();
    ofTranslate( ofGetWidth() / 2, ofGetHeight() / 2 );
    
    //Draw points
    ofSetColor( 198,183,142 );
    ofFill();
    for (int i=0; i<n; i++) {
        //ofCircle( p[i], pointRadius );
    }
    
    int alpha = ofMap(Vel, 0, 0.01, 15, 30, true);
    ofSetLineWidth(lineWidth);
    ofSetColor(255,255,255,alpha);

    //Draw lines between near points
    float dist = 40;	//Threshold parameter of distance
    for (int j=0; j<n; j++) {
        for (int k=j+1; k<n; k++) {
            if ( ofDist( p[j].x, p[j].y, p[k].x, p[k].y )
                < dist ) {
                ofLine( p[j], p[k] );
            }
        }
    }
    
    //Restore coordinate system
    ofPopMatrix();
    
    ofDrawBitmapString("fps: " + ofToString(ofGetFrameRate()),10,20);
    ofDrawBitmapString("Press escape to quit the application.",10,40);
    ofDrawBitmapString("saved.mov will appear in your data folder.",10,60);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
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