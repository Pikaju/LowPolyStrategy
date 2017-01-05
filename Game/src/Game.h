#pragma once

#include "world/World.h"
#include "graphics/font/FontRenderer.h"
#include "graphics/gui/Button.h"

namespace graphics {
	class Window;
}

namespace io {
	class Input;
}

class Game
{
public:
	Game();
	~Game();

	void initialize();
	void cleanup();

	void update(const io::Input& input, float delta);
	void render(const graphics::Window& window);
private:
	world::World m_world;
	graphics::FontRenderer m_fontRenderer;
	graphics::Button m_button;
};