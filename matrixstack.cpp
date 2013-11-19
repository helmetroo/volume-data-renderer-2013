#include "matrixstack.hpp"

// Static definitions
const char* MatrixStack::WORLD_MATRIX_SHADER_NAME = "worldMatrix";
const char* MatrixStack::VIEW_MATRIX_SHADER_NAME = "viewMatrix";
const char* MatrixStack::PROJECTION_MATRIX_SHADER_NAME = "projectionMatrix";
const char* MatrixStack::TEXTURE_MATRIX_SHADER_NAME = "textureMatrix";
const char* MatrixStack::INVERSE_VIEW_MATRIX_SHADER_NAME = "inverseViewMatrix";

Matrix4Stack MatrixStack::worldMatrixStack;
Matrix4Stack MatrixStack::viewMatrixStack;
Matrix4Stack MatrixStack::projectionMatrixStack;
Matrix4Stack MatrixStack::textureMatrixStack;

Matrix4 MatrixStack::currentWorldMatrix;
Matrix4 MatrixStack::currentViewMatrix;
Matrix4 MatrixStack::currentProjectionMatrix;
Matrix4 MatrixStack::currentTextureMatrix;
Matrix4 MatrixStack::viewInverse;

Matrix4Stack* MatrixStack::current_stack;
Matrix4* MatrixStack::current_matrix;

MatrixStack::MatrixStack()
{
  currentWorldMatrix = Matrix4(1.0f);
  currentViewMatrix = Matrix4(1.0f);
  currentProjectionMatrix = Matrix4(1.0f);
  currentTextureMatrix = Matrix4(1.0f);
  viewInverse = Matrix4(1.0f);

  current_stack = &worldMatrixStack;
  current_matrix = &currentWorldMatrix;
}

MatrixStack::~MatrixStack()
{
}
