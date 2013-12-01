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
  bounding_box = new BoundingBox;
  light = new Light((GLfloat)1.0f, (GLfloat)10.0f, (GLfloat)0.0f, (GLfloat)0.0f);

  bounding_box_buffer = new Buffer(Buffer::FRAME, width, height);
  
  // Prep buffer texture
  bounding_box_buffer->prepBufferTexture();
  bounding_box_buffer->setWrapping();
  bounding_box_buffer->setFilter(Buffer::BufferFiltering::LINEAR);
  bounding_box_buffer->createBufferTexture();

  bounding_box_buffer->prepBuffer();
  bounding_box_buffer->createBuffer();
}

Scene::~Scene()
{
  delete camera;
  delete bounding_box_buffer;
  delete bounding_box;
  delete light;
}

void Scene::render(void)
{   
  // Camera.
  MatrixStack::matrixMode(MatrixStack::VIEW);
  camera->aim();

  MatrixStack::matrixMode(MatrixStack::WORLD);
  
  // Bind light position (may not need for now)
  light->bindLightColor();
  light->bindLightPos();

  // First draw front faces (to COL/TMP)
  ShaderSystem::useShader(ShaderSystem::PASSTHROUGH);
  bounding_box_buffer->bind();
  glCullFace(GL_FRONT);
  bounding_box->draw();
  bounding_box_buffer->unbind();

  // Then render back faces so we can calculate direction
  // capture renders to texture
  glCullFace(GL_BACK);
  bounding_box->draw();

  // Here subtract backface colors from frontface colors
  // which gives ray direction, which we do in the shader
  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
}
