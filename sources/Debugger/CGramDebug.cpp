//
// Created by cbihan on 3/27/20.
//

#include "CGramDebug.hpp"
#include "../SNES.hpp"
#include <QColor>
#include <string>
#include <iostream>
#include <QtWidgets/QTableWidget>
#include "../Utility/Utility.hpp"

namespace ComSquare::Debugger
{
	CGramDebug::CGramDebug(SNES &snes, ComSquare::PPU::PPU &ppu)
		: _window(new ClosableWindow<CGramDebug>(*this, &CGramDebug::disableViewer)),
		  _snes(snes),
		  _ui(),
		  _model(ppu),
		  _ppu(ppu)
	{
		this->_window->setContextMenuPolicy(Qt::NoContextMenu);
		this->_window->setAttribute(Qt::WA_QuitOnClose, false);
		this->_window->setAttribute(Qt::WA_DeleteOnClose);

		this->_ui.setupUi(this->_window);
		QMainWindow::connect(this->_ui.cgram_view, &QTableView::pressed, this, &CGramDebug::tileClicked);
		this->_ui.cgram_view->setModel(&this->_model);
		updateInfoTile(0, 0);
		this->_window->show();
		QEvent::registerEventType();
	}

	void CGramDebug::disableViewer()
	{
		this->_snes.disableCgramDebugging();
	}

	void CGramDebug::focus()
	{
		this->_window->activateWindow();
	}

	bool CGramDebug::isDebugger()
	{
		return true;
	}

	uint16_t CGramDebug::read(uint8_t addr)
	{
		return this->_ppu.cgramRead(addr);
	}

	void CGramDebug::updateInfoTile(int row, int column)
	{
		int idTile = row * 16 + column;
		uint16_t addr = idTile / 8 * 16 + (idTile % 8 * 2);
		uint16_t cgramValue = this->_ppu.cgramRead(addr);
		cgramValue += this->_ppu.cgramRead(addr + 1) << 8;
		uint8_t blue = (cgramValue & 0x7D00U) >> 10U;
		uint8_t green = (cgramValue & 0x03E0U) >> 5U;
		uint8_t red = (cgramValue & 0x001FU);
		uint24_t hexColorValue = 0;

		this->_ui.indexLineEdit->setText(std::to_string(addr / 2).c_str());
		this->_ui.valueLineEdit->setText(Utility::to_hex(cgramValue).c_str());
		this->_ui.rLineEdit->setText(std::to_string(red).c_str());
		this->_ui.gLineEdit->setText(std::to_string(green).c_str());
		this->_ui.bLineEdit->setText(std::to_string(blue).c_str());
		hexColorValue += (red * 255U / 31U) << 16U;
		hexColorValue += (green * 255U / 31U) << 8U;
		hexColorValue += (blue * 255U / 31U);
		this->_ui.hexLineEdit->setText(Utility::to_hex(hexColorValue).c_str());
	}

	void CGramDebug::tileClicked(const QModelIndex &index)
	{
		if (!index.isValid())
			return;
		this->updateInfoTile(index.row(), index.column());
	}
}

CGramModel::CGramModel(ComSquare::PPU::PPU &ppu) : _ppu(ppu) {}

int CGramModel::rowCount(const QModelIndex &) const
{
	return this->rows;
}

int CGramModel::columnCount(const QModelIndex &) const
{
	return this->column;
}

QVariant CGramModel::data(const QModelIndex &index, int role) const
{
	u_int16_t addressValue;
	uint8_t red;
	uint8_t green;
	uint8_t blue;

	if (role == Qt::TextAlignmentRole)
		return Qt::AlignCenter;
	if (role != Qt::BackgroundRole)
		return QVariant();
	int idDisplayTile = index.row() * 16 + index.column();
	uint16_t cgramAddress = idDisplayTile / 8 * 16 + (idDisplayTile % 8 * 2);
	addressValue = this->_ppu.cgramRead(cgramAddress);
	addressValue += this->_ppu.cgramRead(cgramAddress + 1) << 8U;

	blue = (addressValue & 0x7D00U) >> 10U;
	green = (addressValue & 0x03E0U) >> 5U;
	red = (addressValue & 0x001FU);

	red = red * 255U / 31U;
	green = green * 255U / 31U;
	blue = blue * 255U / 31U;
	return QColor(red, green, blue);
}