/* Copyright (c) 2007 Sinby Corporation, All Rights Reserved */

#ifndef __SIGN_GNU_UNICODE_FONT_H__
#define __SIGN_GNU_UNICODE_FONT_H__

#include "sign/sgl++/FontTable.h"

namespace sign {
namespace sgl {

class GnuUnicodeFontTable : public FontTable {
public:
	GnuUnicodeFontTable(void);
	~GnuUnicodeFontTable(void);

	virtual uint32 getCodeWidth(uint32 code);

protected:
	virtual uint32 codeToNo(uint32 code) const;

};
extern class GnuUnicodeFontTable GnuUnicodeFontTable;

} // namespace sgl
} // namespace sign

#endif //__GNU_UNICODE_FONT_H__
