//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

#define etoj(c1, c2) {c1 &= 0x7f; c2 &= 0x7f;}

unsigned short
euc2jis(unsigned int euc)
{
	int j1, j2;
	j1 = (euc >> 8) & 0xFF;
	j2 = (euc & 0xFF);
	etoj(j1, j2);
	return (unsigned short)(j1*0x100 + j2);
}

unsigned short
sjis2jis(unsigned int sjis)
{
          unsigned int hib, lob;
          
          hib = (sjis >> 8) & 0xff;
          lob = sjis & 0xff;
          hib -= (hib <= 0x9f) ? 0x71 : 0xb1;
          hib = (hib << 1) + 1;
          if (lob > 0x7f) lob--;
          if (lob >= 0x9e) {
              lob -= 0x7d;
              hib++;
          } else lob -= 0x1f;

          return (hib << 8) | lob;
}

unsigned short
sjis2euc( unsigned int sjis)
{
	unsigned short us;
	unsigned char *cp;
	us = sjis2jis(sjis);
	cp = (unsigned char *)&us;
	*cp |= 0x80;
	++cp;
	*cp |= 0x80;

	return us;
}

////////////////////////////////////////////////////////////////////


