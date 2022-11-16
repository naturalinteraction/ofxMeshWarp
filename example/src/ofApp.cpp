#include "ofxXmlSettings.h"

#include "ofApp.h"
#include "ofxMeshWarpIO.h"
#include "say.h"
using namespace std;

#define MESH_ELEMENTS  256  // todo: 256 looks better, 128 is faster and tested
#define MAX_IMAGE_COUNT  20
int IMAGE_COUNT = 0;

ofApp::ofApp(bool reset)
{
    say(IMAGE_SIZE_SCREEN);
    say(IMAGE_SIZE_PIXEL);

	say(reset);
	should_reset = reset;
	should_load = ! reset;
	load_count = 10;
	has_been_reset = reset;
	gradient.load("gradient1.png");
	gradient_reversed.load("gradient1reversed.png");
	// mask1.load("mask1.png");
	// mask2.load("mask2.png");
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
	for (int i = 0; i < MAX_IMAGE_COUNT; i++)
	{
		string filename = "bin/data/" + to_string(i) + ".jpg";
		FILE *test = fopen(filename.c_str(), "rb");
		if (! test)
			continue;
		fclose(test);
		IMAGE_COUNT++;
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
	// controller1.enable();
	controller1.setElevationPixels(pix_);
	controller1.setCenterOfProjection(IMAGE_SIZE_PIXEL * 0.5, IMAGE_SIZE_PIXEL * 0.5);

	controller2.add(mesh_);
	// controller2.enable();
	controller2.setElevationPixels(pix_);
	controller2.setCenterOfProjection(IMAGE_SIZE_PIXEL * 0.5, IMAGE_SIZE_PIXEL * 0.5);

	say(IMAGE_COUNT);
}

//--------------------------------------------------------------
void ofApp::update()
{
	// ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
	if (should_reset)
	{
		should_reset = false;
		// saveValues(controller1, "values1.xml");
		// saveValues(controller2, "values2.xml");
		// saveDaMesh();
	}
	load_count--;
	if (load_count == 0 && ! has_been_reset)
	{
		loadValues(controller2, "values2.xml");
		loadDaMesh(controller2);
		controller2.elevationWarp(controller2.my_translation, controller2.my_scale, controller2.drama, controller2.my_rotation);
	}
	if (should_load)
	{
		should_load = false;

		loadValues(controller1, "values1.xml");
		controller1.elevationWarp(controller1.my_translation, controller1.my_scale, controller1.drama, controller1.my_rotation);
	}
	mesh_->update();
}

string ofApp::generateString(ofxMeshWarpController &controller_)
{
	string s = "\nSETUP MODE\nw,s (scale) " + to_string(controller_.my_scale) + "\n";
	s += "o,p (rotation) " + to_string(controller_.my_rotation) + "\n";
	s += "q,z (center of projection) " + to_string(controller_.center_of_projection.y) + "\n";
	s += "a,d (elevation factor) " + to_string(controller_.drama) + "\n";
	s += "r,f (gradient extension) " + to_string(gradient_fract) + "\n";
	s += "arrows (translation) " + to_string(controller_.my_translation.x) + " " + to_string(controller_.my_translation.y) + "\n";
	s += "S (save)\n";
	s += "L (load display 1)\n";
	s += "K (load display 2)\n";
	s += "m (show/hide mesh)\nSPACE (next layer)\n";
	s += "1 (load defaults for display 1)\n";
	s += "2 (load defaults for display 2)\n";
	s += "TAB (toggle editing display)\n";
	s += "editing display #" + to_string(2 - (int)first_display) + "\n";
	s += "fps " + to_string(ofGetFrameRate()) + "\n";
	s += "naturalinteract@gmail.com";

	if (! has_been_reset)
	{
		s = "\nPLAYBACK MODE\nm (show/hide mesh)\nSPACE (next image layer)\n";
		s += "\na,b,c... (jump to image layer 0,1,2...)\n";
		s += "fps " + to_string(ofGetFrameRate()) + "\n";
		s += "naturalinteract@gmail.com";
	}

	return s;
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
	ofScale(controller1.x_scale, 1.0, 1.0);

	tex_[image_number] -> bind();
	controller1.drawFace();
	tex_[image_number] -> unbind();
	if (show_controller_interface)
		controller1.draw();

	ofScale(1.0, 1.0, 1.0);
	ofPopView();

	// mask1.draw(0, 0, IMAGE_SIZE_SCREEN * 1.0, IMAGE_SIZE_SCREEN * 9 / 16);
	gradient.draw(IMAGE_SIZE_SCREEN * 0.0, IMAGE_SIZE_SCREEN * 9 / 16 * (1.0 - gradient_fract), IMAGE_SIZE_SCREEN * 1.0, IMAGE_SIZE_SCREEN * 9 / 16 * gradient_fract);

    // this will be needed to show the second half of the projection
    viewport.x = IMAGE_SIZE_SCREEN;
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

	// mask2.draw(IMAGE_SIZE_SCREEN, 0, IMAGE_SIZE_SCREEN * 1.0, IMAGE_SIZE_SCREEN * 9 / 16);
	gradient_reversed.draw(IMAGE_SIZE_SCREEN * 1.0, 0, IMAGE_SIZE_SCREEN * 1.0, IMAGE_SIZE_SCREEN * 9 / 16 * gradient_fract);

	string s;

	if (first_display)
		s = generateString(controller1);
	else
		s = generateString(controller2);

	ofSetColor(ofColor(0, 255, 0));

	ofDrawLine(IMAGE_SIZE_SCREEN * 0.5, 0, IMAGE_SIZE_SCREEN * 0.5, IMAGE_SIZE_SCREEN * 9 / 16);
	ofDrawLine(IMAGE_SIZE_SCREEN * 1.5, 0, IMAGE_SIZE_SCREEN * 1.5, IMAGE_SIZE_SCREEN * 9 / 16);

	ofDrawLine(IMAGE_SIZE_SCREEN * 0, IMAGE_SIZE_SCREEN * 9 / 16 - 1, IMAGE_SIZE_SCREEN * 1.0, IMAGE_SIZE_SCREEN * 9 / 16 - 1);
	ofDrawLine(IMAGE_SIZE_SCREEN * 1.0, 0 + 1, IMAGE_SIZE_SCREEN * 2.0, 0 + 1);

	ofSetColor(ofColor(255));

	ofDrawBitmapStringHighlight(s, 0, 0);
	ofDrawBitmapStringHighlight(s, IMAGE_SIZE_SCREEN, 0);
	ofDrawBitmapStringHighlight(string("DEMO naturalinteract@gmail.com - this is DISPLAY 1"), IMAGE_SIZE_SCREEN * 0.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
	ofDrawBitmapStringHighlight(string("DEMO naturalinteract@gmail.com - this is DISPLAY 2"), IMAGE_SIZE_SCREEN * 1.5, IMAGE_SIZE_SCREEN * 9 / 16 * 0.5);
}

void ofApp::loadValues(ofxMeshWarpController &controller_, string filename)
{
    ofxXmlSettings XML;
    if (! XML.loadFile(filename))
    {
        whisper("could not load XML file. exit.");
        std::exit(0);
    }

    gradient_fract = XML.getValue("gradient_fract", 0.0);

    controller_.my_translation.x = XML.getValue("my_translation_x", 0.0);
    controller_.my_translation.y = XML.getValue("my_translation_y", 0.0);

    controller_.my_scale = XML.getValue("my_scale", 0.0);
    controller_.drama = XML.getValue("drama", 0.0);

    controller_.my_rotation = XML.getValue("rotation", 0.0);
    say(controller_.my_rotation);
    controller_.setRotation(controller_.my_rotation);

    float copx = XML.getValue("center_of_projection_x", 0.0);
    float copy = XML.getValue("center_of_projection_y", 0.0);
    say(copx, copy);
    controller_.setCenterOfProjection(copx, copy);
	printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);

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
		controller_.add(result[0]);
		say("added");
	}
	else
		say("nothing to load.");
}

void ofApp::saveValues(ofxMeshWarpController &controller_, string filename)
{
	ofxXmlSettings XML;
	XML.setValue("gradient_fract", gradient_fract);
	XML.setValue("my_translation_x", controller_.my_translation.x);
	XML.setValue("my_translation_y", controller_.my_translation.y);
	XML.setValue("drama", controller_.drama);
	XML.setValue("my_scale", controller_.my_scale);
	XML.setValue("rotation", controller_.my_rotation);
	XML.setValue("x_scale", 1.0);
	XML.setValue("center_of_projection_x", controller_.center_of_projection.x);
	XML.setValue("center_of_projection_y", controller_.center_of_projection.y);
	XML.saveFile(filename);
}

void ofApp::saveDaMesh()
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
	{
		int potential_image_number = key - 'a';
		if (potential_image_number >= 0 &&
			potential_image_number < IMAGE_COUNT)
			image_number = potential_image_number;
		return;
	}

	if(key == 'r')
	{
		gradient_fract += 0.001;
		if (gradient_fract < 0)
			gradient_fract = 0;
	}
	
	if(key == 'f')
	{
		gradient_fract -= 0.001;
		if (gradient_fract < 0)
			gradient_fract = 0;
	}
	
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
		gradient_fract = 0.062999949;
		controller1.my_scale = 0.0025;
		controller1.setCenterOfProjection(controller1.center_of_projection.x, 3088);
		controller1.my_translation.x = 3;
		controller1.my_translation.y = 552;
		controller1.drama = 0.00004;
		printf("cop %f %f\n", controller1.center_of_projection.x, controller1.center_of_projection.y);
		printf("my_translation %f %f \n", controller1.my_translation.x, controller1.my_translation.y);
		loadDaMesh(controller1);
		controller1.elevationWarp(controller1.my_translation, controller1.my_scale, controller1.drama, controller1.my_rotation);
	}
	// default values for display 2
	if (key == '2')
	{
		gradient_fract = 0.062999949;
		controller2.my_scale = 0.0025;  // 0.008
		controller2.setCenterOfProjection(controller2.center_of_projection.x, 1058);
		controller2.my_translation.x = 6;
		controller2.my_translation.y = -426;
		controller2.drama = 0.00004;
		printf("cop %f %f\n", controller2.center_of_projection.x, controller2.center_of_projection.y);
		printf("my_translation %f %f \n", controller2.my_translation.x, controller2.my_translation.y);
		loadDaMesh(controller2);
		controller2.elevationWarp(controller2.my_translation, controller2.my_scale, controller2.drama, controller2.my_rotation);
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
		// saveDaMesh();
		saveValues(controller1, "values1.xml");
		saveValues(controller2, "values2.xml");
		has_been_reset = false;
	}
	if (key == 'K')
	{
		loadValues(controller2, "values2.xml");
		loadDaMesh(controller2);
		controller2.elevationWarp(controller2.my_translation, controller2.my_scale, controller2.drama, controller2.my_rotation);
	}
	if (key == 'L')
	{
		loadValues(controller1, "values1.xml");
		loadDaMesh(controller1);
		controller1.elevationWarp(controller1.my_translation, controller1.my_scale, controller1.drama, controller1.my_rotation);
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
