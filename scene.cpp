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
  camera = new TrackballCamera(0.5, 0.5, -1.5, 0.5, 0.5, 1);

  // Init objects
  bounding_box = new BoundingBox;
  light = new Light((GLfloat)1.0f, (GLfloat)10.0f, (GLfloat)0.0f, (GLfloat)0.0f);

  backface_texture = new BufferTexture(width, height);
  output_texture = new BufferTexture(width, height);
  
  // Prep backface render texture
  backface_texture->createOnGpu();
  backface_texture->setWrapping();
  backface_texture->setFilter();
  backface_texture->passToGpu();

  // Prep output volume image texture
  output_texture->createOnGpu();
  output_texture->setWrapping();
  output_texture->setFilter();
  output_texture->passToGpu();

  // The volume texture
  volume_texture = new VolumeTexture;
  volume_texture->createTestTexture();
}

Scene::~Scene()
{
  delete camera;

  delete backface_texture;
  delete output_texture;
  delete bounding_box;

  delete volume_texture;

  delete full_quad;

  delete light;
}

void Scene::renderBoundingBox(void)
{
  ShaderSystem::useShader(ShaderSystem::PASSTHROUGH);

  // Camera.
  MatrixStack::matrixMode(MatrixStack::VIEW);
  camera->aim();

  MatrixStack::matrixMode(MatrixStack::WORLD);

  // First draw front faces (to COL/TMP)
  /*
  render_buffer->bind();
  render_buffer->attachBufferToTexture(backface_texture);

  glCullFace(GL_FRONT);
  bounding_box->draw();
  */

  glCullFace(GL_FRONT);
  bounding_box->draw();
}

void Scene::raycast(void)
{
  // Here subtract backface colors from frontface colors
  // which gives ray direction, which we do in the shader
  // Render back faces so we can calculate direction
  // capture renders to texture
  render_buffer->attachBufferToTexture(output_texture);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
  backface_texture->beginRender("backBoundingVol");
  volume_texture->beginRender("volumeTexture");
  glCullFace(GL_BACK);
  bounding_box->draw();

  render_buffer->unbind();
}

void Scene::outputFinalImage(void)
{
  ShaderSystem::useShader(ShaderSystem::OUTPUT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  backface_texture->beginRender("outputVolume");
  // resize
  full_quad->render();
}
