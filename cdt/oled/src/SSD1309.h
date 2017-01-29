#ifndef SSD1309_H_
#define SSD1309_H_

#define BLACK 0
#define WHITE 1
#define INVERSE 2


#define SSD1309_LCDWIDTH                  128
#define SSD1309_LCDHEIGHT                 64


#define SSD1309_SETCONTRAST 0x81
#define SSD1309_DISPLAYALLON_RESUME 0xA4
#define SSD1309_DISPLAYALLON 0xA5
#define SSD1309_NORMALDISPLAY 0xA6
#define SSD1309_INVERTDISPLAY 0xA7
#define SSD1309_DISPLAYOFF 0xAE
#define SSD1309_DISPLAYON 0xAF

#define SSD1309_SETDISPLAYOFFSET 0xD3
#define SSD1309_SETCOMPINS 0xDA

#define SSD1309_SETVCOMDETECT 0xDB

#define SSD1309_SETDISPLAYCLOCKDIV 0xD5
#define SSD1309_SETPRECHARGE 0xD9

#define SSD1309_SETMULTIPLEX 0xA8

#define SSD1309_SETLOWCOLUMN 0x00
#define SSD1309_SETHIGHCOLUMN 0x10

#define SSD1309_SETSTARTLINE 0x40

#define SSD1309_MEMORYMODE 0x20
#define SSD1309_COLUMNADDR 0x21
#define SSD1309_PAGEADDR   0x22

#define SSD1309_COMSCANINC 0xC0
#define SSD1309_COMSCANDEC 0xC8
#define SSD1309_SEGREMAP 0xA0
#define SSD1309_CHARGEPUMP 0x8D
#define SSD1309_EXTERNALVCC 0x1
#define SSD1309_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1309_ACTIVATE_SCROLL 0x2F
#define SSD1309_DEACTIVATE_SCROLL 0x2E
#define SSD1309_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1309_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1309_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1309_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1309_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#define WIDTH 128
#define HEIGHT 64

enum cmd { COMMAND, DATA };

void invertDisplay(uint8_t i);
int SendByte(cmd cmdType, int data);
void drawPixel(int16_t x, int16_t y, uint16_t color);
void drawFastVLine(int16_t x, int16_t __y, int16_t __h,
		uint16_t color);
void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t);
void fillCircleHelper(int16_t x0, int16_t y0, int16_t r,
 uint8_t cornername, int16_t delta, uint16_t color);


class Adafruit_SSD1309  {
 public:
  Adafruit_SSD1309(int8_t SID, int8_t SCLK, int8_t DC, int8_t RST, int8_t CS);
  Adafruit_SSD1309(int8_t DC, int8_t RST, int8_t CS);
  Adafruit_SSD1309(int8_t RST = -1);

  void begin(uint8_t switchvcc = SSD1309_SWITCHCAPVCC, uint8_t i2caddr=0x27, bool reset=true);
  void ssd1306_command(uint8_t c);

  void clearDisplay(void);
  void invertDisplay(uint8_t i);
  void display();

  void startscrollright(uint8_t start, uint8_t stop);
  void startscrollleft(uint8_t start, uint8_t stop);

  void startscrolldiagright(uint8_t start, uint8_t stop);
  void startscrolldiagleft(uint8_t start, uint8_t stop);
  void stopscroll(void);

  void dim(bool dim);

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
 // virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

 private:
  int8_t _i2caddr, _vccstate, sid, sclk, dc, rst, cs;
  void fastSPIwrite(uint8_t c);

  bool hwSPI;
#ifdef HAVE_PORTREG
  PortReg *mosiport, *clkport, *csport, *dcport;
  PortMask mosipinmask, clkpinmask, cspinmask, dcpinmask;
#endif

  //inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
 // inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

};

#endif /* _Adafruit_SSD1309_H_ */
