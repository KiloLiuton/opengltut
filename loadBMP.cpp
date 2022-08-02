#include <stdio.h>
#include <GL/glew.h>

GLuint loadBMP_custom(const char* imagepath)
{
  unsigned char header[54]; // BMP starts with 54-byte header
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;
  unsigned char * data;
  FILE* file = fopen(imagepath, "rb");
  if (!file) {
    printf("Image could not be loaded\n");
  }
  if ((fread(header, 1, 54, file) != 54) ||
      (header[0] != 'B') ||
      (header[1] != 'M')) {
    printf("Invalid BMP file");
    return 0;
  }
  dataPos   = *(int*)&(header[0x0A]); // get metadata from header
  imageSize = *(int*)&(header[0x22]);
  width     = *(int*)&(header[0x12]);
  height    = *(int*)&(header[0x16]);
  if (imageSize == 0) { imageSize = width*height; }
  if (dataPos == 0) { dataPos = 54; }

  data = new unsigned char [imageSize];

  fread(data, 1, imageSize, file);
  fclose(file);

  // create/bind/fill/configure one opengl texture
  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  return textureID;
}
