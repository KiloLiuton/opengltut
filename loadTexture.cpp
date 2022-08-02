#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
        return 0;
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
  
    delete [] data;
  
    return textureID;
}


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII
GLuint loadDDS(const char* imagepath)
{
    unsigned char header[124];
    FILE *fp;

    fp = fopen(imagepath, "rb");
    if (fp==NULL) {
        printf("Image could not be loaded\n");
        return 0;
    }

    char filecode[4];
    fread(filecode, 1, 4, fp);
    if(strncmp(filecode, "DDS", 4) != 0) {
        fclose(fp);
        return 0;
    }

    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8]);
    unsigned int width       = *(unsigned int*)&(header[12]);
    unsigned int linearSize  = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);

    unsigned char* buffer;
    unsigned int bufsize;
    bufsize = mipMapCount > 1 ? linearSize*2: linearSize;
    buffer = (unsigned char*) malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    fclose(fp);

    //unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
    case FOURCC_DXT1:
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    case FOURCC_DXT3:
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    case FOURCC_DXT5:
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, buffer+offset);
        offset += size;
        width /= 2;
        height /= 2;
    }
    free(buffer);

    return textureID;
}
