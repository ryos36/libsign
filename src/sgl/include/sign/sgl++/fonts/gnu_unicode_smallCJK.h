/* Copyright (c) 2008 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_SGL_FONTS_GNU_UNICODE_SMALL_CJK_H__
#define __SIGN_SGL_FONTS_GNU_UNICODE_SMALL_CJK_H__

#include "sign/sgl++/FontTable.h"

namespace sign {
namespace sgl {

class GnuUnicodeSmallCJKFontTable : public FontTable {
public:
	GnuUnicodeSmallCJKFontTable(void);

	virtual uint32 getCodeWidth(uint32 code);

protected:
	virtual uint32 codeToNo(uint32 code) const;

};

extern GnuUnicodeSmallCJKFontTable gnuUnicodeSmallCJKFontTable;

} // namespace sgl
} // namespace sign

#endif /* __SIGN_SGL_FONTS_GNU_UNICODE_SMALL_CJK_H__ */
