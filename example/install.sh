sudo xset -dpms s off

sudo ./install_dependencies.sh
sudo ./install_other.sh

echo "Check if the graphics card is used:"
glxinfo | grep "renderer string"
# lspci | grep VGA
# glxinfo | grep OpenGL

# nvidia-settings
# glmark2
# glxgears
