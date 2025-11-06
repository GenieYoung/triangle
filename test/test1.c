#define REAL double

#include <stdio.h>
#include <stdlib.h>
#include "triangle.h"

// cc -O -o ./test1 ./test1.c ../triangle/triangle.o -I../triangle -lm

void export_pslg(struct triangulateio* in, const char* filename);
void export_mesh(struct triangulateio* out, const char* filename);

int main()
{
  struct triangulateio in, out;

  in.numberofpoints = 42;
  in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
  in.pointlist[0] = 100.0;  in.pointlist[1] = 0.0;
  in.pointlist[2] = 0.0;    in.pointlist[3] = 100.0;
  in.pointlist[4] = -100.0; in.pointlist[5] = 0.0;
  in.pointlist[6] = 0.0;    in.pointlist[7] = -100.0;
  for(int y = -90, i = 0; y < 100; y+=10, ++i) {
    in.pointlist[8 + i*4] = abs(y) - 100 + 5; 
    in.pointlist[8 + i*4 + 1] = y; 
    in.pointlist[8 + i*4 + 2] = 100 - abs(y) - 5;
    in.pointlist[8 + i*4 + 3] = y;
  }
  
  in.pointmarkerlist = (int *) NULL;
  in.numberofpointattributes = 0;

  in.numberofsegments = 23;
  in.segmentlist = (int *) malloc(in.numberofsegments * 2 * sizeof(int));
  in.segmentlist[0] = 0; in.segmentlist[1] = 1;
  in.segmentlist[2] = 1; in.segmentlist[3] = 2;
  in.segmentlist[4] = 2; in.segmentlist[5] = 3;
  in.segmentlist[6] = 3; in.segmentlist[7] = 0;
  for(int i = 0; i < 19; ++i) {
    in.segmentlist[8 + i*2] = 2*i + 4;
    in.segmentlist[8 + i*2 + 1] = 2*i + 5;
  }

  in.segmentmarkerlist = (int *) NULL;

  in.numberofregions = 1;
  in.regionlist = (REAL *) malloc(in.numberofregions * 2 * sizeof(REAL));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 0.5;

  in.numberofholes = 0;

  export_pslg(&in, "test1_input.obj");

  out.pointlist = (REAL *) NULL;            
  out.pointmarkerlist = (int *) NULL;       
  out.pointattributelist = (REAL *) NULL;
  out.segmentlist = (int *) NULL;
  out.segmentmarkerlist = (int *) NULL;
  out.trianglelist = (int *) NULL;   

  triangulate("pzD", &in, &out, (struct triangulateio *) NULL);
  export_mesh(&out, "test1_output.obj");

  /* Free all allocated arrays, including those allocated by Triangle. */

  free(in.pointlist);
  free(in.segmentlist);
  free(in.regionlist);
  free(out.pointlist);
  free(out.segmentlist);
  free(out.trianglelist);

  return 0;
}

void export_pslg(struct triangulateio* in, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if(!fp) {
        printf("Failed to open pslg.obj for writing\n");
        return;
    }
    for(int i = 0; i < in->numberofpoints; ++i) {
        fprintf(fp, "v %.6f %.6f 0.0\n", in->pointlist[i*2], in->pointlist[i*2 + 1]);
    }
    for(int i = 0; i < in->numberofsegments; ++i) {
        fprintf(fp, "l %d %d\n", 
        in->segmentlist[i * 2] + 1,
        in->segmentlist[i * 2 + 1] + 1);
    }
    fclose(fp);
}

void export_mesh(struct triangulateio* out, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if(!fp) {
        printf("Failed to open output.obj for writing\n");
        return;
    }
    for(int i = 0; i < out->numberofpoints; ++i) {
        fprintf(fp, "v %.6f %.6f 0.0\n", out->pointlist[i*2], out->pointlist[i*2 + 1]);
    }
    for(int i = 0; i < out->numberoftriangles; ++i) {
        fprintf(fp, "f %d %d %d\n", 
        out->trianglelist[i * out->numberofcorners] + 1,
        out->trianglelist[i * out->numberofcorners + 1] + 1,
        out->trianglelist[i * out->numberofcorners + 2] + 1);
    }
    fclose(fp);
}