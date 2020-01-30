//
// Created by cbihan on 1/30/20.
//

#ifndef COMSQUARE_IRENDERER_HPP
#define COMSQUARE_IRENDERER_HPP

#include <string>

namespace ComSquare::Renderer
{
	class IRenderer {
	public:
		//! @brief Set a new name to the window, if there is already a name it will be overwrite
		virtual void setWindowName(std::string) = 0;
		//! @brief Tells to the program if the window has been closed, and therefore if he should stop
		bool shouldExit;
		//! @brief Render the buffer to the window
		virtual void drawScreen() = 0;
		//! @brief Set a pixel to the coordinates x, y with the color rgba
		virtual void putPixel(int x, int y, uint8_t rgba) = 0;
	};
}

#endif //COMSQUARE_IRENDERER_HPP