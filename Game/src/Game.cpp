#include "Game.h"

#include "graphics/Window.h"

Game::Game() : m_world(), m_button("Test Button", 20, 100, 300, 30, 0.5f)
{
}

Game::~Game()
{
}

void Game::initialize()
{
	m_world.initialize();
	m_fontRenderer.intialize();
	m_fontRenderer.addFont(new graphics::Font("res/fonts/CalibriLight.fnt"));
}

void Game::cleanup()
{
	m_fontRenderer.cleanup();
	m_world.cleanup();
}

void Game::update(const io::Input& input, float delta)
{
	m_world.update(input, delta);
}

void Game::render(const graphics::Window& window)
{
	m_world.render(window);
	m_fontRenderer.setScreenSize(math::Vec2i(window.getWidth(), window.getHeight()));
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	m_fontRenderer.render("Testing the ASDF012445678", window.getWidth() / 2 - m_fontRenderer.getFont(0)->getStringWidth("Testing the ASDF012445678") / 2 / 2, 4, 0.5f, 0);
	//m_button.render(m_fontRenderer);
}