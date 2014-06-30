#ifndef _TEST_APP
#define _TEST_APP
#include <ctime>
#include "ofxOpenNI.h"
#include "ofxGui.h"
#include "ofMain.h"

struct DepthPoint {
    int x;
    int y;
    long value;
};


class testApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
    void changeSceanListener();
    void chiyoPullListener();
    void drawChiyo();
    void drawDepthMap();
    void drawEarth();
    DepthPoint getMinDepthPoint();
    DepthPoint getMaxDepthPoint();
    bool touchListener(int x, int y, long threshold);
    float getStandardVariation();

    ofTrueTypeFont verdana;
    
	ofxOpenNI openNIDevice;

    int numberOfSceans = 3;
    int currentScene = 1;
    // 1 : default, blue back
    // 2 : red
    // 3 : green
    int pushGap = 1000;
    clock_t lastClicked = 0;

    ofSoundPlayer ring;
    
    // for Scene 1 : Ijime Chiyo
    ofImage defalut;
    ofImage pulling;
    ofImage depth1;
    ofImage depth2;
    ofSoundPlayer poke;
    ofSoundPlayer poke2;
    ofSoundPlayer pull;

    // for Scene 2 : Rotate Earth
    ofImage earth;
    GLUquadricObj *quadric;
    float yAngle = 0;
    float xAngle = 0;

};

#endif
