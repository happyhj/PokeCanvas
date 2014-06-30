#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "testApp.h"

#define STEP 20

long mapValue(long x);
long _mapValue(long x, long in_min, long in_max, long out_min, long out_max );


//--------------------------------------------------------------
void testApp::setup() {

    ofSetLogLevel(OF_LOG_VERBOSE);

    openNIDevice.setup();//FromXML("openni/config/ofxopenni_config.xml");
    openNIDevice.setLogLevel(OF_LOG_VERBOSE);
    openNIDevice.addDepthGenerator();
    openNIDevice.addImageGenerator();   // comment this out
    openNIDevice.start();
    //openNIDevice.addInfraGenerator(); // and uncomment this to see infrared generator
                                        // or press the 'i' key when running

    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    ring.loadSound("BottleCork.mp3");

    
    // for Scene 1 : Chiyo
    defalut.loadImage("images/defalt.jpg");
    pulling.loadImage("images/pulling.jpg");
    depth1.loadImage("images/1depth.jpg");
    depth2.loadImage("images/2depth.jpg");
    
    defalut.resize(640*2, 480*2);
    pulling.resize(640*2, 480*2);
    depth1.resize(640*2, 480*2);
    depth2.resize(640*2, 480*2);
    poke.loadSound("ah.mp3");
    poke2.loadSound("ahu.mp3");
    pull.loadSound("why.mp3");
    
    // for Scene 2 : Earth
    ofBackground(0,0,0);
    
    //ofSetFrameRate(30);
    ofSetVerticalSync(true);
    
    glEnable(GL_DEPTH_TEST); //enable depth comparisons and update the depth buffer
    ofDisableArbTex(); //needed for textures to work with gluSphere
    earth.loadImage("images/earth.jpg");
    
    //prepare quadric for sphere
    quadric = gluNewQuadric();
    gluQuadricTexture(quadric, GL_TRUE);
    gluQuadricNormals(quadric, GLU_SMOOTH);
}


//--------------------------------------------------------------
void testApp::update(){
    openNIDevice.update();
    
    changeSceanListener();
}

void testApp::changeSceanListener(){
    if(touchListener(10, 10, 35)){
        if(clock()-lastClicked > 2000000){
            currentScene++;
            if(currentScene > numberOfSceans) {
                currentScene = currentScene/numberOfSceans;
            }
            lastClicked = clock();
            ring.play();
        }

    }
}

void testApp::chiyoPullListener(){
    DepthPoint min = getMinDepthPoint();
    DepthPoint max = getMaxDepthPoint();

    printf("%lf",min.value);
    if(min.x > 0 && min.value < -8){
    if(clock()-lastClicked > 2000000){
            lastClicked = clock();
            pull.play();
        
    }}
    if(min.x > 0 && max.value > 20 && max.value < 35){
        if(clock()-lastClicked > 2000000){
            lastClicked = clock();
            poke.play();
            
        }}
    if(min.x > 0 && max.value > 50){
        if(clock()-lastClicked > 2000000){
            lastClicked = clock();
            poke2.play();
            
        }}
}


bool testApp::touchListener(int x, int y, long threshold){
    ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
 	int depthMapIdx = 0;
    int i,j;
    i= y;
    j= x;
    depthMapIdx = i * 640 + j;
    unsigned int dept = depthMap[depthMapIdx];
    long mappedDept = mapValue(dept);

    if(mappedDept > threshold) {
        return true;
    } else {
        return false;
    }
}


//--------------------------------------------------------------
void testApp::draw(){

//drawDepthMap();
    
    switch(currentScene){
        case 1:
            drawDepthMap();
            break;
        case 2:
            drawChiyo();
            break;
        case 3:
            drawEarth();
            break;
        default:
            //
            break;
    }


/*
    ofSetColor(0, 0, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate());
	verdana.drawString(msg, 20, ofGetHeight() - 26);
*/
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

void testApp::drawEarth(){
    DepthPoint max = getMaxDepthPoint();

    //change origin to center
    ofSetColor(255, 255, 255);
    
    
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2, 0);
    
    //rotate sphere over time
    if(max.x > 0) {
        //ofRotateY(ofGetFrameNum());
 //       ofRotateY(ofGetFrameNum()*0.01 * (max.y - 180));
        yAngle = yAngle + (180 - max.y)/100.0;
        ofRotateX(yAngle);
        xAngle = xAngle + (320 - max.x)/100.0;
        ofRotateY(xAngle);
    } else {
        ofRotateX(yAngle);
        ofRotateY(xAngle);
    }

    
    //bind and draw texture
    earth.getTextureReference().bind();
    gluSphere(quadric, 130, 100, 100);
}

void testApp::drawDepthMap(){
	ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
	int depthMapIdx = 0;

    DepthPoint max = getMaxDepthPoint();
 //   DepthPoint min = getMinDepthPoint();

	float sv = getStandardVariation();
    for (unsigned int i = 0; i < 360; i += STEP)
	{
		for (unsigned int j = STEP; j < 640; j += STEP)
		{
			depthMapIdx = i * 640 + j;
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept);

            std::snprintf(buf, sizeof(buf), "%d", (int)mappedDept);
            
            if(mappedDept < -255) {
                ofSetColor(0, 255,0);
            } else if(mappedDept > 255) {
                ofSetColor(255, 0,0);
            } else if(abs(mappedDept) <= 255) {
                ofSetColor(abs(mappedDept),abs(mappedDept),abs(mappedDept));
            }
            
            if(max.x == j && max.y == i && sv > 12) {
                if(max.value > 0){
                    ofSetColor(max.value*7,0,0);
                }
            }
            /*
            if(min.x == j && min.y == i ) {
                if(min.value < 0){
                    ofSetColor(0,0,min.value*7);
                }
            }
             */
            ofRect(j*2, i*2, STEP*2, STEP*2);
            ofSetColor(112,112,112);
            ofDrawBitmapString(buf, j*2, i*2);
		}
	}
   
    ofSetColor(0, 255,0);
    ofRect(20,20, STEP*2, STEP*2);
}


DepthPoint testApp::getMinDepthPoint(){
    DepthPoint minPoint;
    minPoint.x = -1;
    minPoint.y = -1;
    minPoint.value = 0;
    
    // get max idx
    int depthMapIdx = 0;
    long depthMinValue = 0;
    int depthMinIdx = 0;
    
    ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
    for (unsigned int i = 0; i < 360; i += STEP)
    {
        for (unsigned int j = STEP; j < 640; j += STEP)
        {
            depthMapIdx = i * 640 + j;
            
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept);
            
            if (depthMinValue > mappedDept) {
                depthMinIdx = depthMapIdx;
                minPoint.value = mappedDept;
                minPoint.y = i;
                minPoint.x = j;
            }
        }
    }

    return minPoint;
}

DepthPoint testApp::getMaxDepthPoint(){
    DepthPoint maxPoint;
    maxPoint.x = -1;
    maxPoint.y = -1;
    maxPoint.value = 0;
    
    
    // get max idx
    int depthMapIdx = 0;
    long depthMaxValue = 0;
    int depthMaxIdx = 0;
    
    ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
    for (unsigned int i = 0; i < 360; i += STEP)
    {
        for (unsigned int j = STEP; j < 640; j += STEP)
        {
            depthMapIdx = i * 640 + j;
            
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept);
            
            if ((depthMaxValue < mappedDept)) {
                if((mappedDept > 100)) {
                    continue;
                }
                depthMaxValue = mappedDept;
                maxPoint.value = mappedDept;
                maxPoint.y = i;
                maxPoint.x = j;
            }
        }
    }
    return maxPoint;
}

float testApp::getStandardVariation(){
    int n=0, i;
    long sum=0;
    float mean, var;

    ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
    int depthMapIdx = 0;

    for (unsigned int i = 0; i < 360; i += STEP)
    {
        for (unsigned int j = STEP; j < 640; j += STEP)
        {
            n++;
            depthMapIdx = i * 640 + j;
            
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept);
            sum+=mappedDept;
        }
    }
    mean=sum/n;
    sum=0;

    for (unsigned int i = 0; i < 360; i += STEP)
    {
        for (unsigned int j = STEP; j < 640; j += STEP)
        {
            depthMapIdx = i * 640 + j;
            
            char buf[10];
            unsigned int dept = depthMap[depthMapIdx];
            long mappedDept = mapValue(dept);
            sum+=(mappedDept-mean)*(mappedDept-mean);
        }
    }
    var = sum/(n-1);
    //printf("평균 = %8.3f\n", mean);
    //printf("분산 = %8.3f\n", var);
    //printf("표준편차 = %8.3f\n", sqrt(var));
    
    return sqrt(var);
}

void testApp::drawChiyo(){
        ofSetBackgroundColor(255, 255, 255);
        ofShortPixels depthMap = openNIDevice.getDepthRawPixels();
    
        chiyoPullListener();
   
        
        int depthMapIdx = 0;
        
        for (unsigned int i = 0; i < 360; i += STEP)
        {
            for (unsigned int j = STEP; j < 640; j += STEP)
            {
                
                depthMapIdx = i * 640 + j;
                
                char buf[10];
                unsigned int dept = depthMap[depthMapIdx];
                long mappedDept = mapValue(dept);
                std::snprintf(buf, sizeof(buf), "%d", (int)mappedDept);
                
                ofSetColor(255);
 
                if(abs(mappedDept) < 10) {
                    defalut.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                } else if(mappedDept <= -10) {
                    pulling.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                } else if(mappedDept >= 10 && mappedDept < 35) {
                    depth1.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                } else if(mappedDept >= 35) {
                    depth2.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                }
                
                /*
                if(5 < mappedDept && mappedDept< 12){
                    depth1.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                    
                }else if(12 < mappedDept && mappedDept < 28){
                    depth2.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                    
                }else if(-20 < mappedDept && mappedDept < -5){
                    pulling.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                    
                }else{
                    defalut.drawSubsection(j*2, i*2, STEP*2, STEP*2, j*2, i*2);
                }
                 */
            }
        }
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

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
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

long mapValue(long x) {
    long x_in = x;
    long in_min = 700;
    long in_max = 820;
    long out_min = 40;
    long out_max = -40;

    /*
    if(x_in < in_min) {
        x_in = in_min;
    }
    if(x_in > in_max) {
        x_in = in_max;
    }
    */
    
    long result = _mapValue( x_in,  in_min,  in_max,  out_min,  out_max );
    
    
    
    return result;
}
long _mapValue(long x, long in_min, long in_max, long out_min, long out_max ) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min ;
}
