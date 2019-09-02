#ifndef _texture_
#define _texture_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;	

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


class Texture
{
	public:
		Texture();
		~Texture();
		bool loadFromFile( std::string path );			//Loads image at specified 
		void free();						//Loads image at specified path
		void setColor( Uint8 red, Uint8 green, Uint8 blue );	//Set color modulation
		void setBlendMode( SDL_BlendMode blending );		//Set blending
		void setAlpha( Uint8 alpha );				//Set alpha modulation
		void render( int x, int y, SDL_Rect* clip = NULL );	//Renders texture at given point

		int getWidth();
		int getHeight();

	private:
		SDL_Texture* mTexture;		//The actual hardware texture

		//Image dimensions
		int mWidth;
		int mHeight;
};

bool init();				//Starts up SDL and creates window
bool loadMedia();			//Loads media
void close();				//Frees media and shuts down SDL

#endif
