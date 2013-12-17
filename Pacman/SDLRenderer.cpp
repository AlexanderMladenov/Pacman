#include <vector>
#include "SDL_image.h"
#include "SDL.h"
#include "SDLRenderer.h"
#include "Tilemap.h"
namespace Pacman
{

	SDLSurface::SDLSurface(const std::string& path)
	{
		//uses IMG_Load to load image
		auto image = IMG_Load(path.c_str());;
		if (image == NULL)
		{
			throw std::runtime_error("Failed to IMG_Load");
		}
		//sets colorkey to be (0,0,0) for proper transperancy
		SDL_SetColorKey(image, SDL_SRCCOLORKEY, SDL_MapRGB(image->format, 0, 0, 0));

		//optimizes image
		auto optimizedImage = SDL_DisplayFormatAlpha(image);

		if (optimizedImage == NULL)
		{
			throw std::runtime_error("Failed to SDL_DisplayFormat");
		}

		//Free the old image
		SDL_FreeSurface(image);

		m_Surface = optimizedImage;
	}

	SDLSurface::~SDLSurface()
	{
		if (m_Surface)
		{
			SDL_FreeSurface(m_Surface); // frees the current surface
		}
	}

	size_t SDLSurface::GetWidth() const
	{
		return m_Surface->w;
	}

	size_t SDLSurface::GetHeight() const
	{
		return m_Surface->h;
	}

	SDLRenderTarget::SDLRenderTarget(size_t width, size_t height)
	{
		m_Surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);

		if (m_Surface == NULL)
		{
			throw std::runtime_error("Failed to create surface");
		}
	}

	SDLRenderTarget::~SDLRenderTarget()
	{
		SDL_FreeSurface(m_Surface); // free the surface;
	}

	void SDLRenderTarget::Blit(const SDLSurface& surface, size_t x, size_t y)
	{
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		SDL_BlitSurface(surface.m_Surface, 0, m_Surface, &offset);
	}
	void SDLRenderTarget::Clear()
	{
		SDL_FillRect(m_Surface, NULL, SDL_MapRGB(m_Surface->format, 0, 0, 0));
	}

	void SDLRenderTarget::RenderTileset(const std::vector<SDLSurface>& surfaces, std::vector<char> grid, size_t grid_width, size_t tile_size)
	{
		//calculates grid height
		size_t grid_height = grid.size() / grid_width;

		for (auto x = 0u; x < grid_width; x++)// goes trough grid
		{
			for (auto y = 0u; y < grid_height; y++)
			{
				//calculates the index we are on.
				auto index = x + y * grid_width;

				if ((unsigned char)grid[index] >= surfaces.size()) // if the tile type we are on is larger than the grid surfaces size
				{
					continue; // skip it
				}

				const auto& surface = surfaces[grid[index]]; //determine wich surface to use

				//calculate the halfway
				auto dw = tile_size - surface.GetWidth();
				auto dh = tile_size - surface.GetHeight();

				//Blit in the middle
				Blit(surface, x * tile_size + dw / 2, y * tile_size + dh / 2);
			}
		}
	}

	SDLRenderer::SDLRenderer(size_t width, size_t height)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
		{
			throw std::runtime_error("SDL failed to initialize ");
		}

		//Set up the screen
		m_Screen = SDL_SetVideoMode(width, height, 32, SDL_HWACCEL);

		//If there was an error in setting up the screen
		if (m_Screen == NULL)
		{
			throw std::runtime_error("SDL failed to set screen");
		}

		//Set the window caption
		SDL_WM_SetCaption("Pacman", NULL);
	}

	SDLRenderer::~SDLRenderer()
	{
		SDL_FreeSurface(m_Screen); // free the surface
	}

	//fills entire screen black
	void SDLRenderer::Clear()
	{
		SDL_FillRect(m_Screen, NULL, SDL_MapRGB(m_Screen->format, 0, 0, 0));
	}

	void SDLRenderer::Blit(const SDLSurface& surface, size_t x, size_t y)
	{
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		SDL_BlitSurface(surface.m_Surface, 0, m_Screen, &offset);
	}

	void SDLRenderer::Blit(const SDLRenderTarget& renderTarget, size_t x, size_t y)
	{
		SDL_Rect offset;
		offset.x = x;
		offset.y = y;
		SDL_BlitSurface(renderTarget.m_Surface, 0, m_Screen, &offset);
	}

	void SDLRenderer::Show()
	{
		if (SDL_Flip(m_Screen) == -1)
		{
			throw std::runtime_error("Failed to SDL_Flip");
		}
	}
}
