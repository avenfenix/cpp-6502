#pragma once

#include <string>
#include <inttypes.h>
#include <vector>

#include "SDL.h"
#include "SDL_ttf.h"

using namespace std;

class Sprite {
public:
	Sprite();
	Sprite(uint32_t width, uint32_t height);
	~Sprite();
	uint32_t width;
	uint32_t height;
public:
	std::vector<SDL_Color> data;
public:
	SDL_Color getPixel(uint32_t x, uint32_t y);
	bool SetPixel(uint32_t x, uint32_t y, SDL_Color color);
private:
	SDL_Color COLOR_NULO = { 0,0,0,0 };
};

class Engine {
public:
	Engine();
	~Engine();

public:
	void Setting(const std::string& title, int width, int height);
	void Start();
	virtual void appUpdate() = 0;
	virtual void appCreate() = 0;

public:
	void clear() const;
	void pollEvents();
	inline bool isClosed() const { return _closed; }
	inline int getKeyPressed() const { return _key_pressed; }

public:

	void DrawSprite(uint32_t x, uint32_t y, Sprite* sprite, int scale);
	void DrawSprite(uint32_t x, uint32_t y, Sprite* sprite);
	void Draw(uint32_t x, uint32_t y, SDL_Color color);

public: // Texto
	TTF_Font* font;
	bool loadFont(const std::string& font_path, int size);
	void DrawString(uint32_t x, uint32_t y, const std::string& text, SDL_Color color);
public: // Atributos de la ventana
	std::string appName;
	uint32_t appWidth;
	uint32_t appHeight;

private:
	bool init();

private:
	int _key_pressed;
public:
	struct keys_s {
		enum {
			SPACE = SDLK_SPACE, Q = SDLK_q, W = SDLK_w, E = SDLK_e, R = SDLK_r, T = SDLK_t, Y = SDLK_y, U = SDLK_u, I = SDLK_i, O = SDLK_o, P = SDLK_p,
			A = SDLK_a, S = SDLK_s, D = SDLK_d, F = SDLK_f, G = SDLK_g, H = SDLK_h, J = SDLK_j, K = SDLK_k, L = SDLK_l, Z = SDLK_z, X = SDLK_x, C = SDLK_c,
			V = SDLK_v, B = SDLK_b, N = SDLK_n, M = SDLK_m
		};
	}KEYS;
private:
	SDL_Window* _window = nullptr;

	bool _closed;
public:
	static SDL_Renderer* renderer;
};




