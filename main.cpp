/**
 * Samuel Sweet
 * CS 5610-1 Project
 *
 * The main program. Handles user interactions
 * as appropriate.
 */

#ifdef __APPLE__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#include "GLUT/glut.h"
#include <GLUI/glui.h>
#else
#include "GL/glui.h"
#include "GL/glut.h"
#endif

#include "scene.hpp"
#include "shader.hpp"
#include "matrixstack.hpp"

// The scene
Scene* scene;

// Matrix stack
MatrixStack* matrixStack;

// Program break-in
int main(int argc, char **argv);

// UI components
GLUI* glui;
GLUI_Rollout* shadow_choice_rollout;
GLUI_Rollout* light_position_rollout;
GLUI_Panel* light_transform_panel;
GLUI_Translation* light_xz_trans;
GLUI_Translation* light_y_trans;
int main_window;

// Display / animation callbacks
void display();
void update(void);

// Event callbacks
void onKeyDown(unsigned char key, int x, int y);
void onWindowResize(int w, int h);
void onMouseClick(int button, int state, int x, int y);
void onMouseMove(int x, int y);

// Interactivity for controlling camera and shading type
void toggleShading(unsigned char which);

// Shader system
ShaderSystem* shaderSystem;

void display()
{
  // Use current shader program
  ShaderSystem::useCurrentShader();

  /* Clear depth and color buffers to prevent dirty writing */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  // Start working in world space.
  glMatrixMode(GL_MODELVIEW); 
  glLoadIdentity(); 
  MatrixStack::matrixMode(MatrixStack::WORLD);
  MatrixStack::loadIdentity();

  MatrixStack::matrixMode(MatrixStack::VIEW);
  MatrixStack::loadIdentity();

  MatrixStack::matrixMode(MatrixStack::WORLD);

  // Draw elements.
  scene->render();
  
  // Bind transformation / projection matrices.
  MatrixStack::bindWorldMatrix();
  MatrixStack::bindViewMatrix();
  MatrixStack::bindProjectionMatrix();
  MatrixStack::bindTextureMatrix();

  // Swap buffers
  glutSwapBuffers();
}

// Correctly scales all objects when the window is resized.
void onWindowResize(int w, int h)
{
  int tx, ty = 0;
  GLUI_Master.get_viewport_area(&tx, &ty, &w, &h);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION); MatrixStack::matrixMode(MatrixStack::PROJECTION);
  glLoadIdentity(); MatrixStack::loadIdentity();

  // We are rendering just two triangles, so no fancy projection.
  glOrtho(0, w, h, 0, -1, 1);
  MatrixStack::ortho(w, h);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  MatrixStack::matrixMode(MatrixStack::VIEW);
  MatrixStack::loadIdentity();

  MatrixStack::matrixMode(MatrixStack::WORLD);
  MatrixStack::loadIdentity();

  glutPostRedisplay();
}

void onWindowPerspResize(int w, int h)
{
  int tx, ty = 0;
  GLUI_Master.get_viewport_area(&tx, &ty, &w, &h);

  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION); MatrixStack::matrixMode(MatrixStack::PROJECTION);
  glLoadIdentity(); MatrixStack::loadIdentity();

  // We are rendering just two triangles, so no fancy projection.
  gluPerspective(45.0f, (GLfloat)w/(GLfloat)h, 0.01f, 400.0f);
  MatrixStack::perspective(45.0f, (GLfloat)w/(GLfloat)h, 0.01f, 400.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  MatrixStack::matrixMode(MatrixStack::VIEW);
  MatrixStack::loadIdentity();

  MatrixStack::matrixMode(MatrixStack::WORLD);
  MatrixStack::loadIdentity();

  glutPostRedisplay();
}

// Drives all animations.
void update(void)
{
  // Bind to window context.
  if(glutGetWindow() != main_window)
    glutSetWindow(main_window);

  glutPostRedisplay();
}

// Catches mouse up/down events.
void onMouseClick(int button, int state, int x, int y)
{
  scene->getCamera()->updateMouseState(button, state, x, y);
  glutPostRedisplay();
}

// Catches mouse move events.
// Implements the trackball.
void onMouseMove(int x, int y)
{
  scene->getCamera()->updateRotation(x, y);
  glutPostRedisplay();
}

// Handles interactions when the keyboard is pressed
void onKeyDown(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
    }

  glui->sync_live(); 
  glutPostRedisplay();
}

void onClose(void)
{
  delete shaderSystem;
  delete scene;
}

void initInterface() 
{
  // Create interface
  glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

  // Quit
  glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

  // Bind interface to window
  glui->set_main_gfx_window(main_window);
}

int main(int argc, char **argv)
{
  // Matrix stack
  matrixStack = new MatrixStack;

  // Init GLUT with GLUI
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(768, 512);
  main_window = glutCreateWindow("VolRay");
  GLUI_Master.set_glutReshapeFunc(onWindowResize);
  GLUI_Master.set_glutKeyboardFunc(onKeyDown);
  glutDisplayFunc(display);
  atexit(onClose);

  // Point event handlers to our own methods
  GLUI_Master.set_glutIdleFunc(update);
  GLUI_Master.set_glutMouseFunc(onMouseClick);
  glutMotionFunc(onMouseMove);

  // Load shader programs and our texture.
  shaderSystem = ShaderSystem::getInstance();
  shaderSystem->initShader("passthrough.vs", "passthrough.fs", ShaderSystem::PASSTHROUGH);

  // Scene
  scene = new Scene(768, 512);
  scene->initShaderVars();

  // Enable specific capabilities
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_GREATER);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
   
  // "Background"
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  
  // UI
  initInterface();

  // Go!
  glutMainLoop();
  return 0;
}
