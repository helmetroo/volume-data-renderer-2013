#include "ui.hpp"

UI::UI(const int& _width, const int& _height) : width(_width), 
						height(_height)
{
  scene = new Scene(width, height);
  matrix_stack = new MatrixStack;
}

void UI::initGlCapabilities(void)
{
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_NORMALIZE);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);
}

void UI::initGlut(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  glutInitWindowSize(width, height);
  main_window = glutCreateWindow("VolRay");
}

void UI::initShaderSystem(void)
{
  // Load shader programs and our texture.
  shader_system_instance = ShaderSystem::getInstance();
  shader_system_instance->initShader("passthrough.vs", "passthrough.fs", ShaderSystem::PASSTHROUGH);
  shader_system_instance->initShader("raycasting.vs", "raycasting.fs", ShaderSystem::RAYCASTING);
  shader_system_instance->initShader("fullscrtexture.vs", "fullscrtexture.fs", ShaderSystem::OUTPUT);
}

void UI::initSceneObjects(void)
{
  scene->initObjects();
}

void UI::buildInterface(void)
{
  // Create interface
  glui = GLUI_Master.create_glui_subwindow(main_window, GLUI_SUBWINDOW_RIGHT);

  // Quit
  glui->add_button("Quit", 0, (GLUI_Update_CB)exit);

  // Bind interface to window
  glui->set_main_gfx_window(main_window);
}

// Correctly scales all objects when the window is resized.
void UI::onWindowResize(int w, int h)
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

void UI::onWindowPerspResize(int w, int h)
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

void UI::display(void)
{
  // Init display for rendering
  onWindowPerspResize(scene->getWidth(), scene->getHeight());
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
  scene->renderBoundingBox();
  scene->raycast();

  // Draw full screen quad to output final image.
  onWindowResize(scene->getWidth(), scene->getHeight());
  scene->outputFinalImage();
  
  // Bind transformation / projection matrices.
  MatrixStack::bindWorldMatrix();
  MatrixStack::bindViewMatrix();
  MatrixStack::bindProjectionMatrix();
  MatrixStack::bindTextureMatrix();

  // Swap buffers
  glutSwapBuffers();
}

// Drives all animations.
void UI::update(void)
{
  // Bind to window context.
  if(glutGetWindow() != main_window)
    glutSetWindow(main_window);

  glutPostRedisplay();
}

// Catches mouse up/down events.
void UI::onMouseClick(int button, int state, int x, int y)
{
  scene->getCamera()->updateMouseState(button, state, x, y);
  glutPostRedisplay();
}

// Catches mouse move events.
// Implements the trackball.
void UI::onMouseMove(int x, int y)
{
  scene->getCamera()->updateRotation(x, y);
  glutPostRedisplay();
}

// Handles interactions when the keyboard is pressed
void UI::onKeyDown(unsigned char key, int x, int y)
{
  switch(key)
    {
    case 27:
      exit(0);
    }

  glui->sync_live(); 
  glutPostRedisplay();
}

void UI::onClose(void)
{
  delete shader_system_instance;
  delete matrix_stack;
  delete scene;
}
