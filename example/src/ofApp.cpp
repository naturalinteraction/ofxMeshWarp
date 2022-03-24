#include "ofxXmlSettings.h"

#include "ofApp.h"
#include "ofxMeshWarpIO.h"
#include "say.h"
using namespace std;

#define MESH_COLS  128
#define MESH_ROWS  128
#define MESH_WIDTH 1920.0

#define X_SCALE    1.0

//--------------------------------------------------------------
ofApp::ofApp(bool reset)
{
	say(reset);
	should_reset = reset;
	should_load = ! reset;
}

void ofApp::setup()
{
	// ofSetFrameRate(61);
    // ofSetVerticalSync(false);
    // ofEnableAntiAliasing();
    // ofEnableSmoothing();

	int sw = ofGetScreenWidth();
	int sh = ofGetScreenHeight();
	say(ofGetScreenWidth());
	say(ofGetScreenHeight());

	say(ofGetWindowWidth());
	say(ofGetWindowHeight());

	say(ofGetWidth());
	say(ofGetHeight());

	if (sw * sh == 0)
	{
		say("screen width or height is zero. exiting.");
		std::exit(0);
	}

	ofPixels pix_;

	ofTexture *t = new ofTexture();
	tex_.push_back(t);
	//	for (int i = 0; i < 2; i++)  // todo: load more images

	ofLoadImage(*tex_[0], "deagostini4096.jpg");
	tex_[0] -> readToPixels(pix_);  // av: elevation pixels

	float pw = tex_[0] -> getWidth();
	float ph = tex_[0] -> getHeight();

	mesh_ = make_shared<ofxMeshWarp>();
	mesh_->setup(ofRectangle(ofGetScreenWidth()/2 - pw * MESH_WIDTH / ph / 2.0, ofGetScreenHeight()/2 - MESH_WIDTH/2, pw * MESH_WIDTH / ph, MESH_WIDTH), MESH_COLS, MESH_ROWS);
	mesh_->setUVRect(ofRectangle(0, 0, pw, ph));
	printf("%f %f\n", pw, ph);
	controller_.add(mesh_);
	controller_.enable();
	controller_.setElevationPixels(pix_);
	controller_.setCenterOfProjection(4096 / 2.0, 4096 * 0.5);
}

//--------------------------------------------------------------
void ofApp::update()
{
	// ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
	if (should_reset)
	{
		should_reset = false;
		say("initial save");
		saveValues();
		saveDaMesh();
		say("done");
	}
	if (should_load)
	{
		say("initial load");
		should_load = false;
		loadValues();
		loadDaMesh();
		say("done");
	}
	mesh_->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackground(255, 0, 0, 255);

    // we'll need the viewports later on for the two projected halves
    ofRectangle viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = 1920;
    viewport.height = 1080;

	ofPushView();
	ofViewport(viewport);
	ofScale(X_SCALE, 1.0, 1.0);

	tex_[0] -> bind();
	controller_.drawFace();
	tex_[0] -> unbind();
	if (show_controller_interface)
		controller_.draw();

	ofScale(1.0, 1.0, 1.0);
	ofPopView();

    // this will be needed to show the second half of the projection
    viewport.x = 1920;
	ofPushView();
	ofViewport(viewport);
	ofScale(X_SCALE, 1.0, 1.0);

	tex_[0] -> bind();
	controller_.drawFace();
	tex_[0] -> unbind();
	// if (show_controller_interface)
	//   controller_.draw();

	ofScale(1.0, 1.0, 1.0);

	ofPopView();

	string s = "\nw,s (scale) " + to_string(my_scale) + "\n";
	s += "o,p (rotation) " + to_string(my_rotation) + "\n";
	s += "q,z (center of projection) " + to_string(controller_.center_of_projection.y) + "\n";
	s += "a,d (elevation factor) " + to_string(drama) + "\n";
	s += "arrows (translation) " + to_string(my_translation.x) + " " + to_string(my_translation.y) + "\n";
	s += "L,S (load & save)\n";
	s += "m (show/hide mesh)\n";
	s += "fps " + to_string(ofGetFrameRate()) + "\n";
	ofDrawBitmapStringHighlight(s, 1800, 100);
	ofDrawBitmapStringHighlight(string("first display"), 1920 * 0.5, 1080 * 0.5);
	ofDrawBitmapStringHighlight(string("second display"), 1920 * 1.5, 1080 * 0.5);
}

void ofApp::loadValues()
{
    ofxXmlSettings XML;
    if (! XML.loadFile("values.xml"))
    {
        whisper("could not load XML file. exit.");
        std::exit(0);  // todo
    }
    my_rotation = XML.getValue("rotation", 0.0);
    say(my_rotation);
    controller_.setRotation(my_rotation);
}

void ofApp::loadDaMesh()
{
	printf("loaDaMesh - my_translation %f %f \n", my_translation.x, my_translation.y);

	ofxMeshWarpLoad loader;
	vector<shared_ptr<ofxMeshWarp>> result = loader.load("mesh.sav");
	say("loaded");
	if(!result.empty()) {
		controller_.clear();
		mesh_ = result[0];
		controller_.add(mesh_);
		say("added");
	}
	else
		say("nothing to load.");
}

void ofApp::saveValues()
{
	ofxXmlSettings XML;
	XML.setValue("rotation", my_rotation);
	XML.saveFile("values.xml");
	say(my_rotation);
}

void ofApp::saveDaMesh()
{
	ofxMeshWarpSave saver;
	saver.addMesh(mesh_);
	saver.save("mesh.sav");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(key == 'o')
	{
		loadDaMesh();
		my_rotation -= 0.1;
		say(my_rotation);
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
		controller_.setRotation(my_rotation);
	}
	if(key == 'p')
	{
		loadDaMesh();
		my_rotation += 0.1;
		say(my_rotation);
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
		controller_.setRotation(my_rotation);
	}
	if(key == 'q')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y - 10);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}

	if(key == 'z')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y + 10);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}

	if(key == 'w')
	{
		my_scale -= 0.01;
		printf("my_scale %f\n", my_scale);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}
	if(key == 's')
	{
		my_scale += 0.01;
		printf("my_scale %f\n", my_scale);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}

	if(key == 'a')
	{
		drama -= 0.0001;
		printf("drama %f\n", drama);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}
	if(key == 'd')
	{
		drama += 0.0001;
		printf("drama %f\n", drama);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}

    glm::vec2 delta;
	switch(key) {
		case OF_KEY_UP:		delta = glm::vec2(0,-1); break;
		case OF_KEY_DOWN:	delta = glm::vec2(0, 1); break;
		case OF_KEY_LEFT:	delta = glm::vec2(-1,0); break;
		case OF_KEY_RIGHT:	delta = glm::vec2(1, 0); break;
	}
	if(glm::length2(delta) > 0)
	{
		my_translation += delta;
		printf("my_translation %f %f \n", my_translation.x, my_translation.y);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}

	switch(key) {
		case 'm':
		{
			show_controller_interface = ! show_controller_interface;
			break;
		}
		case 'S': {
			saveDaMesh();
			saveValues();
		}	break;
		case 'L': {
			loadDaMesh();
			loadValues();
			drama = 0.0;
			my_scale = 0.0;
			my_translation.x = 0.0;
			my_translation.y = 0.0;
		}	break;
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
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

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
