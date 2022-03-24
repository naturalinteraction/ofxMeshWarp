#include "ofMain.h"
#include "ofApp.h"

int main(int argc, char *argv[])
{
    ofGLFWWindowSettings settings;
    settings.windowMode = OF_FULLSCREEN;
    settings.multiMonitorFullScreen = true;

    ofCreateWindow(settings) -> setVerticalSync(false);
    ofAppGLFWWindow::listMonitors();  // prints to console
    ofRunApp(new ofApp((argc == 2 && string(argv[1])== "reset")));
}
