#include "scene.hpp"

Scene::Scene(GLuint view_width, 
	     GLuint view_height) : width(view_width),	    
				   height(view_height)
{
}

void Scene::initObjects(void)
{
  // Initialize the camera
  camera = new TrackballCamera(0.5f, 0.5f, 3.0f, 0.5f, 0.5f, -1.0f);

  // Init objects
  bounding_box = new BoundingBox;

  render_buffer = new OutputBuffer(width, height);
  render_buffer->createFrameBuffer();

  frontface_texture = new BufferTexture(width, height);
  backface_texture = new BufferTexture(width, height);
  output_texture = new BufferTexture(width, height);

  // Prep frontface render texture
  frontface_texture->createOnGpu();
  frontface_texture->setWrapping();
  frontface_texture->setFilter();
  frontface_texture->passToGpu();
  
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
  
  // Create render buffer to create necessary render step for framebuffer
  render_buffer->createRenderBuffer();
  render_buffer->declareBufferStorage();
  render_buffer->attachRenderBufferToFrameBuffer();
  render_buffer->unbindFrameBuffer();
}

Scene::~Scene()
{
  delete camera;

  delete backface_texture;
  delete frontface_texture;
  delete output_texture;
  delete bounding_box;
  delete render_buffer;

  delete volume_texture;

  delete full_quad;
}

void Scene::changeVolumeFromFileName(const char* name)
{
  bool read_correctly = volume_texture->readFromFile(name);

  if(read_correctly) {
    volume_texture->createOnGpu();
    volume_texture->setWrapping();
    volume_texture->setFilter();
    volume_texture->passToGpu();
  }
}

void Scene::renderBoundingBoxBack(void)
{
  ShaderSystem::useShader(ShaderSystem::PASSTHROUGH);
  // Camera transforms.
  camera->aim();

  // Output backfaces to the external frame buffer
  render_buffer->bindFrameBuffer();
  render_buffer->bindRenderBuffer();

  render_buffer->attachFrameBufferToTexture(backface_texture);
  render_buffer->initViewport();

  // Output bounding box back faces to the buffer.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  bounding_box->draw();
  glDisable(GL_CULL_FACE);
}

void Scene::renderBoundingBoxFront(void)
{
  render_buffer->attachFrameBufferToTexture(frontface_texture);
  render_buffer->initViewport();

  // Output bounding box front faces to the buffer.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  bounding_box->draw();
  glDisable(GL_CULL_FACE);
}

void Scene::raycast(void)
{
  // Here subtract frontface colors from backface colors
  // which gives ray direction, which we do in the raymarcher.
  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
  frontface_texture->beginRender("frontBoundingVol");
  backface_texture->beginRender("backBoundingVol");
  volume_texture->beginRender("outputVoluem");

  // Render front faces normally.
  render_buffer->attachFrameBufferToTexture(output_texture);
  render_buffer->initViewport();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  bounding_box->draw();
  glDisable(GL_CULL_FACE);

  // Stop rendering to the buffer. Can now use it as output.
  render_buffer->unbindFrameBuffer();
  render_buffer->unbindRenderBuffer();
}

void Scene::outputFinalImage(void)
{
  // Use device coords to index output texture.
  ShaderSystem::useShader(ShaderSystem::OUTPUT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  output_texture->beginRender("output_volume");
  full_quad->render();
}
