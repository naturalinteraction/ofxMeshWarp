#pragma once

#include "ofMain.h"
#include "ofxMeshWarp.h"
#include "ofxMeshWarpManagedController.h"

class ofApp : public ofBaseApp{
	
public:
	ofApp(bool reset);
	void setup();
	void update();
	void draw();
	void loadValues(ofxMeshWarpController &controller_, string filename);
	void saveValues(ofxMeshWarpController &controller_, string filename);
	void keyPressed(int key);
	void keyPressedForController(int key, ofxMeshWarpController &controller_);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	string generateString(ofxMeshWarpController &controller_);

	void loadDaMesh(ofxMeshWarpController &controller_);
	void saveDaMesh();

private:

	std::shared_ptr<ofxMeshWarp> mesh_;
	ofxMeshWarpController controller1;
	ofxMeshWarpController controller2;
	vector <ofTexture *> tex_;
	bool show_controller_interface = true;

	bool should_reset;
	bool should_load;
	int load_count;
	bool has_been_reset;
	int image_number = 0;
	bool first_display = true;
	ofImage gradient, gradient_reversed;
	float gradient_fract = 0.03;
};
