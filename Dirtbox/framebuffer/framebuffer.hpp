#pragma once
#include <GL/glew.h>
#include "../texture/texture.hpp"


class Framebuffer
{
private:
	unsigned int _fboId;
	unsigned int _rboId;
	Texture _texture;
	
public:
	Framebuffer()
	: _fboId(0), _rboId(0)
	{
		
	}

	Framebuffer(unsigned int fboId, unsigned int rboId, Texture texture)
	: _fboId(fboId), _rboId(rboId), _texture(texture)
	{
		
	}

	static Framebuffer createMultisampleFramebuffer(unsigned int width, unsigned int height, unsigned int samples)
	{
		unsigned int fboId;
	    glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		
	    unsigned int textureId;
	    glGenTextures(1, &textureId);
	    glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureId, 0);
	 	
		unsigned int rboId;
	    glGenRenderbuffers(1, &rboId);
	    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	    glBindRenderbuffer(GL_RENDERBUFFER, 0);
   	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
	    
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	    Texture texture(textureId, width, height, false);

	    return Framebuffer(fboId, rboId, texture);
	}
	
	static Framebuffer createColorFramebuffer(unsigned int width, unsigned int height)
	{
		unsigned int fboId;
	    glGenFramebuffers(1, &fboId);
		glBindFramebuffer(GL_FRAMEBUFFER, fboId);
		
	    unsigned int textureId;
	    glGenTextures(1, &textureId);
	    glBindTexture(GL_TEXTURE_2D, textureId);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

		unsigned int rboId;
	    glGenRenderbuffers(1, &rboId);
	    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
	    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rboId);
	    
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
	    Texture texture(textureId, width, height, false);

	    return Framebuffer(fboId, rboId, texture);
	}
	
	static Framebuffer createDepthFramebuffer(unsigned int width, unsigned int height)
	{
		unsigned int textureId;
		unsigned int fboId;
	    glGenFramebuffers(1, &fboId);
	    glGenTextures(1, &textureId);
	    glBindTexture(GL_TEXTURE_2D, textureId);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
	    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textureId, 0);
	    glDrawBuffer(GL_NONE);
	    glReadBuffer(GL_NONE);
	    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	    Texture texture(textureId, width, height, false);

	    return Framebuffer(fboId, 0, texture);
	}

	void bind()
	{
		glViewport(0, 0, _texture.getWidth(), _texture.getHeight());
        glBindFramebuffer(GL_FRAMEBUFFER, _fboId);
	}

	static void unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	
	unsigned int getFboId() const
	{
		return _fboId;
	}

	Texture getTexture() const
	{
		return _texture;
	}

	unsigned int getWidth() const
	{
		return _texture.getWidth();
	}

	unsigned int getHeight() const
	{
		return _texture.getHeight();
	}

	operator bool() const
	{
		return _fboId;
	}
};
