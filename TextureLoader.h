// From https://stackoverflow.com/questions/12518111/how-to-load-a-bmp-on-glut-to-use-it-as-a-texture

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#ifndef __TEXTURE_LOADER_H
#define __TEXTURE_LOADER_H

GLuint LoadTexture (const char* filename);

#endif