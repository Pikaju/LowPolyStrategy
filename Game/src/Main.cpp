#include "graphics/GraphicsAPI.h"
#include "graphics/Window.h"
#include "graphics/renderer/Renderer.h"
#include "graphics/model/ModelManager.h"
#include "io/Log.h"
#include "util/Timer.h"
#include "Game.h"

int main(int agrc, char** agrv)
{
	graphics::api::initialize();

	graphics::Window window;
	window.initialize("Game  |  Pre-Alpha", 1280, 720, true, false, true);

	graphics::Renderer renderer;
	renderer.setDepthRead(true);
	renderer.setCulling(true);
	
	Game game;
	game.initialize();

	util::Timer deltaTimer;
	deltaTimer.reset();
	float delta = 0.0;
	util::Timer fpsTimer;
	unsigned int frames = 0;

	while (!window.isCloseRequested()) {
		delta = (float)deltaTimer.getPassedSeconds();
		deltaTimer.reset();

		game.update(window.getInput(), delta);

		window.bind();
		renderer.clear(true, true);

		game.render(window);

		window.update();
		window.swap();

		frames++;
		while (fpsTimer.getPassedSeconds() >= 1.0) {
			fpsTimer.reset();
			io::log(std::to_string(frames) + " fps");
			frames = 0;
		}
	}

	game.cleanup();
	graphics::ModelManager::cleanup();
	window.cleanup();
	graphics::api::cleanup();
	return 0;
}