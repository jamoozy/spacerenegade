//////////////////////////////////////////////////////////////////////
//
// OpenGL Texture Class
// by: Matthew Fairfax
//
// GLTexture.cpp: implementation of the GLTexture class.
// This class loads a texture file and prepares it
// to be used in OpenGL. It can open a bitmap or a
// targa file. The min filter is set to mipmap b/c
// they look better and the performance cost on
// modern video cards in negligible. I leave all of
// the texture management to the application. I have
// included the ability to load the texture from a
// Visual Studio resource. The bitmap's id must be
// be surrounded by quotation marks (i.e. "Texture.bmp").
// The targa files must be in a resource type of "TGA"
// (including the quotes). The targa's id must be
// surrounded by quotation marks (i.e. "Texture.tga").
//
// Usage:
// GLTexture tex;
// GLTexture tex1;
// GLTexture tex3;
//
// tex.Load("texture.bmp"); // Loads a bitmap
// tex.Use();				// Binds the bitmap for use
// 
// tex1.LoadFromResource("texture.tga"); // Loads a targa
// tex1.Use();				 // Binds the targa for use
//
// // You can also build a texture with a single color and use it
// tex3.BuildColorTexture(255, 0, 0);	// Builds a solid red texture
// tex3.Use();				 // Binds the targa for use
//
//////////////////////////////////////////////////////////////////////

#include "GLTexture.h"
#include "GL/gl.h"
#include "GL/glu.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "glbmp.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GLTexture::GLTexture()
{

}

GLTexture::~GLTexture()
{

}

char *GLTexture::strlwr(char *str) 
{ 
	// Move over the string. 
	for (int i = 0; str[i] != '\0'; i++) 
		if ('A' <= str[i] && str[i] <= 'Z') 
			str[i] = str[i] - 'A' + 'a'; 

	return str; 
} 

bool GLTexture::Load(char *name)
{
	// make the texture name all lower case
	texturename = strlwr(strdup(name));

	// strip "'s
	if (strstr(texturename, "\""))
		texturename = strtok(texturename, "\"");

	// check the file extension to see what type of texture
	if(strstr(texturename, ".bmp"))	
		return LoadBMP(texturename);
	if(strstr(texturename, ".tga"))	
		return LoadTGA(texturename);

	// Unrecognized format.
	fprintf(stderr, "Unrecognized file extension! %s\n", name);
	fprintf(stderr, "Make sure it is a \'.bmp\' or \'.tga\' file.");
	return false;
}

void GLTexture::Use()
{
	glEnable(GL_TEXTURE_2D);								// Enable texture mapping
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// Bind the texture as the current one
}

bool GLTexture::LoadBMP(char *name)
{
	// Create a place to store the texture
	glbmp_t *textureImage = (glbmp_t*)malloc(sizeof(glbmp_t));

	// Load the bitmap and assign our pointer to it
	if(!glbmp_LoadBitmap(name, 0, textureImage))
	{
		free(textureImage);
		return false;
	}

	// Just in case we want to use the width and height later
	width = textureImage->width;
	height = textureImage->height;

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, textureImage->width, textureImage->height, GL_RGB, GL_UNSIGNED_BYTE, textureImage->rgb_data);

	// Cleanup
	if (textureImage)
	{
		if (textureImage->rgb_data)
			free(textureImage->rgb_data);

		free(textureImage);
	}

	return true;
}

bool GLTexture::LoadTGA(char *name)
{
	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLubyte		TGAcompare[12];								// Used to compare TGA header
	GLubyte		header[6];									// First 6 useful bytes of the header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	FILE *file = fopen(name, "rb");							// Open the TGA file

	// Load the file and perform checks
	if(file == NULL ||														// Does file exist?
	   fread(TGAcompare,1,sizeof(TGAcompare),file) != sizeof(TGAcompare) ||	// Are there 12 bytes to read?
	   memcmp(TGAheader,TGAcompare,sizeof(TGAheader)) != 0				 ||	// Is it the right format?
	   fread(header,1,sizeof(header),file) != sizeof(header))				// If so then read the next 6 header bytes
	{
		if (file == NULL)									// If the file didn't exist then return
		{
			fprintf(stderr, "File does not exist! %s\n", name);
			return false;
		}
		else
		{
			fclose(file);									// If something broke then close the file and return
			fprintf(stderr, "Bad file format! %s\n", name);
			return false;
		}
	}

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = header[1] * 256 + header[0];
	height = header[3] * 256 + header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (header[4] != 24 && header[4] != 32))				// Is it 24 or 32 bit?
	{
		fclose(file);										// If anything didn't check out then close the file and return
		fprintf(stderr, "Bad image resolution or bit-count! %s\n", name);
		return false;
	}

	bpp				= header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Make sure the data is allocated write and load it
	if(imageData == NULL ||									// Does the memory storage exist?
	   fread(imageData, 1, imageSize, file) != imageSize)	// Does the image size match the memory reserved?
	{
		if(imageData != NULL)								// Was the image data loaded
			free(imageData);								// If so, then release the image data

		fclose(file);										// Close the file
		fprintf(stderr, "%d Internal memory storage error! %s\n", __LINE__, name);
		return false;
	}

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < GLuint(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// We are done with the file so close it
	fclose(file);

	// Set the type
	if (bpp == 24)
		type = GL_RGB;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);

	return true;
}

#ifdef WIN32

bool GLTexture::LoadBMPResource(char *name)
{
	// Find the bitmap in the bitmap resources
	HRSRC hrsrc = FindResource(0, name, RT_BITMAP);

	// If you can't find it then return
	if (hrsrc==0)
	{
		fprintf(stderr, "Could not find resource! %s\n", name);
		return false;
	}

	// Load the bitmap
	HGLOBAL resource = LoadResource(0, hrsrc);
	
	// If you can't load it then return
	if (resource==0)
	{
		fprintf(stderr, "Could not load resource! %s\n", name);
		return false;
	}

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into a bitmap
	BITMAP *bmp = (BITMAP*)buffer;

	// Get the height and width for future use
	width = bmp->bmWidth;
	height = bmp->bmHeight;

	// Reverse the blue colot bit and the red color bit
	unsigned char *ptr = (unsigned char *)buffer+sizeof(BITMAPINFO)+2;
	unsigned char temp;

	for (int i = 0; i < width*height; i++)
	{
		temp = ptr[i*3];
		ptr[i*3] = ptr[i*3+2];
		ptr[i*3+2] = temp;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char *)buffer+sizeof(BITMAPINFO)+2);
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, bmp->bmBits);

	// Cleanup
	free(buffer);
	free(bmp);

	return true;
}

bool GLTexture::LoadTGAResource(char *name)
{
	// struct to cast the resource into
	struct TGAstruct {
		GLubyte	TGAcompare[12];								// Used to compare TGA header
		GLubyte	header[6];									// First 6 useful bytes of the header
	};

	GLubyte		TGAheader[12]	= {0,0,2,0,0,0,0,0,0,0,0,0};// Uncompressed TGA header
	GLuint		bytesPerPixel;								// Holds the number of bytes per pixel used
	GLuint		imageSize;									// Used to store the image size
	GLuint		temp;										// Temporary variable
	GLuint		type			= GL_RGBA;					// Set the default type to RBGA (32 BPP)
	GLubyte		*imageData;									// Image data (up to 32 Bits)
	GLuint		bpp;										// Image color depth in bits per pixel.

	// Find the targa in the "TGA" resources
	HRSRC hrsrc = FindResource(0, name, "TGA");

	// If you can't find it then return
	if (hrsrc==0)
	{
		fprintf(stderr, "Could not find resource! %s\n", name);
		return false;
	}

	// Load the targa
	HGLOBAL resource = LoadResource(0, hrsrc);

	// If you can't load it then return
	if (resource==0)
	{
		fprintf(stderr, "Could not load resource! %s\n", name);
		return false;
	}

	// Load it into the buffer
	void *buffer = LockResource(resource);

	// Cast it into the targa struct
	TGAstruct *top = (TGAstruct*)buffer;

	// Make sure it checks out against our comparison header
	if (memcmp(TGAheader,top,sizeof(TGAheader)) != 0)
	{
		fprintf(stderr, "Header is not of the correct format! %s\n", name);
		return false;
	}

	// Determine the TGA width and height (highbyte*256+lowbyte)
	width  = top->header[1] * 256 + top->header[0];
	height = top->header[3] * 256 + top->header[2];
    
	// Check to make sure the targa is valid and is 24 bit or 32 bit
	if(width	<=0	||										// Is the width less than or equal to zero
	   height	<=0	||										// Is the height less than or equal to zero
	   (top->header[4] != 24 && top->header[4] != 32))		// Is it 24 or 32 bit?
	{
		// If anything didn't check out then close the file and return
		fprintf(stderr, "Resource not a valid 24 or 32 bit targa! %s\n", name);
		return false;
	}

	bpp				= top->header[4];							// Grab the bits per pixel
	bytesPerPixel	= bpp / 8;								// Divide by 8 to get the bytes per pixel
	imageSize		= width * height * bytesPerPixel;		// Calculate the memory required for the data

	// Allocate the memory for the image data
	imageData		= new GLubyte[imageSize];

	// Load the data in
	memcpy(imageData, (GLubyte*)buffer+18, imageSize);

	// Loop through the image data and swap the 1st and 3rd bytes (red and blue)
	for(GLuint i = 0; i < int(imageSize); i += bytesPerPixel)
	{
		temp = imageData[i];
		imageData[i] = imageData[i + 2];
		imageData[i + 2] = temp;
	}

	// Set the type
	if (bpp == 24)
		type = GL_RGB;
	
	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, type, width, height, type, GL_UNSIGNED_BYTE, imageData);

	// Cleanup
	free(imageData);
	free(buffer);
	free(top);

	return true;
}

bool GLTexture::LoadFromResource(char *name)
{
	// make the texture name all lower case
	texturename = strlwr(strdup(name));

	// check the file extension to see what type of texture
	if(strstr(texturename, ".bmp"))
		return LoadBMPResource(name);
	if(strstr(texturename, ".tga"))	
		return LoadTGAResource(name);

	fprintf(stderr, "Unrecognized resource extension! %s\n", name);
	fprintf(stderr, "Make sure it is a bitmap or targa resource.");
	return false;
}

#endif // WIN32

void GLTexture::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &texture[0]);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}

