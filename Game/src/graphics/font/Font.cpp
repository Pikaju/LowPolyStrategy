#include "Font.h"

#include <fstream>

#include "../../debug/Debug.h"

namespace graphics {

	Font::Font() : m_texture(), m_characters()
	{
	}

	Font::Font(const char* file) : m_texture(), m_characters()
	{
		initialize(file);
	}

	Font::~Font()
	{
	}

	void Font::initialize(const char* file)
	{
		std::ifstream in(file);
		if (!in.is_open()) {
			debug::crash(std::string("Could not find font file: \"") + file + "\"");
		}

		Character currentCharInfo;
		char currentChar;
		bool started = false;

		std::string currentWord;
		while (!in.eof()) {
			in >> currentWord;
			if (currentWord.substr(0, 5) == "file=") {
				m_texture.initialize(("res/fonts/" + currentWord.substr(6, currentWord.length() - 6 - 1)).c_str());
			}
			else if (currentWord.substr(0, 4) == "char") {
				if (started) {
					m_characters[currentChar] = currentCharInfo;
				}
				else {
					started = true;
				}
			}
			else if (currentWord.substr(0, 3) == "id=") {
				currentChar = (char)atoi(currentWord.substr(3, currentWord.length() - 3).c_str());
			}
			else if (currentWord.substr(0, 2) == "x=") {
				currentCharInfo.x = atoi(currentWord.substr(2, currentWord.length() - 2).c_str());
			}
			else if (currentWord.substr(0, 2) == "y=") {
				currentCharInfo.y = atoi(currentWord.substr(2, currentWord.length() - 2).c_str());
			}
			else if (currentWord.substr(0, 6) == "width=") {
				currentCharInfo.width = atoi(currentWord.substr(6, currentWord.length() - 6).c_str());
			}
			else if (currentWord.substr(0, 7) == "height=") {
				currentCharInfo.height = atoi(currentWord.substr(7, currentWord.length() - 7).c_str());
			}
			else if (currentWord.substr(0, 8) == "xoffset=") {
				currentCharInfo.xOffset = atoi(currentWord.substr(8, currentWord.length() - 8).c_str());
			}
			else if (currentWord.substr(0, 8) == "yoffset=") {
				currentCharInfo.yOffset = atoi(currentWord.substr(8, currentWord.length() - 8).c_str());
			}
			else if (currentWord.substr(0, 9) == "xadvance=") {
				currentCharInfo.xAdvance = atoi(currentWord.substr(9, currentWord.length() - 9).c_str());
			}
		}
		if (started) {
			m_characters[currentChar] = currentCharInfo;
		}
	}
	
	void Font::cleanup()
	{
		m_texture.cleanup();
	}

	int Font::getStringWidth(const char* text)
	{
		int width = 0;
		for (unsigned int i = 0; text[i] != '\0'; i++) {
			width += getCharacter(text[i]).xAdvance - 8;
		}
		return width;
	}

}