typedef struct {
    int   n;
    float o,d;
} axis;

void iaxis(sf_file FF, axis *AA, const int i) {
    char BB[3];
    int     n;
    float o,d;

    (void) snprintf(BB,3,"n%d",i);
    if( !sf_histint  (FF,BB,&n)) n=1;
    AA->n=n;

    (void) snprintf(BB,3,"o%d",i);
    if(! sf_histfloat(FF,BB,&o)) o=0;
    AA->o=o;

    (void) snprintf(BB,3,"d%d",i);
    if(! sf_histfloat(FF,BB,&d)) d=1;
    AA->d=d;
}

void oaxis(sf_file FF, axis *AA, const int i) {
    char BB[3];

    (void) snprintf(BB,3,"n%d",i);
    sf_putint(FF,BB,AA->n);
    
    (void) snprintf(BB,3,"o%d",i);
    sf_putfloat(FF,BB,AA->o);

    (void) snprintf(BB,3,"d%d",i);
    sf_putfloat(FF,BB,AA->d);
}

void raxis(axis a) {
    
    fprintf(stderr,"n=%d o=%f d=%f \n",a.n,a.o,a.d);

}
