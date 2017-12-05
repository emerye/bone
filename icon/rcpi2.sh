#!/bin/bash



#convert toolicon.png -define icon:auto-resize=16,32,48,64,256 \
convert toolicon.png -resize 16x16 -alpha set -define bmp:format=bmp4 toolicon_16_32.bmp
convert toolicon.png -resize 32x32 -alpha set -define bmp:format=bmp4 toolicon_32_32.bmp
convert toolicon.png -resize 48x48 -alpha set -define bmp:format=bmp toolicon_48_32.bmp
convert toolicon.png -resize 256x256 -alpha set -define bmp:format=bmp toolicon_256_32.bmp
convert toolicon.png -resize 16x16 -colors 256 -depth 4 toolicon_16_8.bmp
convert toolicon.png -resize 32x32 -colors 256 -depth 4 toolicon_32_8.bmp
convert toolicon.png -resize 48x48 -colors 256 -depth 4 toolicon_48_8.bmp
#convert toolicon_16_32.ico toolicon_32_32.ico toolicon_48_32.ico \
# toolicon_256_32.ico -colors 256 toolicon_16_8.ico toolicon_32_8.ico \
# toolicon_48_8.ico toolicon.ico
