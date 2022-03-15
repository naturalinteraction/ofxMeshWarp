#include "ofApp.h"
#include "ofxMeshWarpIO.h"
#include "say.h"
using namespace std;

#define MESH_COLS  128
#define MESH_ROWS  96

//--------------------------------------------------------------
void ofApp::setup()
{
	int sw = ofGetScreenWidth();
	int sh = ofGetScreenHeight();
	say(ofGetScreenWidth());
	say(ofGetScreenHeight());
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
	mesh_->setup(ofRectangle(1920/2 - pw * 1000.0 / ph / 2.0, 1080/2 - 500, pw * 1000.0 / ph, 1000.0), MESH_COLS, MESH_ROWS);  // todo
	mesh_->setUVRect(ofRectangle(0, 0, pw, ph));
	printf("%f %f\n", pw, ph);
	controller_.add(mesh_);
	controller_.enable();
	controller_.setElevationPixels(pix_);
	controller_.setCenterOfProjection(580/2, 580);
}

//--------------------------------------------------------------
void ofApp::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
	mesh_->update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    // we'll need the viewports later on for the two projected halves
    ofRectangle viewport;
    viewport.x = 0;
    viewport.y = 0;
    viewport.width = 1920;  // 960
    viewport.height = 1080;

	ofPushView();
	ofViewport(viewport);

	tex_[0] -> bind();
	controller_.drawFace();
	tex_[0] -> unbind();
	if (show_controller_interface)
		controller_.draw();

	ofPopView();

/*
    // this will be needed to show the second half of the projection
    viewport.x = 960;
	ofPushView();
	ofViewport(viewport);

	tex_[0] -> bind();
	controller_.drawFace();
	tex_[0] -> unbind();
	if (show_controller_interface)
		controller_.draw();

	ofPopView();
*/

	string s = "\nw,s (scale) " + to_string(my_scale) + "\n";
	s += "o,p (rotation) " + to_string(my_rotation) + "\n";
	s += "q,z (center of projection) " + to_string(controller_.center_of_projection.y) + "\n";
	s += "a,d (elevation factor) " + to_string(drama) + "\n";
	s += "arrows (translation) " + to_string(my_translation.x) + " " + to_string(my_translation.y) + "\n";
	s += "L,S (load & save)\n";
	s += "m (show/hide mesh)\n";
	s += "fps " + to_string(ofGetFrameRate()) + "\n";
	ofDrawBitmapStringHighlight(s, 100, 100);
}

void ofApp::loadDaMesh()
{
	printf("loaDaMesh - my_translation %f %f \n", my_translation.x, my_translation.y);

	ofxMeshWarpLoad loader;
	vector<shared_ptr<ofxMeshWarp>> result = loader.load("mesh.sav");
	if(!result.empty()) {
		controller_.clear();
		mesh_ = result[0];
		controller_.add(mesh_);
	}
	else
		say("nothing to load.");
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(key == 'o')
	{
		my_rotation -= 0.01;
		say(my_rotation);
		loadDaMesh();
		controller_.elevationWarp(my_translation, my_scale, drama, my_rotation);
		controller_.setRotation(my_rotation);
	}
	if(key == 'p')
	{
		my_rotation += 0.01;
		say(my_rotation);
		loadDaMesh();
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
			ofxMeshWarpSave saver;
			saver.addMesh(mesh_);
			saver.save("mesh.sav");
		}	break;
		case 'L': {
			loadDaMesh();
			drama = 0.0;
			my_scale = 0.0;
			my_translation.x = 0.0;
			my_translation.y = 0.0;
			// todo my_rotation ?
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
