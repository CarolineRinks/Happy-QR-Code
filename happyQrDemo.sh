#!/bin/bash

echo "Caroline Rinks" | ./QRcode-generator/QR_gen | convert - my.ppm
echo "235 42 201 8 124 211 18 225 238 done eps_images/corgi.eps yes" | ./HappyQR my.ppm my.jgr
echo "

Drawing jgraph..Please wait"
./jgraph/jgraph -P my.jgr | ps2pdf - | convert -density 300 - -quality 100 my.jpg

echo "http://web.eecs.utk.edu/~jplank/plank/classes/cs494/494/index.html" | ./QRcode-generator/QR_gen | convert - website.ppm
echo "120 34 201 0 186 107 39 100 240 150 120 32 176 139 29 done eps_images/laughing.eps no" | ./HappyQR website.ppm website.jgr
echo "

Drawing jgraph..Please wait"
./jgraph/jgraph -P website.jgr | ps2pdf - | convert -density 300 - -quality 100 website.jpg

echo "Alice had to go to the store to buy eggs, 3 green hams, and many many cartons of milk. But much to her chagrin, when she got to the store, the milk was nowhere to be found. Distraught, Alice forgot why she was at the store and wandered aimlessly down the aisles. She stopped at the freezer section and picked out a large tub of ice cream. By the time she made it to the checkout area, she had already eaten hal of the tub. The cashier looked at Alice very strangely, but Alice didn't mind, for who was Alice if she didn't have a carton of milk. Oh Alice." | ./QRcode-generator/QR_gen | convert - long.ppm
echo "255 0 0 200 51 0 230 80 0 238 0 85 238 0 135 done eps_images/pink_lotus.eps no" | ./HappyQR long.ppm long.jgr 
echo "

Drawing jgraph..Please wait"
./jgraph/jgraph -P long.jgr | ps2pdf - | convert -density 300 - -quality 100 long.jpg
