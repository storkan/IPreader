#include <tcl8.6/tcl.h>
#include <tiffio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static int Readtiffdata(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]) {
//declaration
  Tcl_Obj *result_p;
  uint32 w, h;
  char *file;
  char output[30];
  size_t c_h;
  size_t c_w;
  int w1, w2, h1, h2, t1, type;
  size_t npixels;
  double sumsq = 0.0;
  double sum = 0.0, mean = 0.0, stdev = 0.0;
  double *sum_p = &sum;
  Tcl_DString data;
  Tcl_DString *data_p = &data;
  Tcl_DStringInit(data_p);
  uint32* raster;
  TIFF* tif;
// check provided parameters
  if (7 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "filename w1 w2 h1 h2 type");
    return TCL_ERROR;
  }
  file = Tcl_GetString(objv[1]);
  if (TCL_OK != Tcl_GetIntFromObj(interp, objv[2], &w1)) {
    return TCL_ERROR;
  }
  if (TCL_OK != Tcl_GetIntFromObj(interp, objv[3], &w2)) {
    return TCL_ERROR;
  }
  if (TCL_OK != Tcl_GetIntFromObj(interp, objv[4], &h1)) {
    return TCL_ERROR;
  }
  if (TCL_OK != Tcl_GetIntFromObj(interp, objv[5], &h2)) {
    return TCL_ERROR;
  }
  if (TCL_OK != Tcl_GetIntFromObj(interp, objv[6], &type)) {
    return TCL_ERROR;
  }
  if (type == 0 || type == 1) {
  } else {
    Tcl_WrongNumArgs (interp, 1, objv, "Readtiffdata was called with unknown type. Only type = 0 (gel) or 1 (tif) are known.");
    return TCL_ERROR;
  }
  if (h1 < h2) {
  } else {
    if (h1 > h2) {
      t1 = h1;
      h1 = h2;
      h2 = t1;
    } else {
      Tcl_WrongNumArgs (interp, 1, objv, "filename w1 w2 h1 h2\n h1 must not be equal to h2");
      return TCL_ERROR;
    }
  }
  if (w1 < w2) {
  } else {
    if (w1 > w2) {
      t1 = w1;
      w1 = w2;
      w2 = t1;
    } else {
      Tcl_WrongNumArgs (interp, 1, objv, "filename w1 w2 h1 h2\n w1 must not be equal to w2");
      return TCL_ERROR;
    }
  }
// tif = TIFFOpen(*filename_p, "r");
  tif = TIFFOpen(file, "r");
// start program
  if (tif) {
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    npixels = w * h;
// printf("w1: %d, w2: %d\n",w1,w2);
    raster = (uint32*) _TIFFmalloc(npixels * sizeof (uint32));
    if (raster != NULL) {
      if (TIFFReadRGBAImageOriented(tif, w, h, raster,ORIENTATION_TOPLEFT, 0)) {
        if (type == 0) {
// read gel file: gel files from GE-Typhoon scanners use a sqrt compression to encode bigger numbers
// routine returns normalized values
          for(c_w=w1;c_w<w2;c_w++) {
            sum = 0.0;
            for(c_h=h1;c_h<h2;c_h++) {
              sum = sum + powf(1.0-(double)(raster[(c_h*w)+c_w]%65536)/65535.0,2);
            }
            mean = sum/(h2-h1);
            sprintf(output,"%f ",mean);
            Tcl_DStringAppend(data_p, output, -1);
// calc mean if h2-h1 > 5; else return -1
            if ((h2-h1) > 4) {
              sumsq = 0.0;
              for(c_h=h1;c_h<h2;c_h++) {
                sumsq = sumsq + powf(mean-powf(1.0-(double)(raster[(c_h*w)+c_w]%65536)/65535.0,2),2);
              }
              stdev = sqrt(sumsq/(double)(h2-h1));
            } else {
              stdev = 0.0 ;
            }
            sprintf(output,"%f ",stdev);
            Tcl_DStringAppend(data_p, output, -1);
          }
        } else if (type == 1) {
// read tif file: tif files use a linear scale internally
          for(c_w=w1;c_w<w2;c_w++) {
            sum = 0.0;
            for(c_h=h1;c_h<h2;c_h++) {
//          printf("%i\n",raster[(c_h*w)+c_w]);
              sum = sum + 1.0-(double)(raster[(c_h*w)+c_w]%65536)/65535.0;
            }
            mean = sum/(h2-h1);
            sprintf(output,"%f ",mean);
            Tcl_DStringAppend(data_p, output, -1);
// calc mean if h2-h1 > 5; else return -1
            if ((h2-h1) > 4) {
              sumsq = 0.0;
              for(c_h=h1;c_h<h2;c_h++) {
                sumsq = sumsq + powf(mean-(1.0-(double)(raster[(c_h*w)+c_w]%65536)/65535.0),2);
              }
              stdev = sqrt(sumsq/(double)(h2-h1));
            } else {
              stdev = 0.0 ;
            }
            sprintf(output,"%f ",stdev);
            Tcl_DStringAppend(data_p, output, -1);
          }
        }
      }
      _TIFFfree(raster);
    }
    TIFFClose(tif);
  }
  Tcl_DStringResult(interp,data_p);
  Tcl_DStringFree(data_p);
  return TCL_OK;
}

int DLLEXPORT Tiffread_Init(Tcl_Interp *interp) {
  if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
    return TCL_ERROR;
  }
  if (Tcl_PkgProvide(interp, "readtiff", "1.0") == TCL_ERROR) {
    return TCL_ERROR;
  }
  Tcl_CreateObjCommand(interp, "readtiff", Readtiffdata, (ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);
  return TCL_OK;
}
