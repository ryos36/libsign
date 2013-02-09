/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_FONT_MANAGER_H__
#define __SIGN_FONT_MANAGER_H__

#include "FontTable.h"

namespace sign {
namespace sgl {
class DefaultFontManager : public FontManager {
public:
	DefaultFontManager(void);
	virtual ~DefaultFontManager(void);

	FontTable *setFontTable(FontTable *fontTable);
	FontTable *lookupFontTable(const char *fontName);

protected:
	FontTable *fontTable;
};

} // namespace sgl
} // namespace sign

#endif /* __SIGN_FONT_MANAGER_H__ */
