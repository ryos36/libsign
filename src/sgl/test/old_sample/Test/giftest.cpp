
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "gif_lib.h"



#define PACK_RGB(r,g,b)	( ((b)<<16 ) | ((g)<<8) | (r) )

//-------------------------------------------------------------
int 
readFunc(GifFileType* gif, GifByteType* buf, int count )
{
    char* ptr = (char*)gif->UserData;
    memcpy( buf, ptr, count );
    gif->UserData = ptr + count;
    return count;
}


//-------------------------------------------------------------
bool
scanImageDescRecordType( GifFileType *gif, unsigned char *out_buf, unsigned int* palette )
{
	if ( DGifGetImageDesc(gif) == GIF_ERROR ) {
		return false;
	}
	
	if (gif->Image.Left + gif->Image.Width > gif->SWidth ||
		gif->Image.Top + gif->Image.Height > gif->SHeight) {
		return false;
	}
	
	// Update Color map
	ColorMapObject *colorMap;
	colorMap = ( gif->Image.ColorMap ? gif->Image.ColorMap : gif->SColorMap);
	int i = colorMap->ColorCount;
	while (--i >= 0) {
		const GifColorType &col = colorMap->Colors[i];
		palette[i] = PACK_RGB( col.Red, col.Green, col.Blue );
	}
	
	int row    = gif->Image.Top; // Image Position relative to Screen.
	int col    = gif->Image.Left;
	int width  = gif->Image.Width;
	int height = gif->Image.Height;
	
	if (gif->Image.Interlace) {
		static const int InterlacedOffset[] = { 0, 4, 2, 1 }; /* The way Interlaced image should. */
		static const int InterlacedJumps[] = { 8, 8, 4, 2 };  /* be read - offsets and jumps... */
		// Need to perform 4 passes on the images:
				
		for ( int i = 0; i < 4; ++i ) {
			for ( int j = row + InterlacedOffset[i];
			      j < (row + height);
			      j += InterlacedJumps[i] ) 
			{
				if ( DGifGetLine( gif, &out_buf[ j*width+col ], width) == GIF_ERROR ) {
					return false;
				}
			}
		}
	}
	else {
		unsigned char *buf = out_buf+(row*width+col);
		for ( int i = row; i < (row + height); ++i, buf+=width ) 
		{
			if ( DGifGetLine( gif, buf, width) == GIF_ERROR ) {
				return false;
			}
			
		}
	}
	
	return true;
}

//-------------------------------------------------------------
bool
scanExtensionRecordType( GifFileType *gif )
{
	int extCode;
	GifByteType *extension;
	
	// Skip any extension blocks in file:
	if ( DGifGetExtension( gif, &extCode, &extension) == GIF_ERROR ) {
		return false;
	}
	while ( extension != NULL ) {
		if ( DGifGetExtensionNext( gif, &extension ) == GIF_ERROR ) {
			return false;
		}
	}
	
	return true;
}

//-------------------------------------------------------------
//バッファを背景色のインデックスで埋める
void
fillBackground( GifFileType *gif, unsigned char *out_buf )
{
	for ( int i = 0; i < gif->SWidth; ++i ) {
		out_buf[i] = gif->SBackGroundColor;
	}
	for ( int i = 1; i < gif->SHeight; ++i ) {
		memcpy( &out_buf[ i*gif->SWidth ], out_buf, gif->SWidth );
	}
}

//-------------------------------------------------------------
int 
decodeGIF( const unsigned char *userData, unsigned char *out_buf, unsigned int* palette, unsigned int *width, unsigned int *height )
{
	memset ( out_buf, 0, 240*160 );

    GifFileType *gif;
	if ( ( gif = DGifOpen((void*)userData, readFunc)) == NULL ) {
		return EXIT_FAILURE;
    }

	*width  = gif->SWidth;
	*height = gif->SHeight;

	fillBackground( gif, out_buf );

	GifRecordType recordType;
    do {
		if ( DGifGetRecordType( gif, &recordType ) == GIF_ERROR ) {
			return EXIT_FAILURE;
		}
		
		switch ( recordType ) {
			case IMAGE_DESC_RECORD_TYPE:
				if ( !scanImageDescRecordType( gif, out_buf, palette ) ) {
					return EXIT_FAILURE;
				}
				break;
				
			case EXTENSION_RECORD_TYPE:
				if ( !scanExtensionRecordType( gif ) ) {
					return EXIT_FAILURE;
				}
				break;
				
			case TERMINATE_RECORD_TYPE:
				break;
			
			default:		    // Should be traps by DGifGetRecordType.
				break;
		}
    } while ( recordType != TERMINATE_RECORD_TYPE );
    
    
    if ( DGifCloseFile( gif ) == GIF_ERROR ) {
		return EXIT_FAILURE;
    }
    
    return 0;
}


//-------------------------------------------------------------

bool 
decode_main( const unsigned char *file_stream, unsigned char *buf, unsigned int *paletteMem, unsigned int *width, unsigned int *height )
{
	//file_stream = cover;
    //const unsigned char *pict = porsche;
	
	/* Convert GIF to 256-color picture */
	return decodeGIF(file_stream, buf, paletteMem, width, height ) == 0;
}
