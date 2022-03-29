#ifndef LABGAME_TEXTURE_CLASS_H
#define LABGAME_TEXTURE_CLASS_H

#include <glad/glad.h>
#include <stb/stb_image.h>

class Texture {
public:
	GLuint id;
	Texture(const char* fname, GLuint slot = GL_TEXTURE0);
	void bind();
	void unbind();
	void destroy();
};

#endif