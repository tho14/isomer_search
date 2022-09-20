#ifndef __CONSTANTS_H
#define __CONSTANTS_H

const int ncrates = 2;// TO 3 for e17011
const int nslots = 13;
const int nchannels = 16;
const int nadcs = 20; // total number of adcs in the system; 22 for e17011 
const int totchan = 208; // nslots*nchannels

const int ndssd = 40;
const int nsssd = 16;
const int nanodes = 256;
const int npspmt = 16;

const int nsega = 16;// TO e16032
const int nclovers = 16;// TO e17011
const int ncrystals = 4;// TO e17011
const int totcrystals = 64; // TO e17011 nclovers*ncrystals
const int nlabr3 = 15;

// TO UMass Lowell CLYC entries
//const int nclyc = 2;
//const int nclyc = 8;

// TLinearFitter
const int ERR_MINV = -100;   // Matrix for linear equation solver is not invertable
const int ERR_BADPOS = -99; // Position fit outside of anode pixel range [0,16)
const int ERR_CONCDOWN = -98; // Quadratic fit is concave down (no minimum)
const bool displaylinfitinfo = false; // Set to true to display linear fit information
const bool drawfits = false; // Set to true to draw anode position fits

#endif
