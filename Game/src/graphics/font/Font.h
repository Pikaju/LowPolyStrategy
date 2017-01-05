#pragma once

#include <unordered_map>

#include "../texture/Texture.h"

namespace graphics {

	class Character
	{
	public:
		int x;
		int y;
		int width;
		int height;
		int xOffset;
		int yOffset;
		int xAdvance;

		Character(int x, int y, int width, int height, int xOffset, int yOffset, int xAdvance) : x(x), y(y), width(width), height(height), xOffset(xOffset), yOffset(yOffset), xAdvance(xAdvance) {  }
		Character() : x(), y(), width(), height(), xOffset(), yOffset(), xAdvance() {  }
		~Character() {  }
	};

	class Font
	{
	public:
		Font();
		Font(const char* file);
		~Font();

		void initialize(const char* file);
		void cleanup();

		int getStringWidth(const char* text);

		inline const Character& getCharacter(char c) const { return m_characters.find(c)->second; }
		inline const graphics::Texture& getTexture() const { return m_texture; }
	private:
		graphics::Texture m_texture;
		std::unordered_map<char, Character> m_characters;
	};

}