# System requirements and Steps to run this projects onto your systems:
1. Windows is required. Only tested on Windows10 x64 as of now. 
2. Just to be safe have atleast 4GB ram.
3. Must have visual studio installed. To install visual studio on windows go to its official website and download visual studio installer, from installer download Desktop Development for C++. This should suffice.
4. Open the solution file of git repo with visual studio
5. run the app by pressing F5

# How to use Application
1. You can move in the horizontal plane using WASD keys, to rise up in vertical plane press space and to rise down in vertical plane press shift. Note that this will work even if the scene is loaded or not.
2. When you click load button, you can freely watch the scene using your mouse, you can also move around using keys as told above, this allows you to have full controll over the camera. To escape the camera mode press ESC.
3. You can see 2 windows, one has the load button and many scenes while other one has random stuff, please ignore the window with Hello World and random stuff for now. You can remove it using mouse, using the Load Camera window you can easily change the scenes, there are 3 scenes to choose from.

# Some info on Scenes
1. In first scene, the 2 big white boxes are lights, note the specular reflection on floor and bright specular reflection on the box crate will only be visible from the steel edges, as they reflect more light (this shows the application of specular maps), you need to get the correct angle using the camera to see the correct specular reflextion from the steel edges.
2. Solar system you spawn inside the sun, get out of sun using camera movements, you can see 5 planets. Note this is not to scale, it is just to test the system, so I did not spend much time on making it a perfect Solar System. The spheres have some problems such as a line through which we can see the inside things, this can be fixes easily using the correct vertex values and line drawing. But currently I have made framework to only work with traingles. I do plan to add the lines rendering in future.
3. This one loads a model. There is a backpack model with texture applied on it. Nothing more this is just to show that we can load models from output of applications like blender. We just need the .obj and .mtl formats with some changes (changing absolute texture paths to relative texture paths) and we can load any model. Including big cars and plane models! I did not include them in demo as their textures are not properly available in required format but the model loads and in many cases even texture loads successfully. `