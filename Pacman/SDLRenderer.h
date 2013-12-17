#ifndef __SDL_RENDERER_H
#define __SDL_RENDERER_H

struct SDL_Surface; //Declare sdl_surface in order to avoid another include, since we are only gonna need it here

namespace Pacman
{
	class SDLRenderer; //main renderer class. Takes care of rendering the screen. Can blit surfaces from auxilary render class
	class SDLRenderTarget; // auxilary render class. Creates a surface and renders the surface.

	class SDLSurface //isloates the raw SDL_Surface and makes it only move assignable and move constructable
	{
		//making the render clases friends of our surface class. This is to avoid writing almost useless get functions and to facilitate better interaction between the classes
		friend class SDLRenderer;
		friend class SDLRenderTarget;

	public:
		SDLSurface(const std::string& path); // constructs a surface from a given path. uses a (0,0,0) colorkey on the loaded image and calls SDL_DisplayFormat to optimize the image.

		~SDLSurface(); // calls SDL_FreeSurface to free the raw surface pointer.

		//move constructor
		SDLSurface(SDLSurface&& surface)
		{
			m_Surface = surface.m_Surface;
			surface.m_Surface = nullptr;
		}

		//move assignment operator
		SDLSurface& operator=(SDLSurface&& surface)
		{
			if (&surface != this)
			{
				m_Surface = surface.m_Surface;
				surface.m_Surface = nullptr;
			}

			return *this;
		}

		size_t GetWidth() const;
		size_t GetHeight() const;

	private:
		SDLSurface(const SDLSurface& surface); // makes the copy constructor inaccesible;
		SDLSurface& operator=(const SDLSurface& surface); // makes the copy assignment operator inaccesble

		SDL_Surface* m_Surface;
	};

	class SDLRenderTarget
	{
		friend class SDLRenderer;

	public:

		SDLRenderTarget(size_t width, size_t height);//creates a new surface with SDL_CreateRGBSurface. Uses applied widht and height. Depth is always 32 bits;

		~SDLRenderTarget();

		void Clear(); 	//clears the surface, filling it with black.

		void Blit(const SDLSurface& surface, size_t x, size_t y); //Applies a surface to the the target offset

		//renders a target grid taking surfaces for tiling from surfaces. Uses grid width and tile_size to set dimensions
		void RenderTileset(const std::vector<SDLSurface>& surfaces, std::vector<char> grid, size_t grid_width, size_t tile_size);

	private:

		SDL_Surface* m_Surface;

	};

	// renderer class. Hold the Screen surface.
	class SDLRenderer
	{

	public:
		SDLRenderer(size_t width, size_t height);
		~SDLRenderer();

		void Clear(); //clears the screen, filling it with black.

		void Blit(const SDLSurface& surface, size_t x, size_t y); //blits target surface at x,y

		void Blit(const SDLRenderTarget& renderTarget, size_t x, size_t y); //blits the surface of the RenderTarget to x,y

		void Show(); //calls SDL_Flip, to update the screen

	private:

		SDL_Surface* m_Screen;
	};
}

#endif