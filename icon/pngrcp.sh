#!/bin/bash



#convert toolicon.png -define icon:auto-resize=16,32,48,64,256 \
convert toolicon.png -resize 16x16 png32:toolicon_16_32.png
convert toolicon.png -resize 32x32 png32:toolicon_32_32.png
convert toolicon.png -resize 48x48 png32:toolicon_48_32.png
convert toolicon.png -resize 256x256 png32:toolicon_256_32.png
#convert toolicon.png -resize 256x256 toolicon_256_32.png
convert toolicon.png -resize 16x16 -colors 256 -channel Alpha png8:toolicon_16_8.png
convert toolicon.png -resize 32x32 -colors 256 -channel Alpha  png8:toolicon_32_8.png
convert toolicon.png -resize 48x48 -colors 256 -channel Alpha png8:toolicon_48_8.png
#convert toolicon.png -resize 16x16 -colors 256 -depth 8 toolicon_16_8.ico
#convert toolicon.png -resize 32x32 -colors 256 -depth 8 toolicon_32_8.ico
#convert toolicon.png -resize 48x48 -colors 256 -depth 8 toolicon_48_8.ico
   convert toolicon_16_32.png toolicon_32_32.png toolicon_48_32.png \
 toolicon_16_8.png toolicon_32_8.png toolicon_48_8.png toolicon_256_32.png toolicon4.ico

   identify toolicon4.ico
