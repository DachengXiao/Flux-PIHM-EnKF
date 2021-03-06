#include "pihm.h"
#include "enkf.h"

void DisOper (obs_struct *obs, pihm_struct pihm)
{
    double          dist;
    double          dist_min = 999999999.9;
    int             ind_min;
    int             i;

    obs->var_ind = (int *) malloc (sizeof (int));
    obs->var_ind[0] = 10;

    obs->dim = pihm->numriv;

    obs->weight = (double *) malloc (obs->dim * sizeof (double));

    obs->nlyr = 1;
    obs->k = (double **)malloc (obs->dim * sizeof (double *));
    obs->b = (double **)malloc (obs->dim * sizeof (double *));
    for (i = 0; i < obs->dim; i++)
    {
        obs->k[i] = (double *)malloc (sizeof (double));
        obs->b[i] = (double *)malloc (sizeof (double));
    }

    for (i = 0; i < pihm->numriv; i++)
    {
        dist = sqrt ((pihm->riv[i].topo.x - obs->x) * (pihm->riv[i].topo.x - obs->x) +
            (pihm->riv[i].topo.y - obs->y) * (pihm->riv[i].topo.y - obs->y));
        if (dist < dist_min)
        {
            ind_min = i;
            dist_min = dist;
        }
    }

    for (i = 0; i < pihm->numriv; i++)
    {
        obs->weight[i] = 0.0;
        obs->k[i][0] = 1.0;
        obs->b[i][0] = 0.0;
    }

    obs->weight[ind_min] = 24.0 * 3600.0;
}

void LandSfcTmpOper (obs_struct *obs, pihm_struct pihm)
{
    double          dist;
    int             i;
    double          total_area = 0.0;

    obs->var_ind = (int *)malloc (sizeof (int));
    obs->var_ind[0] = 24;

    obs->dim = pihm->numele;

    obs->weight = (double *)malloc (obs->dim * sizeof (double));

    obs->nlyr = 1;
    obs->k = (double **)malloc (obs->dim * sizeof (double));
    obs->b = (double **)malloc (obs->dim * sizeof (double));
    for (i = 0; i < obs->dim; i++)
    {
        obs->k[i] = (double *)malloc (sizeof (double));
        obs->b[i] = (double *)malloc (sizeof (double));
    }

    for (i = 0; i < pihm->numele; i++)
    {
        dist = sqrt ((pihm->elem[i].topo.x - obs->x) * (pihm->elem[i].topo.x - obs->x) +
            (pihm->elem[i].topo.y - obs->y) * (pihm->elem[i].topo.y - obs->y));

        obs->k[i][0] = 1.0;
        obs->b[i][0] = 0.0;

        if (dist < obs->rad)
        {
            obs->weight[i] = pihm->elem[i].topo.area;
            total_area += pihm->elem[i].topo.area;
        }
        else
        {
            obs->weight[i] = 0.0;
        }
    }

    for (i = 0; i < pihm->numele; i++)
    {
        obs->weight[i] /= total_area;
    }
}

void COSMOSOper (obs_struct *obs, pihm_struct pihm)
{
    double          dist;
    int             i;
    double          total_area = 0.0;

    obs->var_ind = (int *)malloc (sizeof (int));
    obs->var_ind[0] = 37;

    obs->dim = pihm->numele;

    obs->weight = (double *)malloc (obs->dim * sizeof (double));

    obs->nlyr = 1;
    obs->k = (double **)malloc (obs->dim * sizeof (double));
    obs->b = (double **)malloc (obs->dim * sizeof (double));
    for (i = 0; i < obs->dim; i++)
    {
        obs->k[i] = (double *)malloc (sizeof (double));
        obs->b[i] = (double *)malloc (sizeof (double));
    }

    for (i = 0; i < pihm->numele; i++)
    {
        dist = sqrt ((pihm->elem[i].topo.x - obs->x) * (pihm->elem[i].topo.x - obs->x) +
            (pihm->elem[i].topo.y - obs->y) * (pihm->elem[i].topo.y - obs->y));

        obs->k[i][0] = 1.0;
        obs->b[i][0] = 0.0;

        if (dist < obs->rad)
        {
            obs->weight[i] = pihm->elem[i].topo.area;
            total_area += pihm->elem[i].topo.area;
        }
        else
        {
            obs->weight[i] = 0.0;
        }
    }

    for (i = 0; i < pihm->numele; i++)
    {
        obs->weight[i] /= total_area;
    }
}


void SPVF10Oper (obs_struct *obs, var_struct *var, pihm_struct pihm)
{
    double          dist;
    double          dist_min = 999999999.9;
    int             ind_min;
    int             i;

    obs->var_ind = (int *) malloc (sizeof (int));
    obs->var_ind[0] = 48;

    obs->dim = pihm->numele;

    obs->weight = (double *)malloc (obs->dim * sizeof (double));

    obs->nlyr = 1;
    obs->k = (double **)malloc (obs->dim * sizeof (double));
    obs->b = (double **)malloc (obs->dim * sizeof (double));
    for (i = 0; i < obs->dim; i++)
    {
        obs->k[i] = (double *)malloc (sizeof (double));
        obs->b[i] = (double *)malloc (sizeof (double));
    }

    for (i = 0; i < pihm->numele; i++)
    {
        dist =
            sqrt ((pihm->elem[i].topo.x - obs->x) * (pihm->elem[i].topo.x -
                obs->x) + (pihm->elem[i].topo.y -
                obs->y) * (pihm->elem[i].topo.y - obs->y));
        if (dist < dist_min)
        {
            ind_min = i;
            dist_min = dist;
        }
    }

    for (i = 0; i < pihm->numele; i++)
    {
        obs->weight[i] = 0.0;
        obs->k[i][0] = 1.0;
        obs->b[i][0] = 0.0;
    }    

    obs->weight[ind_min] = 1.0;
}

void UNSATOper (obs_struct *obs, var_struct *var, pihm_struct pihm)
{
    double          dist;
    double          dist_min = 999999999.9;
    int             ind_min;
    int             i;

    obs->var_ind = (int *) malloc (sizeof (int));
    obs->var_ind[0] = 2;

    obs->dim = pihm->numele;

    obs->weight = (double *)malloc (obs->dim * sizeof (double));

    obs->nlyr = 1;
    obs->k = (double **)malloc (obs->dim * sizeof (double));
    obs->b = (double **)malloc (obs->dim * sizeof (double));
    for (i = 0; i < obs->dim; i++)
    {
        obs->k[i] = (double *)malloc (sizeof (double));
        obs->b[i] = (double *)malloc (sizeof (double));
    }

    for (i = 0; i < pihm->numele; i++)
    {
        dist =
            sqrt ((pihm->elem[i].topo.x - obs->x) * (pihm->elem[i].topo.x -
                obs->x) + (pihm->elem[i].topo.y -
                obs->y) * (pihm->elem[i].topo.y - obs->y));
        if (dist < dist_min)
        {
            ind_min = i;
            dist_min = dist;
        }
    }

    for (i = 0; i < pihm->numele; i++)
    {
        obs->weight[i] = 0.0;
        obs->k[i][0] = 1.0;
        obs->b[i][0] = 0.0;
    }    

    obs->weight[ind_min] = 1.0;
}

