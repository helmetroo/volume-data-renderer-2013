#include "scene.hpp"

Scene::Scene(GLuint view_width, 
	     GLuint view_height) : width(view_width),	    
				   height(view_height),
				   in_toon_mode(0)
{
  initObjects();
}

void Scene::initObjects(void)
{
  // Initialize the camera
  camera = new TrackballCamera(0, 0, 45, 0, 0, -1);

  // Init objects
  dog = new Dog;
  room = new Room;
  light = new Light((GLfloat)1.0f, (GLfloat)10.0f, (GLfloat)0.0f, (GLfloat)0.0f);
}

Scene::~Scene()
{
  delete camera;
  delete dog;
  delete room;
  delete light;
}

void Scene::initShaderVars(void)
{
  // Use shader that the system is currently set to
  onChangeDogShading(-1);
}

void Scene::render(void)
{   
  // Camera.
  MatrixStack::matrixMode(MatrixStack::VIEW);
  camera->aim();

  MatrixStack::matrixMode(MatrixStack::WORLD);
  
  // Bind light position
  light->bindLightColor();
  light->bindLightPos();

  // Room normally
  glDisable(GL_CULL_FACE);
  room->draw();

  if(in_toon_mode)
    { 
      ShaderSystem::useShader(ShaderSystem::TOON_SILHOUETTE);
      glEnable(GL_CULL_FACE);
      glColor3f(0.0f, 0.0f, 0.0f);
      glLineWidth(1.5f);
      glPolygonMode(GL_BACK, GL_LINE);
      glCullFace(GL_FRONT);
      glDepthFunc(GL_LESS);
      dog->draw();

      // Then draw dog normally with toon shading
      ShaderSystem::useShader(ShaderSystem::TOON);
      glLineWidth(1.0f);
      glPolygonMode(GL_BACK, GL_FILL);
      glCullFace(GL_BACK);
      glDepthFunc(GL_LEQUAL);
      dog->draw();

      glDisable(GL_BLEND);
      glDisable(GL_CULL_FACE);
    } 
  else
    {
      ShaderSystem::useShader(ShaderSystem::PHONG);
      dog->draw();
    }
}
