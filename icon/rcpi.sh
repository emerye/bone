#!/bin/bash



#convert toolicon.png -define icon:auto-resize=16,32,48,64,256 \
convert toolicon.png -resize 16x16 toolicon_16_32.ico
convert toolicon.png -resize 32x32 toolicon_32_32.ico
convert toolicon.png -resize 48x48 toolicon_48_32.ico
convert toolicon.png -resize 256x256 toolicon_256_32.ico
#convert toolicon.png -resize 16x16 -colors 256 -depth 8 toolicon_16_8.ico
#convert toolicon.png -resize 32x32 -colors 256 -depth 8 toolicon_32_8.ico
#convert toolicon.png -resize 48x48 -colors 256 -depth 8 toolicon_48_8.ico
   convert toolicon_16_32.ico toolicon_32_32.ico toolicon_48_32.ico \
	toolicon_256_32.ico toolicon2.ico
