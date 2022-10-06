# Happy-QR-Code
HappyQR is a program that reads a QR code given as a PPM image and rewrites it using jgraph, adding color, images, and patterns to the pixels.

Setup the repository
--------------------
1. Clone this repository
2. Run setup script with the command "sh setup.sh"

Using HappyQR
-------------
You can run the script happyQrDemo.sh to automatically generate 3 Happy QR codes using preselected options:
      
      sh happyQrDemo.sh

To customize a QR code yourself:

Generate a PPM image QR code. Replace "text for QR Code" with whatever text you want encoded in the QR code:
      
      echo "text for QR Code" | ./QRcode-generator/QR_gen | convert - QR.ppm

Run HappyQR, providing the PPM QR code and a name for the jgraph file that will be generated:

      ./HappyQR QR.ppm QR.jgr
   
Follow the instructions to customize your QR code.
When the program is done, run jgraph with the created jgraph file. The following command also converts the final QR code to a jpg:

      ./jgraph/jgraph -P QR.jgr | ps2pdf - | convert -density 300 - -quality 100 QR.jpg

Tips
----
- The program has a random element to it, so even if you input the same customization options, you will get a different QR code.
- Avoid inputting really light colors to make sure that your QR code is readable. The best colors to input are those that clearly contrast with the white background.
- Some example eps images are included to use with HappyQR. You can use https://cloudconvert.com/jpg-to-eps to convert any jpg image to eps. Make sure that your image is square.


The QR code generator program was pulled from the project https://github.com/nayuki/QR-Code-generator. Check it out!
