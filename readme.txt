Author and Copyright owner: Jörn Schmedt auf der Günne
Years 2013-2021
#----------------------------------------------
#Installation:
0.) tar xvzf ipreader-1.4.tar.gz
1.) cd ipreader-1.4
2.) sudo mkdir -p /usr/local/Library/IPreader
3.) sudo cp Makefile libtiffread-1.c ipreader-1.4.tcl /usr/local/Library/IPreader
4.) sudo cp utilities/gnuplot1Daverage.tcl /usr/local/bin/gnuplot1Daverage
4.) cd /usr/local/Library/IPreader
5.) make
6.) cd /usr/local/bin
7.) ln -s /usr/local/Library/IPreader/ipreader-1.4.tcl /usr/local/bin/ipreader
8.) sudo chmod +x /usr/local/bin/ipreader /usr/local/bin/gnuplot1Daverage

#dependencies (ubuntu-20.04)
apt-get install make gcc libtiff4-dev tcl-dev tk-dev tcllib libtk-img libtk-img-dev pngtools libtiff-tools blt

#----------------------------------------------
#Usage:
You need an image of a scanned powder X-ray pattern for example from a Huber Guinier camera
equipped with imaging plates and an external imaging plate scanner. The scanner produces a gel
or tif files which can be read by the software ipreader.
Usually you would have sample holder with 2 or more traces, the center one usually carrying 
the sample-under-investigation and one of the outer traces would be prepared with a reference 
compound, e.g. quartz, silicon, CaWO4, diamond or LaB6.
The software ipreader is intended to help with the conversion of the imagefile into an ASCII file.
Besides the conversion to files in ASCII or raw format it helps with setting the 2theta scale.
The 2theta scale is automatically calibrated upon telling the software which wavelength and
reference compound is used. 

A typical image conversion has the following steps.
1.) read the gel/tif file (-> file -> load image)
2.) draw a box around the region in the image which contains the sample diffractogram
3.) click on "take selection" (bottom)
4.) shift the box to the part the image which contains the reference diffractogram; you can change the height as well
5.) click on "take selection" (middle)
6.) choose correction radiation -> CuKalpha1, ...
7.) change sample (middle) to the correct compound (sample/CaWO4/SiO2/Si/....)
    At this stage the 2theta scale is calibrated. The blue lines show the the theoretical position 
    of the reflections of the reference compound. If these lines are superimposed with the experimental 
    reflections (middle), everything is fine and you can trust the 2theta scale.
    The calibration routine uses non-linear interpolation and linear regression which
    usually produces good results.
8.) Last you want to export your data: save diffractogram.
    You can change the type to raw or keep the default ASCII-XY type.
    
The software IPreader is designed of images being bitmap graphics with 16bit or higher (gel or tiff files).
If you need to cut or align your picture before importanting them you must use a software
which keeps the grayscale resolution. A good program for this purpose is "imagej". Many
programs silently convert 16bit grayscale pictures to 8bit which reduces image size
and y-scale information. Such pictures are a really bad choice for any real work.

In order to average several xy-files "LaB6-1.xy", "LaB6-2.xy" and "LaB6-3.xy" type the following.
gnuplot1Daverage LaB6-1.xy LaB6-2.xy LaB6-3.xy 

The directory examples contains an example "gel" file and a set of three xy files which
may serve as an example for the averaging script.

#license
The software is published under the license conditions of the GNU General Public License.
A copy of this license you have downloaded with this software package.

