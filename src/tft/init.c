void __init ssd1963_setup(struct ssd1963 *item)
{
	dev_dbg(item->dev, "%s: item=0x%p\n", __func__, (void *)item);

	nhd_init_gpio_regs();

	at91_set_gpio_output(AT91_PIN_PE27, 0); //RESET
	udelay(5);							//TODO if not works try using ms instead of us;
	at91_set_gpio_output(AT91_PIN_PE27, 1); //RESET
	udelay(100);							//TODO if not works try using ms instead of us;

	nhd_write_data(NHD_COMMAND, 0x01); 		//Software Reset
	nhd_write_data(NHD_COMMAND, 0x01);
	nhd_write_data(NHD_COMMAND, 0x01);
	udelay(10);
	nhd_write_to_register(0xe0, 0x01);    		//START PLL
	udelay(100);
	nhd_write_to_register(0xe0, 0x03);    		//LOCK PLL
	nhd_write_data(NHD_COMMAND, 0xb0);		//SET LCD MODE  SET TFT 18Bits MODE
	nhd_write_data(NHD_DATA, 0x0c);			//SET TFT MODE 24 bits & hsync+Vsync+DEN MODE
	nhd_write_data(NHD_DATA, 0x80);			//SET TFT MODE & hsync+Vsync+DEN MODE           !!!!
	nhd_write_data(NHD_DATA, 0x01);			//SET horizontal size=320-1 HightByte
	nhd_write_data(NHD_DATA, 0x3f);		    //SET horizontal size=320-1 LowByte
	nhd_write_data(NHD_DATA, 0x00);			//SET vertical size=240-1 HightByte
	nhd_write_data(NHD_DATA, 0xef);			//SET vertical size=240-1 LowByte
	nhd_write_data(NHD_DATA, 0x00);			//SET even/odd line RGB seq.=RGB
	nhd_write_to_register(0xf0,0x00);	        //SET pixel data I/F format=8bit
	nhd_write_to_register(0x3a,0x70);           //SET R G B format = 8 8 8
	nhd_write_data(NHD_COMMAND, 0xe6);   	//SET PCLK freq=6.4MHz  ; pixel clock frequency
	nhd_write_data(NHD_DATA, 0x00);
	nhd_write_data(NHD_DATA, 0xe7);
	nhd_write_data(NHD_DATA, 0x4f);
	nhd_write_data(NHD_COMMAND, 0xb4);		//SET HBP,
	nhd_write_data(NHD_DATA, 0x01);			//SET HSYNC Total 440
	nhd_write_data(NHD_DATA, 0xb8);
	nhd_write_data(NHD_DATA, 0x00);			//SET HBP 68
	nhd_write_data(NHD_DATA, 0x44);
	nhd_write_data(NHD_DATA, 0x0f);			//SET VBP 16=15+1
	nhd_write_data(NHD_DATA, 0x00);			//SET Hsync pulse start position
	nhd_write_data(NHD_DATA, 0x00);
	nhd_write_data(NHD_DATA, 0x00);			//SET Hsync pulse subpixel start position
	nhd_write_data(NHD_COMMAND, 0xb6); 		//SET VBP,
	nhd_write_data(NHD_DATA, 0x01);			//SET Vsync total 265=264+1
	nhd_write_data(NHD_DATA, 0x08);
	nhd_write_data(NHD_DATA, 0x00);			//SET VBP=19
	nhd_write_data(NHD_DATA, 0x13);
	nhd_write_data(NHD_DATA, 0x07);			//SET Vsync pulse 8=7+1
	nhd_write_data(NHD_DATA, 0x00);			//SET Vsync pulse start position
	nhd_write_data(NHD_DATA, 0x00);
	nhd_write_data(NHD_COMMAND, 0x2a);		//SET column address
	nhd_write_data(NHD_DATA, 0x00);			//SET start column address=0
	nhd_write_data(NHD_DATA, 0x00);
	nhd_write_data(NHD_DATA, 0x01);			//SET end column address=319
	nhd_write_data(NHD_DATA, 0x3f);
	nhd_write_data(NHD_COMMAND, 0x2b);		//SET page address
	nhd_write_data(NHD_DATA, 0x00);			//SET start page address=0
	nhd_write_data(NHD_DATA, 0x00);
	nhd_write_data(NHD_DATA, 0x00);			//SET end page address=239
	nhd_write_data(NHD_DATA, 0xef);
	nhd_write_data(NHD_COMMAND, 0x29);		//SET display on

	nhd_set_window(0x0000, 0x013f, 0x0000, 0x00ef);
	nhd_write_data(NHD_COMMAND, 0x2c);

	nhd_clear_graph();

	printk(KERN_ALERT "COLOR LCD driver initialized\n");
