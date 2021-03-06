
/*****************************************************************************
 * File       : pihm.h
 * Function   : Declaration and Definition of global variables and data
 *              structure
 ****************************************************************************/

#ifndef PIHM_HEADER
#define PIHM_HEADER

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdarg.h>

/* SUNDIAL Header Files */
#include "sundials_types.h"     /* realtype, integertype, booleantype
                                 * defination */
#include "cvode.h"              /* CVODE header file */
#include "cvode_spgmr.h"        /* CVSPGMR linear header file */
#include "sundials_smalldense.h"        /* use generic DENSE linear solver
                                         * for "small" */
#include "nvector_serial.h"     /* contains the definition of type
                                 * N_Vector */
#include "sundials_math.h"      /* contains UnitRoundoff, RSqrt,
                                 * SQR functions  */
#include "cvode_dense.h"        /* CVDENSE header file */

#include "pihm_const.h"
#include "pihm_input_struct.h"
#include "pihm_struct.h"
#include "pihm_func.h"

#endif
