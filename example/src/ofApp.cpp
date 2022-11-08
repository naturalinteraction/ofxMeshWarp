#include "ofxXmlSettings.h"

#include "ofApp.h"
#include "ofxMeshWarpIO.h"
#include "say.h"
using namespace std;

#define MESH_ELEMENTS  128  // todo: 256 looks better
#define IMAGE_COUNT  4

// todo: values0.xml e values1.xml...
// todo: bande di crossfade tra le due mesh/viewport

//--------------------------------------------------------------
ofApp::ofApp(bool reset)
{
    say(IMAGE_SIZE_SCREEN);
    say(IMAGE_SIZE_PIXEL);

	say(reset);
	should_reset = reset;
	should_load = ! reset;
	has_been_reset = reset;
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
	float pw, ph;
	for (int i = 0; i < IMAGE_COUNT; i++)
	{
		ofTexture *t = new ofTexture();
		tex_.push_back(t);
		ofLoadImage(*tex_[i], to_string(i) + string(".jpg"));
		if (i == 0)
			tex_[i] -> readToPixels(pix_);  // av: elevation pixels

		pw = tex_[i] -> getWidth();
		ph = tex_[i] -> getHeight();

		if (pw != IMAGE_SIZE_PIXEL || ph != IMAGE_SIZE_PIXEL)
		{
			say("all images must be 4096x4096. Exiting.");
			say("image:", i);
			std::exit(0);
		}
	}
	mesh_ = make_shared<ofxMeshWarp>();
	mesh_->setup(ofRectangle(ofGetScreenWidth()/2 - pw * IMAGE_SIZE_SCREEN / ph / 2.0,
		                     ofGetScreenHeight()/2 - IMAGE_SIZE_SCREEN/2,
		                     pw * IMAGE_SIZE_SCREEN / ph,
		                     IMAGE_SIZE_SCREEN),
	                         MESH_ELEMENTS, MESH_ELEMENTS);
	mesh_->setUVRect(ofRectangle(0, 0, pw, ph));
	printf("%f %f\n", pw, ph);

	controller1.add(mesh_);
	controller1.enable();
	controller1.setElevationPixels(pix_);
	controller1.setCenterOfProjection(IMAGE_SIZE_PIXEL * 0.5, IMAGE_SIZE_PIXEL * 0.5);

	controller2.add(mesh_);
	controller2.enable();
	controller2.setElevationPixels(pix_);
	controller2.setCenterOfProjection(IMAGE_SIZE_PIXEL * 0.5, IMAGE_SIZE_PIXEL * 0.5);
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
		loadValues(controller1);
		loadValues(controller2);
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
    viewport.width = IMAGE_SIZE_SCREEN/2;
    viewport.height = IMAGE_SIZE_SCREEN * 9 / 16/2;

	ofPushView();
	ofViewport(viewport);
	ofSetupScreen();
	ofScale(controller1.x_scale, 1.0, 1.0);

	tex_[image_number] -> bind();
	controller1.drawFace();
	tex_[image_number] -> unbind();
	if (show_controller_interface)
		controller1.draw();

	ofScale(1.0, 1.0, 1.0);
	ofPopView();

    // this will be needed to show the second half of the projection
    viewport.x = IMAGE_SIZE_SCREEN/2;
	ofPushView();
	ofViewport(viewport);
	ofSetupScreen();
	ofScale(controller2.x_scale, 1.0, 1.0);

	tex_[image_number] -> bind();
	controller2.drawFace();
	tex_[image_number] -> unbind();
	if (show_controller_interface)
		controller2.draw();

	ofScale(1.0, 1.0, 1.0);

	ofPopView();

	ofxMeshWarpController &controller_ = controller1;  // todo

	string s = "\nSETUP MODE\nw,s (scale) " + to_string(controller_.my_scale) + "\n";
	s += "o,p (rotation) " + to_string(controller_.my_rotation) + "\n";
	s += "q,z (center of projection) " + to_string(controller_.center_of_projection.y) + "\n";
	s += "a,d (elevation factor) " + to_string(controller_.drama) + "\n";
	s += "arrows (translation) " + to_string(controller_.my_translation.x) + " " + to_string(controller_.my_translation.y) + "\n";
	s += "L,S (load & save)\n";
	s += "m (show/hide mesh)\nSPACE (next layer)\n";
	s += "1 (load defaults for display 1)\n";
	s += "2 (load defaults for display 2)\n";
	s += "fps " + to_string(ofGetFrameRate()) + "\n";
	s += "naturalinteract@gmail.com";

	if (! has_been_reset)
	{
		s = "\nPLAYBACK MODE\nm (show/hide mesh)\nSPACE (next layer)\n";
		s += "fps " + to_string(ofGetFrameRate()) + "\n";
		s += "naturalinteract@gmail.com";
	}
	ofDrawBitmapStringHighlight(s, 0, 0);
	ofDrawBitmapStringHighlight(string("naturalinteract@gmail.com - DISPLAY 1"), IMAGE_SIZE_SCREEN * 0.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
	ofDrawBitmapStringHighlight(string("naturalinteract@gmail.com - DISPLAY 2"), IMAGE_SIZE_SCREEN * 1.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
}

void ofApp::loadValues(ofxMeshWarpController &controller_)
{
    ofxXmlSettings XML;
    if (! XML.loadFile("values.xml"))
    {
        whisper("could not load XML file. exit.");
        std::exit(0);
    }
    controller_.my_rotation = XML.getValue("rotation", 0.0);
    say(controller_.my_rotation);
    controller_.setRotation(controller_.my_rotation);

    float copx = XML.getValue("center_of_projection_x", 0.0);
    float copy = XML.getValue("center_of_projection_y", 0.0);
    say(copx, copy);
    controller_.setCenterOfProjection(copx, copy);
	printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
	loadDaMesh(controller_);
	controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);

	controller_.x_scale = XML.getValue("x_scale", 1.0);
    say(controller_.x_scale);
 }

void ofApp::loadDaMesh(ofxMeshWarpController &controller_)
{
	printf("loaDaMesh - my_translation %f %f \n", controller_.my_translation.x, controller_.my_translation.y);

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
	ofxMeshWarpController &controller_ = controller1;  // todo

	ofxXmlSettings XML;
	XML.setValue("rotation", controller_.my_rotation);
	XML.setValue("x_scale", 1.0);
	XML.setValue("center_of_projection_x", controller_.center_of_projection.x);
	XML.setValue("center_of_projection_y", controller_.center_of_projection.y);
	XML.saveFile("values.xml");
}

void ofApp::saveDaMesh()  // todo
{
	ofxMeshWarpSave saver;
	saver.addMesh(mesh_);
	saver.save("mesh.sav");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (first_display)
		keyPressedForController(key, controller1);
	else
		keyPressedForController(key, controller2);
}

void ofApp::keyPressedForController(int key, ofxMeshWarpController &controller_)
{
	if(key == 'm')
	{
		show_controller_interface = ! show_controller_interface;
	}

	if(key == ' ')
	{
		image_number = (image_number + 1) % IMAGE_COUNT;
	}

	if (! has_been_reset)
		return;

	if(key == 'o')
	{
		loadDaMesh(controller_);
		controller_.my_rotation -= 0.02;
		say(controller_.my_rotation);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
		controller_.setRotation(controller_.my_rotation);
	}
	if(key == 'p')
	{
		loadDaMesh(controller_);
		controller_.my_rotation += 0.02;
		say(controller_.my_rotation);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
		controller_.setRotation(controller_.my_rotation);
	}

	// default values for display 1
	if (key == '1')
	{
		controller_.my_scale = 0.0025;
		controller_.setCenterOfProjection(controller_.center_of_projection.x, 3088);
		controller_.my_translation.x = 3;
		controller_.my_translation.y = 552;
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		printf("my_translation %f %f \n", controller_.my_translation.x, controller_.my_translation.y);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}

	// default values for display 2
	if (key == '2')
	{
		say("todo");
	}

	if (key == 9)  // tab
	{
		first_display = ! first_display;
	}

	if(key == 'q')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y - 2);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}
	if(key == 'z')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y + 2);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}
	if(key == 'w')
	{
		controller_.my_scale -= 0.0005;
		printf("my_scale %f\n", controller_.my_scale);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}
	if(key == 's')
	{
		controller_.my_scale += 0.0005;
		printf("my_scale %f\n", controller_.my_scale);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}

	if(key == 'a')
	{
		controller_.drama -= 0.000005;
		printf("drama %f\n", controller_.drama);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}
	if(key == 'd')
	{
		controller_.drama += 0.000005;
		printf("drama %f\n", controller_.drama);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
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
		controller_.my_translation += delta;
		printf("my_translation %f %f \n", controller_.my_translation.x, controller_.my_translation.y);
		loadDaMesh(controller_);
		controller_.elevationWarp(controller_.my_translation, controller_.my_scale, controller_.drama, controller_.my_rotation);
	}
	if(key == 'S')
	{
		saveDaMesh();
		saveValues();
		has_been_reset = false;
	}
	if(key == 'L')
	{
		loadValues(controller_);
		controller_.drama = 0.0;
		controller_.my_scale = 0.0;
		controller_.my_translation.x = 0.0;
		controller_.my_translation.y = 0.0;
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
