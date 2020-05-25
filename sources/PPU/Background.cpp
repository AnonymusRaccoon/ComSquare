//
// Created by cbihan on 5/14/20.
//

#include "Background.hpp"
#include "PPUUtils.hpp"

namespace ComSquare::PPU
{
	Background::Background(int bpp, Vector2<int> backgroundSize, Vector2<int> characterSize, bool directColor, bool highRes, bool priority, uint16_t vramAddress, uint16_t graphicVramAddress):
		_bpp(bpp),
		_backgroundSize(backgroundSize),
		_characterSize(characterSize),
		_highRes(highRes),
		_directColor(directColor),
		_priority(priority),
		_vramAddress(vramAddress),
		_graphicVramAddress(graphicVramAddress)
	{
	}


	void Background::renderBackground(void)
	{
		uint16_t vramAddress = this->_vramAddress;
		Vector2<int> offset(0, 0);

		for (int i = 0; i < 4; i++) {
			if (!(i == 1 && this->_backgroundSize.x == 1) && !(i > 1 && this->_backgroundSize.y == 1)) {
				drawBasicTileMap(vramAddress, offset);
			}
			vramAddress+= 0x800;
			offset.x += 32 * this->_characterSize.x;
			if (i == 2) {
				offset.x = 0;
				offset.y += 32 * this->_characterSize.y;
			}
		}
	}

	void Background::drawBgTile(uint16_t data, Vector2<int> pos)
	{
		uint16_t graphicAddress;
		union TileMapData tileData;
		std::vector<uint16_t> palette;
		int index = 0;
		uint8_t reference = 0;
		uint32_t color = 0;

		tileData.raw = data;
		graphicAddress = this->_graphicVramAddress;
		for (int i = 0; i < this->_characterSize.y; i++) {
			for (int j = 0; j < this->_characterSize.x; j++) {
				palette = getPalette(tileData.palette);
				reference = getTilePixelReference(graphicAddress, index);
				color = getRealColor(palette[reference]);
				this->_buffer[pos.x][pos.y] = color;
				index++;
				pos.x++;
				if (index == (8 / this->_bpp) - 1) {
					index = 0;
					graphicAddress++;
				}
			}
			index = 0;
			pos.x -= this->_characterSize.x;
			pos.y++;
		}
	}

	std::vector<uint16_t> Background::getPalette(int nbPalette)
	{
		std::vector<uint16_t> palette(0xF);

		uint16_t addr = nbPalette * 0x10;
		for (int i = 0; i < 0xF; i++) {
			palette[i] = this->cgramRead(addr);
			palette[i] += this->cgramRead(addr + 1) << 8U;
		}
		return palette;
	}

	uint32_t Background::getRealColor(uint16_t color)
	{
		uint8_t blue;
		uint8_t red;
		uint8_t green;
		uint32_t pixelTmp;

		blue = (color & 0x7D00U) >> 10U;
		green = (color & 0x03E0U) >> 5U;
		red = (color & 0x001FU);

		pixelTmp = 0xFFFF;
		pixelTmp += (red * 255U / 31U) << 24U;
		pixelTmp += (green * 255U / 31U) << 16U;
		pixelTmp += (blue * 255U / 31U) << 8U;
		return pixelTmp;
	}

	uint8_t Background::getTilePixelReference(uint16_t addr, int nb)
	{
		uint8_t reference = this->vram->read_internal(addr);

		switch (this->_bpp) {
		case 8:
			return reference;
		case 4:
			return (reference & (0xFU << ((1 - nb) * 4U))) >> (1 - nb) * 4U;
		case 2:
			return (reference & (0x3U << ((3 - nb) * 2U))) >> (3 - nb) * 2U;
		default:
			break;
		}
		return 0;
	}

	void Background::drawBasicTileMap(uint16_t baseAddress, Vector2<int> offset)
	{
		uint16_t tileMapValue = 0;
		Vector2<int> pos(0,0);
		uint16_t vramAddress = baseAddress;

		while (vramAddress < 0x800 + baseAddress) {
			tileMapValue = this->vram->read_internal(vramAddress);
			tileMapValue += this->vram->read_internal(vramAddress + 1) << 8U;
			vramAddress += 2;
			drawBgTile(tileMapValue, {(pos.x * this->_characterSize.x) + offset.x, (pos.y * this->_characterSize.y) + offset.y}, this->_bpp);
			if (pos.x % 31 == 0 && pos.x) {
				pos.y++;
				pos.x = 0;
			}
			else
				pos.x++;
		}
	}
}