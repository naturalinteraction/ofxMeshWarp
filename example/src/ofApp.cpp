#include "ofApp.h"
#include "ofxMeshWarpIO.h"
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
	ofLoadImage(tex_, "crop2hc.png");
	mesh_ = make_shared<ofxMeshWarp>();
	mesh_->setup(ofRectangle(30, 30, 1845, 919), 4, 4);
	mesh_->setUVRect(ofRectangle(0, 0, tex_.getWidth(), tex_.getHeight()));
	printf("%f %f\n", tex_.getWidth(), tex_.getHeight());
	controller_.add(mesh_);
	controller_.enable();
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

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
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
			ofxMeshWarpLoad loader;
			vector<shared_ptr<ofxMeshWarp>> result = loader.load("transformation.txt");
			if(!result.empty()) {
				controller_.clear();
				mesh_ = result[0];
				controller_.add(mesh_);
			}
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
