// cpg_spec.c	: Set up for PGPLOT
//
// Author : Seiji Kameno
// Created: 2012/12/19
#include "shm_k5data.inc"
#include <cpgplot.h>
#include <math.h>

int	cpg_spec(
	struct SHM_PARAM	*param_ptr,
	float	*freq_ptr,			// Pointer to Frequency
	float	*xspec_ptr)			// Pointer to Spectral Data
{
	float	xmin, xmax;				// Plot Window Range
	float	ymin, ymax;				// Plot Window Range
	float	plot_y[NFFT2];			// Y values to plot
	int		index;					// channel index
	int		st_index;				// Index for Sub-Stream
	int		nxwin, nywin;			// Number of Panels in X and Y
	int		nx_index, ny_index;		// Index for Panels
	int		err_code;				// Error Code
	double	freq_incr;				// Frequency Increment
	float	xwin_incr,	ywin_incr;	// Position Increment of Panels
	float	x_text, y_text;			// Text Drawing Position
	char	text[256];				// Text to Write

	cpgsch(0.5);
	freq_incr = (double)(param_ptr->fsample) / 2 / NFFT2;
	nxwin   = (int)sqrt((double)param_ptr->num_st);
	nywin   = (param_ptr->num_st + nxwin - 1)/nxwin;
	xwin_incr = 0.9 / (float)nxwin;
	ywin_incr = 0.9 / (float)nywin;
	for(st_index=0; st_index<param_ptr->num_st; st_index++){

		nx_index	= st_index % nxwin;
		ny_index	= st_index / nxwin;

		/*-------- PLOT WINDOW --------*/
		xmin = - 0.5*freq_incr;	xmax = xmin + ((double)NFFT2 - 0.5) * freq_incr;
		ymin = -10.0;			ymax = 50.0;
		for(index=0; index<NFFT2; index++){
			plot_y[index] = 10.0* log10(xspec_ptr[st_index* NFFT2 + index]);	// autocorr. in dB
		}
		cpgsvp(	0.067+xwin_incr*nx_index, 0.067+xwin_incr*(nx_index+0.9),
				0.067+ywin_incr*ny_index, 0.067+ywin_incr*(ny_index+0.9));
		cpgswin(xmin, xmax, ymin, ymax);

		cpgsci(2);	cpgrect(xmin, xmax, ymin, ymax);
		cpgsci(0);	cpgbox("G", 0.0, 0, "G", 10.0, 0);
		cpgsci(1);	cpgbox(	"BCNTS", 0.0, 0, "BCNTS", 10.0, 10);
		cpgsci(3);	cpgline( NFFT2, freq_ptr, plot_y );

		//-------- IF number
		x_text = xmin*0.2 + xmax*0.8; y_text = ymin*0.1 + ymax*0.9;
		sprintf(text, "IF = %d", st_index); cpgsci(3);	cpgtext( x_text, y_text, text );
	}
	//-------- UTC
	x_text = xmin*0.3 + xmax*0.7; y_text = 0.05*ymin + 0.95* ymax;
	sprintf(text, "%04d %03d %02d:%02d:%02d\0", param_ptr->year, param_ptr->doy, param_ptr->hour, param_ptr->min, param_ptr->sec); cpgsci(1);	cpgtext( x_text, y_text, text );

	cpgebuf();

	return(0);
}
