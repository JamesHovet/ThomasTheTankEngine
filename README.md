![Demo of ThomasTheTankEngine editor](./ThomasDemo.gif)
![Demo of rendered models](./ThomasModelDemo.png)

Inspired by the excellent GDC talk by the Overwatch team on their Entity Component System game engine ([link](https://youtu.be/W3aieHjyNvw)), 
reading some of the source code of Chucklefish's ECS engine that they used for WarGroove ([link](https://github.com/amzeratul/halley) and [link](https://wargroove.com/)),
and watching some of Jonathan Blow's livestreams where he works on his own game engine ([link](https://www.youtube.com/playlist?list=PLmV5I2fxaiCI9IAdFmGChKbIbenqRMi6Z)), I decided that writing my own ECS game engine would be a great big project for me to take on in the summer of 2020. 

My goals for the project are pretty simple:
- Practice developing a large codebase while keeping complexity and coupling to a minimum
- Practice writing code that is friendly to the cache and to multithreading
- Practice making and reevaluating design decisions as the project progresses. 

Big milestones already reached:
- The Entity Component System architecture itself
- autogeneration (via python scripts and #include directives) of components and their memory lifecycles
- a "Family" or component tuple system for iterating over components
- Serialization and deserialization of all entity data
- Mesh importing and rendering
- A somewhat useful editor with parent & child support.

Big Milestones to come:
- True multithreading (the groundwork is there but its early days)
- More advanced rendering
- A few working games for testing (pong probably, maybe tetris?)

Stretch goals:
- Animation system
- In-Engine CPU profiler
- extracting the engine into a dylib 

## Build instructions (macOS)
- install these dependencies, probably using brew: `cmake`, `sdl2`, `glew`, `boost` (or alternatively, just boost filesystem). 
- clone this repo and `cd` into it. 
- exectue these commands, one after another
```
cd build
cmake .
make
``` 

## Build instructions (Windows 10 64 bit, tested on bootcamp only)
- install these dependencies and place them somewhere you can keep track of: Visual Studio (Community works just fine), cmake, SDL2 (The development download, not just the .dll), GLEW, and Boost (which you will also need to build). 

- cd into `build` and run cmake, specifying where to find the dependencies like so:
```
> cmake . -D SDL2_PATH="<Location>\SDL2-2.0.12" 
-D GLEW_LOCATION="<Location>\glew-2.1.0" 
-D BOOST_ROOT="<Location>\boost_1_73_0"
```
For me, it looks like:
```
> cmake . -D SDL2_PATH="C:\Libraries\SDL2-2.0.12" 
-D GLEW_LOCATION="C:\Libraries\glew-2.1.0" 
-D BOOST_ROOT="C:\Libraries\boost_1_73_0"
```

- Open up the resulting ThomasTheTankEngine.vcxproj and compile away. 

## Acknowledgements:
- The lion and column models are from the Crytek "Sponza" scene and can be found [here](https://github.com/jimmiebergmann/Sponza)
- I forked Bly7's Obj-Loader to import model files. Here is the [original](https://github.com/Bly7/OBJ-Loader) and here's my [fork](https://github.com/JamesHovet/OBJ-Loader), which uses glm and calculates tangent vectors for each vertex. 
- I use the ever-amazing [Dear Imgui](https://github.com/ocornut/imgui) for all the UI
- I use SDL2 for window creation and os input
- I use [stb_image.h](https://github.com/nothings/stb) by Sean Barrett for image loading
- I use Boost::filesystem for platform-independent filesystem stuff and Boost::pool for an object pool implementation
- I use nlohmann's [json for modern C++](https://github.com/nlohmann/json) for json reading and writing (used for serialization everywhere)
- I use [glm](https://glm.g-truc.net/) for linear algebra
- I use [GLEW](http://glew.sourceforge.net/) for openGL extension finding, etc. 
- I am in debt to Joey de Vries of [learnopengl.com](https://learnopengl.com/) for his amazing openGL tutorials
- and finally, I would like to thank Tim Ford of blizzard for his [talk](https://www.gdcvault.com/play/1024001/-Overwatch-Gameplay-Architecture-and) on ECS that inspired this whole project. Totally want to meet this guy. 
