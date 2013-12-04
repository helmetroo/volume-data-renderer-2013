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
  camera = new TrackballCamera(0.5f, 0.5f, 3.0f, 0.5f, 0.5f, -1.0f);

  // Init objects
  bounding_box = new BoundingBox;
  light = new Light((GLfloat)1.0f, (GLfloat)10.0f, (GLfloat)0.0f, (GLfloat)0.0f);

  render_buffer = new OutputBuffer(width, height);
  render_buffer->createFrameBuffer();

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
  delete output_texture;
  delete bounding_box;
  delete render_buffer;

  delete volume_texture;

  delete full_quad;

  // May oust
  delete light;
}

void Scene::renderBoundingBox(void)
{
  ShaderSystem::useShader(ShaderSystem::PASSTHROUGH);

  // Camera transforms.
  MatrixStack::matrixMode(MatrixStack::VIEW);
  camera->aim();

  MatrixStack::matrixMode(MatrixStack::WORLD);

  // Output backfaces to the external frame buffer
  //render_buffer->bindFrameBuffer();
  //render_buffer->bindRenderBuffer();

  //render_buffer->attachFrameBufferToTexture(backface_texture);

  glCullFace(GL_FRONT);
  bounding_box->draw();
}

void Scene::raycast(void)
{
  // Here subtract backface colors from frontface vertices (texcoords)
  // which gives ray direction, which we do in the raymarcher.
  ShaderSystem::useShader(ShaderSystem::RAYCASTING);
  render_buffer->attachFrameBufferToTexture(output_texture);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Render the back faces to a texture the raymarcher can use.
  // Also render the volume texture for sampling.
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

  backface_texture->beginRender("outputVolume");
  full_quad->render();
}
