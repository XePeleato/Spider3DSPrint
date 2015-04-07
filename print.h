#include "spider.h"

#define VRAM_LOC	0x1F000000
#define BYTES_PER_PIXEL	3
#define BOTTOM_FB0_LOC	(void*)(VRAM_LOC + 0x0048F000)
#define BOTTOM_FB1_LOC	(void*)(VRAM_LOC + 0x004C7800)
#define BOTTOM_X_RES	320
#define BOTTOM_Y_RES	240
#define BOTTOM_FB_SIZE	BOTTOM_X_RES * BOTTOM_Y_RES * BYTES_PER_PIXEL
#define TOP_L_FB0_LOC	(void*)(VRAM_LOC + 0x001E6000)
#define TOP_L_FB1_LOC	(void*)(VRAM_LOC + 0x0022C800)
#define TOP_R_FB0_LOC	(void*)(VRAM_LOC + 0x00273000)
#define TOP_R_FB1_LOC	(void*)(VRAM_LOC + 0x002B9800)
#define TOP_FB0_LOC	TOP_L_FB0_LOC
#define TOP_FB1_LOC	TOP_L_FB1_LOC
#define TOP_X_RES	400
#define TOP_Y_RES	240
#define TOP_FB_SIZE	TOP_X_RES * TOP_Y_RES * BYTES_PER_PIXEL
#define MEM_STRIPE_SIZE BOTTOM_Y_RES * CHAR_WIDTH * BYTES_PER_PIXEL
#define CHAR_WIDTH	8
#define CHAR_HEIGHT	8
#define TOP_COLS	TOP_X_RES / CHAR_WIDTH
#define TOP_ROWS	TOP_Y_RES / CHAR_HEIGHT
#define BOTTOM_COLS	BOTTOM_X_RES / CHAR_WIDTH
#define BOTTOM_ROWS	BOTTOM_Y_RES / CHAR_HEIGHT
#define BUFFER_LOC	(void*)0x18410180
#define FONT_START	32
#define COLOR_FG_DEF	0x00FFFFFF
#define COLOR_BG_DEF	0x00000000
#define COLOR_TRANS	0xFF000000

#define position	0x18410000
#define col	0x18410004
#define row	0x18410008

unsigned char font[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x78,0x78,0x30,0x30,0x00,0x30,0x00,0x6C,0x6C,0x6C,0x00,0x00,0x00,0x00,0x00,0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00,0x30,0x7C,0xC0,0x78,0x0C,0xF8,0x30,0x00,0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00,0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00,0x60,0x60,0xC0,0x00,0x00,0x00,0x00,0x00,0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00,0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x30,0x30,0xFC,0x30,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00,0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00,0x7C,0xC6,0xCE,0xDE,0xF6,0xE6,0x7C,0x00,0x30,0x70,0x30,0x30,0x30,0x30,0xFC,0x00,0x78,0xCC,0x0C,0x38,0x60,0xCC,0xFC,0x00,0x78,0xCC,0x0C,0x38,0x0C,0xCC,0x78,0x00,0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x1E,0x00,0xFC,0xC0,0xF8,0x0C,0x0C,0xCC,0x78,0x00,0x38,0x60,0xC0,0xF8,0xCC,0xCC,0x78,0x00,0xFC,0xCC,0x0C,0x18,0x30,0x30,0x30,0x00,0x78,0xCC,0xCC,0x78,0xCC,0xCC,0x78,0x00,0x78,0xCC,0xCC,0x7C,0x0C,0x18,0x70,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x60,0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x00,0x00,0x00,0xFC,0x00,0x00,0xFC,0x00,0x00,0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00,0x78,0xCC,0x0C,0x18,0x30,0x00,0x30,0x00,0x7C,0xC6,0xDE,0xDE,0xDE,0xC0,0x78,0x00,0x30,0x78,0xCC,0xCC,0xFC,0xCC,0xCC,0x00,0xFC,0x66,0x66,0x7C,0x66,0x66,0xFC,0x00,0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00,0xF8,0x6C,0x66,0x66,0x66,0x6C,0xF8,0x00,0xFE,0x62,0x68,0x78,0x68,0x62,0xFE,0x00,0xFE,0x62,0x68,0x78,0x68,0x60,0xF0,0x00,0x3C,0x66,0xC0,0xC0,0xCE,0x66,0x3E,0x00,0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x1E,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00,0xE6,0x66,0x6C,0x78,0x6C,0x66,0xE6,0x00,0xF0,0x60,0x60,0x60,0x62,0x66,0xFE,0x00,0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00,0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00,0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x00,0xFC,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00,0x78,0xCC,0xCC,0xCC,0xDC,0x78,0x1C,0x00,0xFC,0x66,0x66,0x7C,0x6C,0x66,0xE6,0x00,0x78,0xCC,0xE0,0x70,0x1C,0xCC,0x78,0x00,0xFC,0xB4,0x30,0x30,0x30,0x30,0x78,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0xFC,0x00,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00,0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00,0xC6,0xC6,0x6C,0x38,0x38,0x6C,0xC6,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x30,0x78,0x00,0xFE,0xC6,0x8C,0x18,0x32,0x66,0xFE,0x00,0x78,0x60,0x60,0x60,0x60,0x60,0x78,0x00,0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00,0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00,0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x0C,0x7C,0xCC,0x76,0x00,0xE0,0x60,0x60,0x7C,0x66,0x66,0xDC,0x00,0x00,0x00,0x78,0xCC,0xC0,0xCC,0x78,0x00,0x1C,0x0C,0x0C,0x7C,0xCC,0xCC,0x76,0x00,0x00,0x00,0x78,0xCC,0xFC,0xC0,0x78,0x00,0x38,0x6C,0x60,0xF0,0x60,0x60,0xF0,0x00,0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0xF8,0xE0,0x60,0x6C,0x76,0x66,0x66,0xE6,0x00,0x30,0x00,0x70,0x30,0x30,0x30,0x78,0x00,0x0C,0x00,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0xE0,0x60,0x66,0x6C,0x78,0x6C,0xE6,0x00,0x70,0x30,0x30,0x30,0x30,0x30,0x78,0x00,0x00,0x00,0xCC,0xFE,0xFE,0xD6,0xC6,0x00,0x00,0x00,0xF8,0xCC,0xCC,0xCC,0xCC,0x00,0x00,0x00,0x78,0xCC,0xCC,0xCC,0x78,0x00,0x00,0x00,0xDC,0x66,0x66,0x7C,0x60,0xF0,0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0x1E,0x00,0x00,0xDC,0x76,0x66,0x60,0xF0,0x00,0x00,0x00,0x7C,0xC0,0x78,0x0C,0xF8,0x00,0x10,0x30,0x7C,0x30,0x30,0x34,0x18,0x00,0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x76,0x00,0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x00,0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x6C,0x00,0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6,0x00,0x00,0x00,0xCC,0xCC,0xCC,0x7C,0x0C,0xF8,0x00,0x00,0xFC,0x98,0x30,0x64,0xFC,0x00,0x1C,0x30,0x30,0xE0,0x30,0x30,0x1C,0x00,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00,0xE0,0x30,0x30,0x1C,0x30,0x30,0xE0,0x00,0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x38,0x6C,0xC6,0xC6,0xFE,0x00};
/*
void CopyMem(void *src, void *dst, unsigned size){ 
	GSPGPU_FlushDataCache(src, size); 
	GX_SetTextureCopy(src, dst, size, 0, 0, 0, 0, 8); 
	GSPGPU_FlushDataCache(dst, size); 
//	svcSleepThread(0x200000LL); 
}
*/
void print(char *value, unsigned fgcolor, unsigned bgcolor){
	(*(unsigned*)position) = 0;
	unsigned char c;
	while((c = value[(*(unsigned*)position)++]) != 0){
		if((*(unsigned*)col) >= BOTTOM_COLS){
			(*(unsigned*)col) = 0;
			(*(unsigned*)row)++;
		}
		if((*(unsigned*)row) >= BOTTOM_ROWS){
			(*(unsigned*)row) = 0;
		}
		switch(c){
			case '\b':
				if((*(unsigned*)col) == 0){
					(*(unsigned*)col) = BOTTOM_COLS;
					(*(unsigned*)row)--;
				}
				(*(unsigned*)col)--;
				break;
			case '\t':
				(*(unsigned*)col) += 8;
				break;
			case '\n':
				(*(unsigned*)row)++;
				break;
			case '\v':
				(*(unsigned*)row) += 6;
				break;
			case '\f':
				(*(unsigned*)col) = 0;
				(*(unsigned*)row) = 0;
				if(~bgcolor & COLOR_TRANS){
					for(unsigned i = 0; i < MEM_STRIPE_SIZE; i += 4){
						bgcolor |= bgcolor << 24;
						*(unsigned *)(BUFFER_LOC + i) = bgcolor;
						bgcolor >>= 8;
					}
					GSPGPU_FlushDataCache(BUFFER_LOC, MEM_STRIPE_SIZE); 
					for(unsigned i = 0; i < BOTTOM_FB_SIZE; i += MEM_STRIPE_SIZE){
//						CopyMem(BUFFER_LOC, (void*)(BOTTOM_FB0_LOC + i), MEM_STRIPE_SIZE);
//						CopyMem(BUFFER_LOC, (void*)(BOTTOM_FB1_LOC + i), MEM_STRIPE_SIZE);
						GX_SetTextureCopy(BUFFER_LOC, (void*)(BOTTOM_FB0_LOC + i), MEM_STRIPE_SIZE, 0, 0, 0, 0, 8); 
						GX_SetTextureCopy(BUFFER_LOC, (void*)(BOTTOM_FB1_LOC + i), MEM_STRIPE_SIZE, 0, 0, 0, 0, 8); 
					}
					GSPGPU_FlushDataCache(BOTTOM_FB0_LOC, BOTTOM_FB_SIZE); 
					GSPGPU_FlushDataCache(BOTTOM_FB1_LOC, BOTTOM_FB_SIZE); 
				}
				break;
			case '\r':
				(*(unsigned*)col) = 0;
				break;
			default:
				if(c >= FONT_START){
					unsigned fontoffs = c - FONT_START << 3;
					unsigned offs = ((BOTTOM_ROWS - *(unsigned*)row) * CHAR_HEIGHT - 1) * BYTES_PER_PIXEL;
//					CopyMem(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, BUFFER_LOC, MEM_STRIPE_SIZE);
					GSPGPU_FlushDataCache(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, MEM_STRIPE_SIZE); 
					GX_SetTextureCopy(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, BUFFER_LOC, MEM_STRIPE_SIZE, 0, 0, 0, 0, 8); 
					GSPGPU_FlushDataCache(BUFFER_LOC, BOTTOM_FB_SIZE); 
					for(unsigned y = 0; y < CHAR_HEIGHT; y++){
						unsigned char f = font[fontoffs++];
						for(unsigned x = 0; x < CHAR_WIDTH; x++){
							unsigned color;
							if(~(color = f << x & 0x80 ? fgcolor : bgcolor) & COLOR_TRANS){
								*(unsigned*)(BUFFER_LOC + offs) &= 0xFF000000;
								*(unsigned*)(BUFFER_LOC + offs) |= color;
							}
							offs += BOTTOM_Y_RES * BYTES_PER_PIXEL;
						}
						offs -= MEM_STRIPE_SIZE + BYTES_PER_PIXEL;
					}
//					CopyMem(BUFFER_LOC, (void*)(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE), MEM_STRIPE_SIZE);
//					CopyMem(BUFFER_LOC, (void*)(BOTTOM_FB1_LOC + *(unsigned*)col * MEM_STRIPE_SIZE), MEM_STRIPE_SIZE);
					GSPGPU_FlushDataCache(BUFFER_LOC, MEM_STRIPE_SIZE); 
					GX_SetTextureCopy(BUFFER_LOC, (void*)(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, MEM_STRIPE_SIZE, 0, 0, 0, 0, 8); 
					GX_SetTextureCopy(BUFFER_LOC, (void*)(BOTTOM_FB1_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, MEM_STRIPE_SIZE, 0, 0, 0, 0, 8); 
					GSPGPU_FlushDataCache((void*)(BOTTOM_FB0_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, MEM_STRIPE_SIZE); 
					GSPGPU_FlushDataCache((void*)(BOTTOM_FB1_LOC + *(unsigned*)col * MEM_STRIPE_SIZE, MEM_STRIPE_SIZE); 
				}
				(*(unsigned*)col)++;
				break;
		}
	}
}

void printint(unsigned value, unsigned fgcolor, unsigned bgcolor){
	unsigned i, c;
	for(i = 0; i < 8; i++){
		c = value >> 28 - (i << 2) & 0x0F;
		c += (c > 9 ? 0x37 : 0x30);
		print((unsigned char*)&c, fgcolor, bgcolor);
	}
}

void printw(unsigned char* value, unsigned fgcolor, unsigned bgcolor){
	unsigned i = 0;
	while(value[i] != 0){
		print((unsigned char*)&(value[i]), fgcolor, bgcolor);
		i += 2;
	}
}
