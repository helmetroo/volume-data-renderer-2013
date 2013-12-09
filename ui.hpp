/**
 * Samuel Sweet
 * CS 5610-1 Project
 *
 * Represents the application user interface.
 */

#ifndef UI_H
#define UI_H

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#include <GLUI/glui.h>
#else
#include "GL/glui.h"
#include "GL/glut.h"
#endif

#include <cstdlib>

#include "shader.hpp"
#include "scene.hpp"

typedef void (*UI_Callback)(void);
typedef void (*UI_Vector2D_Callback)(int x, int y);
typedef void (*UI_MouseClick_Callback)(int button, int state, int x, int y);
typedef void (*UI_KeyPress_Callback)(unsigned char key, int x, int y);

class UI
{
public:
  UI(const int& _width, const int& _height);

  void initGlCapabilities(void);
  void initGlut(int argc, char** argv);
  void initSceneObjects(void);

  inline void bindExitFunction(UI_Callback callback)
  {
    atexit(callback);
  }

  inline void bindReshapeFunction(UI_Vector2D_Callback callback)
  {
    GLUI_Master.set_glutReshapeFunc(callback);
  }

  inline void bindKeyPressFunction(UI_KeyPress_Callback callback)
  {
    GLUI_Master.set_glutKeyboardFunc(callback);
  }

  inline void bindMouseMoveFunction(UI_Vector2D_Callback callback)
  {
    glutMotionFunc(callback);
  }

  inline void bindMouseClickFunction(UI_MouseClick_Callback callback)
  {
    GLUI_Master.set_glutMouseFunc(callback);
  }

  inline void bindIdleFunction(UI_Callback callback)
  {
    GLUI_Master.set_glutIdleFunc(callback);
  }

  inline void bindDisplayFunction(UI_Callback callback)
  {
    glutDisplayFunc(callback);
  }

  inline void bindFileHandleFunction(GLUI_CB callback)
  {
    file_handle_callback = callback;
  }

  void initShaderSystem(void);

  void buildInterface(void);

  // Display / animation callbacks
  void display(void);
  void update(void);

  // Event callbacks
  void onClose(void);
  void onKeyDown(unsigned char key, int x, int y);
  void onWindowResize(int w, int h);
  void onWindowPerspResize(int w, int h);
  void onMouseClick(int button, int state, int x, int y);
  void onMouseMove(int x, int y);
  void onChooseFile(int control);

  inline void setBackground(float r, float g, float b, float a)
  {
    glClearColor(r, g, b, a);
  }
  
  inline void run(void)
  {
    glutMainLoop();
  }
  
private:
  int width, height;
  int main_window;

  GLUI* glui;

  GLUI_Rollout* transform_rollout;
  GLUI_Panel* transform_panel;
  GLUI_Translation* bounding_box_zoomer;
  GLUI_Rotation* bounding_box_rotater;
  
  GLUI_Rollout* file_browser_rollout;
  GLUI_Panel* file_browser_panel;
  GLUI_FileBrowser* file_browser;

  // Key components
  Scene* scene;
  ShaderSystem* shader_system_instance;

  GLUI_CB file_handle_callback;
};

#endif
