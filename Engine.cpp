#include "Engine.h"

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_ttf.h"

/*
	https://wiki.libsdl.org/SDL2/SDL_Palette
	https://wiki.libsdl.org/SDL2/SDL_Color
*/


// ------------------ MOTOR PRINCIPAL ------------------ //

SDL_Renderer* Engine::renderer = nullptr;

void Engine::Start() {
	_closed = !init();
	this->appCreate();
	while (!_closed) {
		this->pollEvents();
		SDL_SetRenderDrawColor(renderer, 10, 10, 100, 255); // Setea el color default de pantalla limpia para el render
		SDL_RenderClear(renderer); // Limpia el render
		this->appUpdate();
		SDL_RenderPresent(renderer); // Lo actualiza en la pantalla
	}
}

Engine::~Engine() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(_window);
	TTF_Quit();
	SDL_Quit();
}

bool Engine::init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cerr << "Error in SDL_Init: " << SDL_GetError() << std::endl;
		return false;
	}
	if (TTF_Init() != 0) {
		std::cerr << "Error in TTF_Init: " << SDL_GetError() << std::endl;
		return false;
	}

	_window = SDL_CreateWindow(appName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, appWidth, appHeight, 0);

	if (_window == nullptr) {
		std::cerr << "Error creating window: " << SDL_GetError() << std::endl;
		return false;
	}

	renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr) {
		std::cerr << "Error creating renderer: " << SDL_GetError() << std::endl;
	}

	return true;
}

Engine::Engine() {
}

void Engine::Setting(const std::string& title, int width, int height)
{
	appName = title;
	appWidth = width;
	appHeight = height;
	_closed = false;
	_key_pressed = -1;
}

void Engine::pollEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT: {
			_closed = true;
			break;
		}
		case SDL_KEYDOWN: {
			_key_pressed = event.key.keysym.sym;
			break;
		}
		default: {
			_key_pressed = -1;
			break;
		}
		}
	}
}

void Engine::clear() const {
	SDL_RenderPresent(renderer); // Lo actualiza en la pantalla
	SDL_SetRenderDrawColor(renderer, 10, 10, 100, 255); // Setea el color default de pantalla limpia para el render
	SDL_RenderClear(renderer); // Limpia el render
}

void Engine::DrawSprite(uint32_t x, uint32_t y, Sprite* sprite, int scale) {
	if (sprite == nullptr) {
		return;
	} if (scale == 1) {
		for (int32_t i = 0; i < sprite->width; i++) {
			for (int32_t j = 0; j < sprite->height; j++) {
				Draw(x + i, y + j, sprite->getPixel(i, j));
			}
		}
	}
	else {
		int32_t w = 0;
		for (int32_t i = 0; i < (sprite->width); i++, w += scale) {
			int32_t h = 0;
			for (int32_t j = 0; j < (sprite->height); j++, h += scale) {
				SDL_Color color = sprite->getPixel(i, j);
				for (int32_t ix = w; ix < (w + scale); ix++) {
					for (int32_t jy = h; jy < (h + scale); jy++) {
						Draw(ix + x, jy + y, color);
					}
				}
			}

		}
	}

}

void Engine::DrawSprite(uint32_t x, uint32_t y, Sprite* sprite) {
	DrawSprite(x, y, sprite, 1);
}


void Engine::Draw(uint32_t x, uint32_t y, SDL_Color color) {
	SDL_SetRenderDrawColor(this->renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawPoint(this->renderer, x, y);
}

void Engine::DrawString(uint32_t x, uint32_t y, const std::string& text, SDL_Color color) {
	auto text_surface = TTF_RenderText_Solid(this->font, text.c_str(), color);
	if (!text_surface) {
		std::cerr << "failed to create text surface\n";
	}

	auto text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	if (!text_texture) {
		std::cerr << "failed to create text surface\n";
	}

	SDL_FreeSurface(text_surface);
	SDL_Rect rect;
	SDL_QueryTexture(text_texture, nullptr, nullptr, &rect.w, &rect.h);
	rect.x = x;
	rect.y = y;
	SDL_RenderCopy(renderer, text_texture, nullptr, &rect);
	SDL_DestroyTexture(text_texture);

}

bool Engine::loadFont(const std::string& path, int size) {
	this->font = TTF_OpenFont(path.c_str(), size);
	if (!font) {
		std::cerr << "Error al carga la fuente\n";
		return false;
	}
	return true;
}



// ------------------ SPRITE CLASS ------------------ //



Sprite::Sprite() {
	this->height = 500;
	this->width = 500;
	//data.resize(width * height);
	data.resize(width * height, COLOR_NULO);
}
Sprite::Sprite(uint32_t width, uint32_t height) {
	this->height = height;
	this->width = width;
	data.resize(width * height);
	data.resize(width * height, COLOR_NULO);
	//cout << "r: " << (int)data[0].r << endl;
}
Sprite::~Sprite() {
	//data.resize(0);
}


bool Sprite::SetPixel(uint32_t x, uint32_t y, SDL_Color color) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		data[y * width + x] = color;
		return true;
	}
	return false;
}

SDL_Color Sprite::getPixel(uint32_t x, uint32_t y) {
	if (x >= 0 && x < width && y >= 0 && y < height) {
		return data[(y * width) + x];
	}
	return COLOR_NULO;
}


