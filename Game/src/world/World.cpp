#include "World.h"

#include "../graphics/Window.h"
#include "../graphics/renderer/AttributelessRenderer.h"

namespace world {

	World::World() : m_chunkManager(this), m_entityManager(this), m_camera(math::Vec3f(0.0f, 0.0f, 0.0f), math::Vec3f(60.0f, 0.0f, 0.0f))
	{
		m_camera.distance = 16.0f;
	}
	
	World::~World()
	{
	}

	void World::initialize()
	{
		m_chunkManager.initialize();
		m_entityManager.initialize();

		m_screenShader.initialize();
		m_screenShader.loadShader("res/shaders/Screen.vert", graphics::SHADERTYPE_VERTEX);
		m_screenShader.loadShader("res/shaders/Screen.frag", graphics::SHADERTYPE_FRAGMENT);
		m_screenShader.link();

		m_ssao.initialize();
		m_bloom.initialize();
	}
	
	void World::cleanup()
	{
		m_bloom.cleanup();
		m_ssao.cleanup();

		m_screenShader.cleanup();

		m_entityManager.cleanup();
		m_chunkManager.cleanup();
	}

	void World::update(const io::Input& input, float delta)
	{
		m_chunkManager.update(delta);
		m_entityManager.update(delta);
		m_camera.debugUpdate(input, delta);
		while (m_chunkManager.getHeight(m_camera.position.x, m_camera.position.z) > m_camera.position.y - 2.0f) {
			m_camera.distance += 1.0f / 128.0f;
			m_camera.calculatePosition();
		}
	}

	void World::render(const graphics::Window& window)
	{
		if (m_framebuffer.getWidth() != window.getWidth() || m_framebuffer.getHeight() != window.getHeight()) {
			m_framebuffer.cleanup();
			std::vector<graphics::Attachment> attachments;
			attachments.push_back(graphics::ATTACHMENT_COLOR);
			attachments.push_back(graphics::ATTACHMENT_COLOR);
			attachments.push_back(graphics::ATTACHMENT_DEPTH);
			std::vector<graphics::Format> formats;
			formats.push_back(graphics::FORMAT_RGBA8);
			formats.push_back(graphics::FORMAT_RGB8);
			formats.push_back(graphics::FORMAT_DEPTH24);
			m_framebuffer.initialize(window.getWidth(), window.getHeight(), attachments.size(), attachments.data(), formats.data());

			m_ssao.setFramebufferSize(m_framebuffer.getWidth(), m_framebuffer.getHeight());
			m_bloom.setFramebufferSize(m_framebuffer.getWidth(), m_framebuffer.getHeight());
		}

		graphics::AttributelessRenderer renderer;

		math::Mat4f viewMatrix = m_camera.getViewMatrix();
		math::Mat4f projectionMatrix = math::Mat4f::perspective(window.getAspectRatio(), 90.0f, 0.125f, 512.0f);
		
		m_frustumCuller.extractPlanes(viewMatrix * projectionMatrix);

		m_framebuffer.bind();
		
		renderer.clear(true, true);
		m_chunkManager.render(viewMatrix, projectionMatrix);
		m_framebuffer.bind();
		m_entityManager.render(viewMatrix, projectionMatrix);
		renderer.setBlending(true, false);
		m_chunkManager.renderWater(viewMatrix, projectionMatrix, m_camera.position);
		renderer.setBlending(false, false);
		
		m_ssao.render(m_framebuffer, 1, 2, viewMatrix, projectionMatrix);
		m_bloom.render(m_framebuffer, 0);

		window.bind();

		m_framebuffer.use(0, 0);
		m_framebuffer.use(1, 1);
		m_framebuffer.use(2, 2);
		m_ssao.getFramebuffer().use(0, 3);
		m_bloom.getFramebuffer().use(0, 4);

		m_screenShader.enable();
		m_screenShader.setUniform("inverseViewMatrix", math::Mat4f::inverse(viewMatrix));
		m_screenShader.setUniform("inverseProjectionMatrix", math::Mat4f::inverse(projectionMatrix));
		m_screenShader.setUniform("renderedColor", 0);
		m_screenShader.setUniform("renderedNormal", 1);
		m_screenShader.setUniform("renderedDepth", 2);
		m_screenShader.setUniform("renderedSSAO", 3);
		m_screenShader.setUniform("renderedBloom", 4);

		renderer.renderTriangleStrip(4);

		m_screenShader.disable();
	}

}