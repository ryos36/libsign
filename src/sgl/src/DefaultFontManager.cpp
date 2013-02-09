/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#include "sign/sgl++/DefaultFontManager.h"

namespace sign {
namespace sgl {

//-----------------------------------------------------
DefaultFontManager::DefaultFontManager(void)
{
	fontTable = 0;
}

//-----------------------------------------------------
DefaultFontManager::~DefaultFontManager(void)
{
	// nothing to do
}

//-----------------------------------------------------
FontTable *
DefaultFontManager::setFontTable(FontTable *fontTable)
{
	FontTable *oldFontTable;

	oldFontTable = this->fontTable;
	this->fontTable = fontTable;

	return oldFontTable;
}

//-----------------------------------------------------
FontTable *
DefaultFontManager::lookupFontTable(const char *fontName)
{
	return fontTable;
}

} // namespace sign
} // namespace sgl
