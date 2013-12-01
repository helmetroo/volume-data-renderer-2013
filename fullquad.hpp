class FullQuad
{
public:
  FullQuad();
 
  void render(void);
  inline void doVertex(float x, float y) {
    glVertex2f(x, y); glTexCoord2f(x, y);
  }
};
