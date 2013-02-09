/* Copyright (c) 2004, 2007 Sinby Corporation, All Rights Reserved */

/* Sinby Hanami Font */
#ifndef __SIGN_SGL_FONTS_HANAMI_H__
#define __SIGN_SGL_FONTS_HANAMI_H__

#include "sign/sgl++/FontTable.h"

namespace sign {
namespace sgl {

class HanamiFontTable : public FontTable {
public:
	HanamiFontTable(void);
	~HanamiFontTable(void);

protected:
	uint32 codeToNo(uint32 code) const; 
};

extern HanamiFontTable hanamiFontTable;

} // namespace sgl
} // namespace sign

#endif /* __SIGN_SGL_FONTS_HANAMI_H__ */
