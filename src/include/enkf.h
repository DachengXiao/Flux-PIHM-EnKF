#include "mpi.h"

#define MAXPARAM        100
#define MAXVAR          100
#define MAXINT          2147483647
#define CORRMAX         0.25

#define SUCCESS_TAG     2
#define CYCLE_TAG          1
#define PARAM_TAG         3

enum prmt_type {KSATH, KSATV, KINF, KMACH, KMACV, DINF, RZD, DMAC, POROSITY,
    ALPHA, BETA, AREAFV, AREAFH, VEGFRAC, ALBEDO, ROUGH, PRCP, SFCTMP,
    ET0, ET1, ET2, RIVROUGH, RIVKSATH, RIVKSATV, RIVBEDTHICK, RIVDEPTH,
    RIVSHPCOEFF, DRIP, INTCP, RSMIN, CZIL, FXEXP, CFACTR, RGL, HS, THETAREF,
    THETAW};

enum obs_type {RUNOFF_OBS, TSKIN_OBS, COSMOS_OBS, SPVF10_OBS, UNSAT_OBS};
typedef struct var_struct
{
    char            name[MAXSTRING];
    int             dim;
    double          min;
    double          max;
} var_struct;

typedef struct param_struct
{
    int             perturb;
    int             update;
    double          perturb_min;
    double          perturb_max;
    double          init_std;
    double          min;
    double          max;
    int             type;
    char            name[MAXSTRING];
} param_struct;

typedef struct obs_struct
{
    char            name[MAXSTRING];
    char            fn[MAXSTRING];
    int             type;
    double          x;
    double          y;
    double          rad;
    double          depth;
    int             dim;
    int             nlyr;
    int            *var_ind;
    /* The linear observation operator vector has a general form of
     *
     * xf = SUM_j { SUM_i  [w_i * (k_ij x_ij + b_ij)]}
     *
     * w_i, k_j, and b_j are initialized at the beginning and stored here for
     * easy access so they don't need to be calculated at every EnKF step.
     * w_i could be the area of each model grid
     * k_j is ususally 1.0, or could be the thickness of each soil layer
     * b_j could be the soil column depth if xf is water table depth. */
    double         *weight;
    double        **k;
    double        **b;
} obs_struct;

typedef struct ens_mbr_struct
{
    double          param[MAXPARAM];
    double         *var[MAXVAR];
} ens_mbr_struct;

typedef struct enkf_struct
{
    int             ne;
    int             nobs;
    int             interval;
    int             start_mode;
    int             end_time;
    int             cycle_start_time;
    int             cycle_end_time;
    int             mbr_start_mode;
    int             ascii;
    int             numele;
    int             numriv;
    double          weight;
    ens_mbr_struct *member;
    int             update_var;
    int             update_param;

    //double         *total_water_volume;

    param_struct    param[MAXPARAM];
    var_struct      var[MAXVAR];
    obs_struct     *obs;
} *enkf_struct;

void EnKFRead(char *project, enkf_struct ens);
double Randn();
void MapVar (var_struct *var, int numele, int numriv);
void Perturb(char *project, enkf_struct ens, char *outputdir);
void MapVar (var_struct *var, int numele, int numriv);
void Calib2Mbr (calib_struct cal, double *param);
void Mbr2Cal (calib_struct *cal, const double *param);
void WriteParamOutput (int rawtime, enkf_struct ens, int ind, char *outputdir);
void WriteCalFile (enkf_struct ens, char *project);
void JobHandout (int starttime, int endtime, int startmode, ens_mbr_struct *member, double *param, int ne, int total_jobs);
void JobRecv (int *starttime, int *endtime, int *startmode, double *param, int ne);
void PrintEnKFStatus (int starttime, int endtime);
void JobHandIn (int total_jobs);
void WritePara (char *project, int start_mode, int start_time, int end_time);

void EnKFCore (double *xa, double obs, double obs_err, double *xf, int ne);
void EnKF (char *project, enkf_struct ens, int obs_time, char *outputdir);
void ReadObs (int obs_time, char *fn, double *obs, double *obs_error);
void InitOper (char *project, enkf_struct ens);
void DisOper (obs_struct *obs, pihm_struct pihm);
void ReadFcst (enkf_struct enkf, obs_struct obs, double *xf);
void ReadVar (char *project, char *outputdir, enkf_struct ens, int obs_time);
void UpdAnlys (enkf_struct ens, double obs, double obs_error, double *xf);
void CovInflt (enkf_struct ens, enkf_struct ens0);
void WriteEnKFOut (char *project, enkf_struct ens, char *outputdir, int t);
void GenRandNum (int ne, int nparam, double **randnum, double lower, double upper);
double Randn();
void LandSfcTmpOper (obs_struct *obs, pihm_struct pihm);
void COSMOSOper (obs_struct *obs, pihm_struct pihm);
void FreeEns (enkf_struct ens);

