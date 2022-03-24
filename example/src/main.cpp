#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){

    ofGLFWWindowSettings settings;
    settings.windowMode = OF_FULLSCREEN;
    settings.multiMonitorFullScreen = true;

    ofCreateWindow(settings) -> setVerticalSync(false);
    ofAppGLFWWindow::listMonitors();  // prints to console
    ofRunApp(new ofApp);
}
