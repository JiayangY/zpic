/*
 *  particles.h
 *  zpic
 *
 *  Created by Ricardo Fonseca on 11/8/10.
 *  Copyright 2010 Centro de Física dos Plasmas. All rights reserved.
 *
 */

#ifndef __PARTICLES__
#define __PARTICLES__

#include "zpic.h"
#include "field.h"
#include "charge.h"

#define MAX_SPNAME_LEN 32

typedef struct {
	int ix;
	float x;
	float vx;
} t_part;

enum density_type {UNIFORM, STEP, SLAB, RAMP, CUSTOM};

typedef struct {

	float n;				// reference density (defaults to 1.0, multiplies density profile)

	enum density_type type;	// Density profile type
	float start, end;		// Position of the plasma start/end, in simulation units
	
	float ramp[2];     // Initial and final density of the ramp

	float (*custom)(float, void*); // Pointer to custom density function
	void *custom_data;

	unsigned long total_np_inj;	// Total number of particles already injected
	double custom_q_inj;		// Total charge injected (density integral) in custom profile

} t_density;


typedef struct {
	
	char name[MAX_SPNAME_LEN];
	
	// Particle data buffer
	t_part *part;
	int np;
	int np_max;

	// mass over charge ratio
	float m_q;
	
	// charge of individual particle
	float q;

	// total kinetic energy
	double energy;
	
	// Number of particles per cell
	int ppc;

	// Density profile to inject
	t_density density;

	// Initial momentum of particles
	float vfl;
	float vth;

	// Simulation box info
	int nx;
	float dx;
	float box;

	// Time step
	float dt;

	// Iteration number
	int iter;
	
} t_species;

void spec_new( t_species* spec, char name[], const float m_q, const int ppc, 
			  const float * vfl, const float * vth,
			  const int nx, float box, const float dt, t_density* density );

void spec_delete( t_species* spec );

void spec_advance( t_species* spec, t_field* emf, t_charge* charge );

double spec_time( void );

/*********************************************************************************************
 
 Diagnostics
 
 *********************************************************************************************/

#define CHARGE 		0x1000
#define PHA    		0x2000
#define PARTICLES   0x3000
#define X1     		0x0001
#define V1     		0x0004

#define PHASESPACE(a,b) ((a) + (b)*16 + PHA)

void spec_deposit_pha( const t_species *spec, const int rep_type,
			  const int pha_nx[], const float pha_range[][2], float* buf );

void spec_deposit_charge( const t_species* spec, float* charge );

void spec_report( const t_species *spec, const int rep_type, 
				  const int pha_nx[], const float pha_range[][2] );


#endif
