#include <stdlib.h>
#include <stdio.h>
#include "ofApp.h"

#define STEP 20

long mapValue(long x, long in_min, long in_max, long out_min, long out_max );
//--------------------------------------------------------------
void ofApp::setup() {

    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();//FromXML("openni/config/ofxopenni_config.xml");
    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();   // comment this out
    openNIDevice.start();
    //openNIDevice.addInfraGenerator(); // and uncomment this to see infrared generator
                                        // or press the 'i' key when running

    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
}

//--------------------------------------------------------------
void ofApp::update(){
    openNIDevice.update();
}

//--------------------------------------------------------------
void ofApp::draw(){

	//ofSetColor(255, 255, 255);

    //openNIDevice.drawDebug(); // draws all generators
    //openNIDevice.drawDepth(0, 0);
    //openNIDevice.drawImage(640, 0);
	ofSetBackgroundColor(255, 255, 255);


	ofShortPixels depthMap = openNIDevice.getDepthRawPixels();

	int depthMapIdx = 0;

	for (unsigned int i = 0; i < 480; i += STEP)
	{
		for (unsigned int j = 0; j < 640; j += STEP)
		{

			depthMapIdx = i * 640 + j;
			
//			if (depthMap[depthMapIdx] > 800)
//			{
//				ofSetColor(0, 0, 255); //blue
//				ofEllipse(j * 2, i * 2, 5, 5);
//                
//			}
//			else
//			{
//				ofSetColor(255, 0, 0); //red
//				ofEllipse(j * 2, i * 2, 10, 10);
//			}
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept,850,900,0,-9);
            std::snprintf(buf, sizeof(buf), "%d", (int)mappedDept);
            switch(mappedDept){
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    ofSetColor(mappedDept*25, mappedDept*25, mappedDept*25);
                    break;
                default:
                    ofSetColor(255,255,255);
            }
            ofRect(j*2, i*2, STEP*2, STEP*2);
            ofSetColor(0,0,0);
            ofDrawBitmapString(buf, j*2, i*2);

		}
	}

    ofSetColor(0, 0, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, ofGetHeight() - 26);

}

//--------------------------------------------------------------
namespace std
{
    inline string to_string(int _Val)
    {   // convert long long to string
        char _Buf[2 * 10];
        //snprintf(_Buf, "%d", _Val);
        return (string(_Buf));
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    switch (key) {
        case 'i':
            if (openNIDevice.isImageOn()){
                openNIDevice.removeImageGenerator();
                openNIDevice.addInfraGenerator();
                break;
            }
            if (openNIDevice.isInfraOn()){
                openNIDevice.removeInfraGenerator();
                openNIDevice.addImageGenerator();
                break;
            }
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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

long mapValue(long x, long in_min, long in_max, long out_min, long out_max ) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
}
