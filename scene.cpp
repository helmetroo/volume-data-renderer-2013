#include "scene.hpp"

Scene::Scene(GLuint view_width, 
	     GLuint view_height) : width(view_width),	    
				   height(view_height)
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
  output_image_buffer = new Buffer(Buffer::FRAME, width, height);
  
  // Prep buffer texture
  bounding_box_buffer->prepBufferTexture();
  bounding_box_buffer->setWrapping();
  bounding_box_buffer->setFilter(Buffer::BufferFiltering::LINEAR);
  bounding_box_buffer->createBufferTexture();

  bounding_box_buffer->prepBuffer();
  bounding_box_buffer->createBuffer();

  // Prep output volume image texture
  output_image_buffer->prepBufferTexture();
  output_image_buffer->setWrapping();
  output_image_buffer->setFilter(Buffer::BufferFiltering::LINEAR);
  output_image_buffer->createBufferTexture();

  output_image_buffer->prepBuffer();
  output_image_buffer->createBuffer();
}

Scene::~Scene()
{
  delete camera;

  delete bounding_box_buffer;
  delete output_image_buffer;
  delete bounding_box;

  delete full_quad;

  delete light;
}

void Scene::renderBoundingBox(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
}

void Scene::raycast(void)
{
  // Here subtract backface colors from frontface colors
  // which gives ray direction, which we do in the shader
  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render back faces so we can calculate direction
  // capture renders to texture
  glCullFace(GL_BACK);
  bounding_box->draw();
}

void Scene::outputFinalImage(void)
{
  ShaderSystem::useShader(ShaderSystem::OUTPUT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  output_image_buffer->bindBufferTexture("outputVolume");
  full_quad->draw();
}
