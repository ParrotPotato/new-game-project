## Another game development from scratch project

Here is another attempt at making a game from scratch as a learning exercise for getting better at OpenGL. This time the target involves making a fully working game with physics and audio. 

We are using : 

- **SDL2** : For window context handling
- **glew** : For openGL context handling and loading all openGL function pointers.
- **TBD** : physics engine (3rd party would be prefered)
- **TBD** : mesh loading library (3rd party would be prefered)
- **TBD** : audio library (3rd party would be prefered)

#### Thoughts right now

// 2024-02-07
Current progress has made me extremely bored of what I am doing. Since I have an Idea in my head which I want to work on, I will leave the complicated model loading and texture loading and texture mapping for the time when I am willing to do something fun. In the mean time let's write a renderer which can render a cube easily on the screen.

// 2024-02-21
Fixed the issues which was causing the triangle colors to not show up when the projection, model, and view matrix were bound

#### Progress 

- [x] Setting up basic platform layer which creates SDL window with opengl 
- [x] setting up keyboard and mouse input handling 
- [x] Setting up basic shader
- [x] Setting up basic vertex renderer 
- [x] Setting up camera is done.
- [x] Setting up a basic camera with motion avaliable 
- [x] Making a descision about what the game should be like (2D/3D/Isometric)
- [x] Setting up basic texture renderer
- [x] Setting up mesh loading (would be preferable to do with a library due to previous experience)
- [ ] Settign up basic lighting
- [ ] Updating this list when above tasks are complete
- [ ] Setting up a  cube generator so that we can have some cubes running around on the screen
