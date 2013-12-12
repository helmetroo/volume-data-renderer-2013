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
  
  // Prep backface render texture
  backface_texture->createOnGpu();
  backface_texture->setWrapping();
  backface_texture->setFilter();
  backface_texture->passToGpu();
  render_buffer->attachFrameBufferToTexture(backface_texture);

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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Camera transforms.
  camera->aim();

  // Output backfaces to the external frame buffer
  render_buffer->bindFrameBuffer();
  render_buffer->bindRenderBuffer();

  render_buffer->attachFrameBufferToTexture(backface_texture);

  glCullFace(GL_FRONT);
  bounding_box->draw();
}

void Scene::renderBoundingBoxFront(void)
{
  render_buffer->attachFrameBufferToTexture(frontface_texture);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glCullFace(GL_BACK);
  bounding_box->draw();
}

void Scene::raycast(void)
{
  // Here subtract frontface colors from backface colors
  // which gives ray direction, which we do in the raymarcher.
  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
  render_buffer->attachFrameBufferToTexture(output_texture);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render the back faces to a texture the raymarcher can use.
  // Also render the volume texture for sampling.
  
  // Render front faces normally.
  frontface_texture->beginRender("frontBoundingVol");
  backface_texture->beginRender("backBoundingVol");
  volume_texture->beginRender("volumeTexture");
  glCullFace(GL_BACK);
  bounding_box->draw();

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

  volume_texture->beginRender("outputVolume");
  full_quad->render();
}
