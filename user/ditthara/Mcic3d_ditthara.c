/* Conventional IC 3D */

/*
  Copyright (C) 2013 Colorado School of Mines
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* 
 * inputs: wavefields organized as z-y-x-t
 * output:      image organized as z-y-x
 */

#include <rsf.h>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char* argv[])
{
    bool verb,isreversed;

    sf_file Fs,Fr,Fi;    /* I/O files */
    sf_axis az,ay,ax,at,aa; /* cube axes */
    int     iz,iy,ix,it;
    int     nz,ny,nx,nt;
    long iseek;

    float ***us=NULL,***ur=NULL,***ii=NULL;

    float scale;

    /*------------------------------------------------------------*/
    /* init RSF */
    sf_init(argc,argv);

    /* OMP parameters */
#ifdef _OPENMP
    omp_init();
#endif

    if(! sf_getbool("verb",&verb)) verb=false; /* verbosity flag */
    if(! sf_getbool("isreversed",&isreversed)) isreversed=false; /* received wavefield */

    Fs = sf_input ("in" );
    Fr = sf_input ("ur" );
    Fi = sf_output("out");

    /*------------------------------------------------------------*/
    /* read axes */
    az=sf_iaxa(Fs,1); nz = sf_n(az);
    ay=sf_iaxa(Fs,2); ny = sf_n(ay);
    ax=sf_iaxa(Fs,3); nx = sf_n(ax);
    at=sf_iaxa(Fs,4); nt = sf_n(at);

    aa=sf_maxa(1,0,1);
    sf_setlabel(aa,"");
    sf_setunit (aa,"");

    if(verb) {
        sf_raxa(az);
        sf_raxa(ay);
        sf_raxa(ax);
        sf_raxa(at);
    }

    /* write axes */
    sf_oaxa(Fi,az,1);
    sf_oaxa(Fi,ay,2);
    sf_oaxa(Fi,ax,3);
    sf_oaxa(Fi,aa,4);

    /*------------------------------------------------------------*/
    /* allocate work arrays */
    ii = sf_floatalloc3(nz,ny,nx);
    us = sf_floatalloc3(nz,ny,nx);
    ur = sf_floatalloc3(nz,ny,nx);
    for (ix=0; ix<nx; ix++) {
      for (iy=0; iy<ny; iy++) {
        for(iz=0; iz<nz; iz++) {
            ii[ix][iy][iz]=0.;
        }
      }
    }

    /*------------------------------------------------------------*/
    if(isreversed) { /* receiver wavefield is reversed */

        if(verb) fprintf(stderr,"nt\n");
        for (it=0; it<nt; it++) {
            if(verb) fprintf(stderr,"\b\b\b\b\b\b\b\b\b\b%04d",it);

            sf_floatread(us[0][0],nz*ny*nx,Fs);
            sf_floatread(ur[0][0],nz*ny*nx,Fr);

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)                              \
    private(ix,iy,iz)                                                      \
    shared (ii,us,ur,nx,ny,nz)
#endif


            for  (ix=0; ix<nx; ix++) {
              for (iy=0; iy<ny; iy++) {
                for(iz=0; iz<nz; iz++) {
                    ii[ix][iy][iz] += us[ix][iy][iz]*ur[ix][iy][iz];
                }
            }

        } /* it */
        if(verb) fprintf(stderr,"\n");

    }} else { /* receiver wavefield is NOT reversed */

        if(verb) fprintf(stderr,"nt\n");
        for (it=0; it<nt; it++) {
            if(verb) fprintf(stderr,"\b\b\b\b\b%d",(nt-it-1));

            sf_floatread(us[0][0],nz*ny*nx,Fs);
            iseek=(off_t)(nt-1-it)*nz*ny*nx*sizeof(float);
            sf_seek(Fr,iseek,SEEK_SET);
            sf_floatread(ur[0][0],nz*nx*ny,Fr);

#ifdef _OPENMP
#pragma omp parallel for schedule(dynamic)                              \
    private(ix,iy,iz)                                                      \
    shared (ii,us,ur,nx,ny,nz)
#endif
 
         for (ix=0; ix<nx; ix++) {
            for (iy=0; iy<ny; iy++) {
                for(iz=0; iz<nz; iz++) {
                    ii[ix][iy][iz] += us[ix][iy][iz]*ur[ix][iy][iz];
                }
            }
          }
        } /* it */
        if(verb) fprintf(stderr,"\n");

    } /* end "is reversed" */
    /*------------------------------------------------------------*/

    /*------------------------------------------------------------*/
    /* scale image */
    scale = 1./nt;
    for(ix=0; ix<nx; ix++) {
      for(iy=0; iy<ny; iy++) {
        for(iz=0; iz<nz; iz++) {
          ii[ix][iy][iz] *=scale;
        }
      }
    }

    /*------------------------------------------------------------*/
    /* write image */
    sf_floatwrite(ii[0][0],nx*ny*nz,Fi);

    /*------------------------------------------------------------*/
    /* deallocate arrays */

    free(**ii); free(*ii); free(ii);
    free(**us); free(*us); free(us);
    free(**ur); free(*ur); free(ur);
    /*------------------------------------------------------------*/

    exit(0);
}
                               

