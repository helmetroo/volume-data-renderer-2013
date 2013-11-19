/**
 * Samuel Sweet
 * CS 5610-1 Assignment 2
 *
 * The trackball-based camera implementations.
 */

#include "trackball.hpp"

TrackballCamera::TrackballCamera(float eye_x, float eye_y, float eye_z, 
				 float lookat_x, float lookat_y, float lookat_z)
{
  eye[0] = eye_x;
  eye[1] = eye_y;
  eye[2] = eye_z;
  lookat[0] = lookat_x;
  lookat[1] = lookat_y;
  lookat[2] = lookat_z;

  cur_button = -1;
}

void TrackballCamera::updateMouseState(int button, int state, int x, int y)
{
   if (state == GLUT_DOWN)
    cur_button = button;
  else
    {
      if (button == cur_button)
	cur_button = -1;
    }

  last_x = x;
  last_y = y;
}

void TrackballCamera::updateRotation(int x, int y)
{
  // the change in mouse position
  int dx = x-last_x;
  int dy = y-last_y;

  float scale, len, theta;
  float neye[3], neye2[3];
  float f[3], r[3], u[3];

  switch(cur_button)
    {
    case GLUT_LEFT_BUTTON:
      // translate
      f[0] = lookat[0] - eye[0];
      f[1] = lookat[1] - eye[1];
      f[2] = lookat[2] - eye[2];
      u[0] = 0;
      u[1] = 1;
      u[2] = 0;

      // scale the change by how far away we are
      scale = sqrt(length(f)) * 0.007;

      crossproduct(f, u, r);
      crossproduct(r, f, u);
      normalize(r);
      normalize(u);

      eye[0] += -r[0]*dx*scale + u[0]*dy*scale;
      eye[1] += -r[1]*dx*scale + u[1]*dy*scale;
      eye[2] += -r[2]*dx*scale + u[2]*dy*scale;

      lookat[0] += -r[0]*dx*scale + u[0]*dy*scale;
      lookat[1] += -r[1]*dx*scale + u[1]*dy*scale;
      lookat[2] += -r[2]*dx*scale + u[2]*dy*scale;

      break;

    case GLUT_MIDDLE_BUTTON:
      // zoom
      f[0] = lookat[0] - eye[0];
      f[1] = lookat[1] - eye[1];
      f[2] = lookat[2] - eye[2];

      len = length(f);
      normalize(f);

      // scale the change by how far away we are
      len -= sqrt(len)*dx*0.03;

      eye[0] = lookat[0] - len*f[0];
      eye[1] = lookat[1] - len*f[1];
      eye[2] = lookat[2] - len*f[2];

      // make sure the eye and lookat points are sufficiently far away
      // push the lookat point forward if it is too close
      if (len < 1)
	{
	  lookat[0] = eye[0] + f[0];
	  lookat[1] = eye[1] + f[1];
	  lookat[2] = eye[2] + f[2];
	}

      break;

    case GLUT_RIGHT_BUTTON:
      // rotate
      neye[0] = eye[0] - lookat[0];
      neye[1] = eye[1] - lookat[1];
      neye[2] = eye[2] - lookat[2];

      // first rotate in the x/z plane
      theta = -dx * 0.007;
      neye2[0] = (float)cos(theta)*neye[0] + (float)sin(theta)*neye[2];
      neye2[1] = neye[1];
      neye2[2] =-(float)sin(theta)*neye[0] + (float)cos(theta)*neye[2];


      // now rotate vertically
      theta = -dy * 0.007;

      f[0] = -neye2[0];
      f[1] = -neye2[1];
      f[2] = -neye2[2];
      u[0] = 0;
      u[1] = 1;
      u[2] = 0;
      crossproduct(f, u, r);
      crossproduct(r, f, u);
      len = length(f);
      normalize(f);
      normalize(u);

      neye[0] = len * ((float)cos(theta)*f[0] + (float)sin(theta)*u[0]);
      neye[1] = len * ((float)cos(theta)*f[1] + (float)sin(theta)*u[1]);
      neye[2] = len * ((float)cos(theta)*f[2] + (float)sin(theta)*u[2]);

      eye[0] = lookat[0] - neye[0];
      eye[1] = lookat[1] - neye[1];
      eye[2] = lookat[2] - neye[2];

      break;
    }

  last_x = x;
  last_y = y;

  // Update position for the uniform
  bindPosition();
}

// Vector math operations. Used largely
// for the trackball.
void TrackballCamera::crossproduct(float a[3], float b[3], float res[3])
{
  res[0] = (a[1] * b[2] - a[2] * b[1]);
  res[1] = (a[2] * b[0] - a[0] * b[2]);
  res[2] = (a[0] * b[1] - a[1] * b[0]);
}

void TrackballCamera::normalize(float v[3])
{
  float l = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
  l = 1 / (float)sqrt(l);

  v[0] *= l;
  v[1] *= l;
  v[2] *= l;
}

float TrackballCamera::length(float v[3])
{
  return (float)sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
}
