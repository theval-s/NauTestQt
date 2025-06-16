# Simple Image "Viewer" with Qt6
Uses QGraphics framework to display images.

## Dependancies
Qt6.7+ (Core, Widgets) - qt6-base/qt6-base-devel or qt6 depending on your package manager  
CMake (I've set minimum required to 3.24 but I'm pretty sure it can be built with lower)

## Building
```bash
git clone https://github.com/theval-s/NauTestQt
cd NauTestQt
mkdir build && cd build
cmake ../ 
make #or ninja, depending on the generator you have as preferred
```

Tested building on MacOS 15.4 (Silicon) and ArchLinux x86_64 (didn't test on pure Windows, setting up Qt6 after sanctions is a bit tough)

## Possible improvements
Writing custom widget for GroupBox for InspectorWidget would make everything a bit more nice, but that went a bit out of my scope.  
Implementing custom class over QGraphicsPixmapItem would allow to draw borders  
Decided not to use smart pointers because Qt memory management should be good enough for this.
