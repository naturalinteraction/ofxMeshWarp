#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	// ofRunApp(new ofApp());

    ofGLFWWindowSettings settings;
    settings.windowMode = OF_GAME_MODE;
    // settings.multiMonitorFullScreen = true;

    settings.setSize(1920, 1080);
    //settings.decorated = false;
    settings.setPosition(ofVec2f(0,0));
    //settings.resizable = true;
    settings.monitor = 0; // Specify monitor one
    ofCreateWindow(settings) -> setVerticalSync(false);
    ofAppGLFWWindow::listMonitors();  // prints to console
    ofRunApp(new ofApp);

    /*
    shared_ptr<ofAppBaseWindow> mainWindow = ofCreateWindow(settings);

    settings.setSize(300, 300);
    settings.setPosition(ofVec2f(0,0));
    settings.resizable = false;
    settings.monitor = 1; // Specify monitor two
    shared_ptr<ofAppBaseWindow> guiWindow = ofCreateWindow(settings);
    guiWindow->setVerticalSync(false);
    shared_ptr<ofApp> mainApp(new ofApp);

    ofRunApp(mainWindow, mainApp);
    ofRunMainLoop();

    per fare andare quanto sopra:

    settings.windowMode = ofWindowMode::OF_GAME_MODE

    oppure (funziona su windows):

    settings.decorated = false
    settings.setPosition(glm::vec2(1920.f, 0 ));
    */
}
