#include "ofxXmlSettings.h"

#include "ofApp.h"
#include "ofxMeshWarpIO.h"
#include "say.h"
using namespace std;

#define MESH_COLS  128
#define MESH_ROWS  128
#define IMAGE_COUNT  4

// todo: DTM allineato
// todo: 2 controller separati nei due viewport
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
	                         MESH_COLS, MESH_ROWS);
	mesh_->setUVRect(ofRectangle(0, 0, pw, ph));
	printf("%f %f\n", pw, ph);
	controller_.add(mesh_);
	controller_.enable();
	controller_.setElevationPixels(pix_);
	controller_.setCenterOfProjection(IMAGE_SIZE_PIXEL * 0.5, IMAGE_SIZE_PIXEL * 0.5);
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
    viewport.width = IMAGE_SIZE_SCREEN;
    viewport.height = IMAGE_SIZE_SCREEN * 9 / 16;

	ofPushView();
	ofViewport(viewport);
	ofSetupScreen();
	ofScale(x_scale, 1.0, 1.0);

	tex_[image_number] -> bind();
	controller_.drawFace();
	tex_[image_number] -> unbind();
	if (show_controller_interface)
		controller_.draw();

	ofScale(1.0, 1.0, 1.0);
	ofPopView();

    // this will be needed to show the second half of the projection
    viewport.x = IMAGE_SIZE_SCREEN;
	ofPushView();
	ofViewport(viewport);
	ofSetupScreen();
	ofScale(x_scale, 1.0, 1.0);

	tex_[0] -> bind();
	controller_.drawFace();
	tex_[0] -> unbind();
	// if (show_controller_interface)
	//   controller_.draw();

	ofScale(1.0, 1.0, 1.0);

	ofPopView();

	string s = "\nSETUP MODE\nw,s (scale) " + to_string(my_scale) + "\n";
	s += "o,p (rotation) " + to_string(my_rotation) + "\n";
	s += "q,z (center of projection) " + to_string(controller_.center_of_projection.y) + "\n";
	s += "a,d (elevation factor) " + to_string(drama) + "\n";
	s += "arrows (translation) " + to_string(my_translation.x) + " " + to_string(my_translation.y) + "\n";
	s += "L,S (load & save)\n";
	s += "m (show/hide mesh)\n";
	s += "fps " + to_string(ofGetFrameRate()) + "\n";

	if (! has_been_reset)
	{
		s = "\nPLAYBACK MODE\nm (show/hide mesh)\n";
		s += "fps " + to_string(ofGetFrameRate()) + "\n";
	}
	ofDrawBitmapStringHighlight(s, 1200, 100);
	ofDrawBitmapStringHighlight(string("first display"), IMAGE_SIZE_SCREEN * 0.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
	ofDrawBitmapStringHighlight(string("second display"), IMAGE_SIZE_SCREEN * 1.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
}

void ofApp::loadValues()
{
    ofxXmlSettings XML;
    if (! XML.loadFile("values.xml"))
    {
        whisper("could not load XML file. exit.");
        std::exit(0);
    }
    my_rotation = XML.getValue("rotation", 0.0);
    say(my_rotation);
    controller_.setRotation(my_rotation);

    float copx = XML.getValue("center_of_projection_x", 0.0);
    float copy = XML.getValue("center_of_projection_y", 0.0);
    say(copx, copy);
    controller_.setCenterOfProjection(copx, copy);
	printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
	loadDaMesh();
	controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);

	x_scale = XML.getValue("x_scale", 1.0);
    say(x_scale);
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
	XML.setValue("x_scale", 1.0);
	XML.setValue("center_of_projection_x", controller_.center_of_projection.x);
	XML.setValue("center_of_projection_y", controller_.center_of_projection.y);
	XML.saveFile("values.xml");
}

void ofApp::saveDaMesh()
{
	ofxMeshWarpSave saver;
	saver.addMesh(mesh_);
	saver.save("mesh.sav");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
		drama -= 0.00001;
		printf("drama %f\n", drama);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
	}
	if(key == 'd')
	{
		drama += 0.00001;
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
	if(key == 'S')
	{
		saveDaMesh();
		saveValues();
		has_been_reset = false;
	}
	if(key == 'L')
	{
		loadDaMesh();
		loadValues();
		drama = 0.0;
		my_scale = 0.0;
		my_translation.x = 0.0;
		my_translation.y = 0.0;
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
