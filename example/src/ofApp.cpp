#include "ofApp.h"
#include "ofxMeshWarpIO.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup()
{
	ofLoadImage(tex_, "tondo_crop_square.jpg");
	tex_.readToPixels(pix_);

	float pw = tex_.getWidth();
	float ph = tex_.getHeight();

	mesh_ = make_shared<ofxMeshWarp>();
	mesh_->setup(ofRectangle(1920/2 - pw * 1000.0 / ph / 2.0, 1080/2 - 500, pw * 1000.0 / ph, 1000.0), 64, 64);
	mesh_->setUVRect(ofRectangle(0, 0, pw, ph));
	printf("%f %f\n", pw, ph);
	controller_.add(mesh_);
	controller_.enable();
	controller_.setPixels(pix_);
	controller_.setCenterOfProjection(580/2, 580/2);
}

//--------------------------------------------------------------
void ofApp::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate(),2));
	mesh_->update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	tex_.bind();
//	mesh_->drawMesh();
	controller_.drawFace();
	tex_.unbind();
	if (show_controller_interface)
		controller_.draw();
}

void ofApp::loadDaMesh()
{
	ofxMeshWarpLoad loader;
	vector<shared_ptr<ofxMeshWarp>> result = loader.load("transformation.txt");
	if(!result.empty()) {
		controller_.clear();
		mesh_ = result[0];
		controller_.add(mesh_);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if(key == 'q')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y - 10);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
	}

	if(key == 'z')
	{
		controller_.setCenterOfProjection(controller_.center_of_projection.x, controller_.center_of_projection.y + 10);
		printf("cop %f %f\n", controller_.center_of_projection.x, controller_.center_of_projection.y);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
	}

	if(key == 'w')
	{
		my_scale -= 0.01;
		printf("my_scale %f\n", my_scale);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
	}
	if(key == 's')
	{
		my_scale += 0.01;
		printf("my_scale %f\n", my_scale);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
	}

	if(key == 'a')
	{
		drama -= 0.0001;
		printf("drama %f\n", drama);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
	}
	if(key == 'd')
	{
		drama += 0.0001;
		printf("drama %f\n", drama);
		loadDaMesh();
		controller_.elevationWarp(my_scale, drama);
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
			saver.save("transformation.txt");
		}	break;
		case 'L': {
			loadDaMesh();
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
