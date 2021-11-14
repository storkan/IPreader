#include <tcl8.5/tcl.h>
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
  int w1, w2, h1, h2, t1;
  size_t npixels;
  double sum = 0.0;
  double *sum_p = &sum;
  Tcl_DString data;
  Tcl_DString *data_p = &data;
  Tcl_DStringInit(data_p);
  uint32* raster;
  TIFF* tif;
// check provided parameters
  if (6 != objc) {
    Tcl_WrongNumArgs (interp, 1, objv, "filename w1 w2 h1 h2");
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
        for(c_w=w1;c_w<w2;c_w++) {
          sum = 0.0;
          for(c_h=h1;c_h<h2;c_h++) {
            sum = sum + powf(1.0-(raster[(c_h*w)+c_w]%65536)/65535.0,2);
          }
          sprintf(output,"%f ",sum/(h2-h1));
          Tcl_DStringAppend(data_p, output, -1);
//          printf("%f\n",sum/(h2-h1));
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
  if (Tcl_PkgProvide(interp, "Hell", "1.0") == TCL_ERROR) {
    return TCL_ERROR;
  }
  Tcl_CreateObjCommand(interp, "readtiff", Readtiffdata, (ClientData)NULL, (Tcl_CmdDeleteProc*)NULL);
  return TCL_OK;
}
