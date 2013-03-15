A couple of notes on using CMake with Visual Studio on Windows:

* Create the build folder in misc because win_resource.rc looks for the icon in ../quake3.ico
  (e.g. cd misc && mkdir cmakebuild && cd cmakebuild && cmake -G "Visual Studio 10" ../..)
* Opus does not seem to compile on MSVC so you'll likely want to set UseCodecOpus to OFF (-D UseCodecOpus=OFF)