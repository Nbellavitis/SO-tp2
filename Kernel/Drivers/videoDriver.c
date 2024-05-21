#include <videoDriver.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

uint32_t cursorX  = 0;
uint32_t cursorY  = 0;
uint32_t size = FONT_SIZE;

// uint8_t buff[256*16];  

// extern void getVGAfont(uint8_t * buffer);

void putPixel(uint32_t hexColor, uint32_t x, uint32_t y) {
    uint8_t * framebuffer = (uint8_t *) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

void drawRectangle(uint32_t color, uint32_t x, uint32_t y, uint32_t height, uint32_t width){
	for(int i=0; i< width; i++){
		for(int j=0; j < height; j++){
			putPixel(color, x + i , y + j);
		}
	}
}

void drawSquare(uint32_t hexColor, uint32_t side_length, uint32_t x, uint32_t y){
	drawRectangle(hexColor, x, y, side_length, side_length);
}
void drawChar(uint32_t hexColor, char character){
	int x = cursorX;
	int y = cursorY;
	int current = x;
	int index = character - 33;
	if(character == " ")
		return;

	for(int i=0; i< 32;i++){
		if (i % 2 == 0 && i != 0) {
			current = x;  
            y += size;  // Salto a la siguiente fila de píxeles
        }
		font[i + index*32] & (char)0x01 ? drawSquare(current, y, size, hexColor ) : 1;
		current += size;
		uint8_t aux = 0x02;

        for (int j = 0; j < 8; j++) {
            // Comprueba cada bit de la fuente y dibuja un píxel si está activo
            ((uint8_t)font[i + (index * 32)] & (uint8_t)aux) >> j ? put_square(current, y, size, hexColor) : 0;
            current += size;  // Avanza a la siguiente posición horizontal
            aux <<= 1;  // Desplaza el bit auxiliar hacia la izquierda
        }

	}
}