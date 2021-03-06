/**
 * Samuel Sweet
 * CS 5610-1 Project
 *
 * The main program. Handles user interactions
 * as appropriate.
 */

#include "ui.hpp"

UI* ui;

inline void callOnClose(void)
{
  ui->onClose();
}

inline void callDisplay(void)
{
  ui->display();
}

inline void callUpdate(void)
{
  ui->update();
}

inline void callMouseMove(int x, int y)
{
  ui->onMouseMove(x, y);
}

inline void callMouseClick(int button, int state, int x, int y)
{
  ui->onMouseClick(button, state, x, y);
}

inline void callReshape(int w, int h)
{
  ui->onWindowPerspResize(w, h);
}

inline void callKeyboardPress(unsigned char key, int x, int y)
{
  ui->onKeyDown(key, x, y);
}

inline void callFileHandle(int control)
{
  ui->onChooseFile(control);
}

int main(int argc, char** argv)
{
  ui = new UI(1024, 512);

  // Init glut
  ui->initGlut(argc, argv);

  // Function binding
  ui->bindReshapeFunction(callReshape);
  ui->bindKeyPressFunction(callKeyboardPress);
  ui->bindDisplayFunction(callDisplay);
  ui->bindExitFunction(callOnClose);
  ui->bindIdleFunction(callUpdate);
  ui->bindMouseClickFunction(callMouseClick);
  ui->bindMouseMoveFunction(callMouseMove);
  ui->bindFileHandleFunction(callFileHandle);

  ui->initShaderSystem();
  ui->initGlCapabilities();
  ui->setBackground(0.0f, 0.0f, 0.0f, 0.0f);
  ui->initSceneObjects();
  ui->buildInterface();

  // Initial boot with a file passed in
  if(argc == 2)
    ui->getScene()->changeVolumeFromFileName(argv[1]);

  // Go!
  ui->run();
  return 0;
}
