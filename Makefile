#-----------------------------------------------------------------
# MM-PIHM Makefile
# -----------------------------------------------------------------

CC = gcc
CFLAGS = -g -O0 -Wall

SUNDIALS_PATH = ./sundials

SRCDIR = ./src
LIBS =	-lm
INCLUDES = \
	-I${SUNDIALS_PATH}/include \
	-I${SUNDIALS_PATH}/include/cvode \
	-I${SUNDIALS_PATH}/include/sundials\
	-I${SRCDIR}/include

LFLAGS = -L${SUNDIALS_PATH}/lib -lsundials_cvode -lsundials_nvecserial

SRCS_ = pihm.c \
	read_alloc.c \
	read_func.c \
	initialize.c \
	hydrol_func.c \
	is_sm_et.c \
	hydrol.c \
	print.c \
	forcing.c \
	misc_func.c \
	daily.c \
	update.c

HEADERS_ = include/pihm.h \
	include/pihm_input_struct.h \
	include/pihm_const.h \
	include/pihm_struct.h \
	include/pihm_func.h

MODULE_HEADERS_ =
EXECUTABLE = pihm
MSG = "...  Compiling PIHM  ..."

ifeq ($(MAKECMDGOALS),flux-pihm)
  SFLAGS = -D_NOAH_ 
  MODULE_SRCS_= noah/lsm_func.c \
  	noah/coupling.c \
	spa/spa.c \
	noah/module_sf_noahlsm.c
  MODULE_HEADERS_ = include/noah.h \
	include/spa.h
  EXECUTABLE = flux-pihm
  MSG = "... Compiling FLUX-PIHM ..."
endif

ifeq ($(MAKECMDGOALS),rt-flux-pihm)
  SFLAGS = -D_RT_ -D_FLUX_PIHM_
  MODULE_SRCS_= noah/coupling.c \
	noah/module_sf_noahlsm.c \
	spa/spa.c \
	noah/lsm_func.c \
	rt/rt.c \
	rt/react.c \
	rt/os3d.c
  MODULE_HEADERS_ = noah/noah.h \
	spa/spa.h \
	rt/rt.h
  EXECUTABLE = rt-flux-pihm
  MSG = "... Compiling FLUX-PIHM ..."
endif

ifeq ($(MAKECMDGOALS),flux-pihm-bgc)
  SFLAGS = -D_BGC_ -D_NOAH_ 
  MODULE_SRCS_=	noah/coupling.c \
	noah/module_sf_noahlsm.c \
	spa/spa.c \
	noah/lsm_func.c \
	bgc/bgc_func.c \
	bgc/presim_state_init.c \
	bgc/make_zero_flux_struct.c \
	bgc/restart_io.c \
	bgc/firstday.c \
	bgc/bgc_spinup.c \
	bgc/metarr_init.c \
	bgc/zero_srcsnk.c \
	bgc/daily_bgc.c \
	bgc/get_co2.c \
	bgc/get_ndep.c \
	bgc/precision_control.c \
	bgc/daymet.c \
	bgc/radtrans.c \
	bgc/maint_resp.c \
	bgc/phenology.c \
	bgc/soilpsi.c \
	bgc/daily_allocation.c \
	bgc/canopy_et.c \
	bgc/photosynthesis.c \
	bgc/decomp.c \
	bgc/annual_rates.c \
	bgc/growth_resp.c \
	bgc/state_update.c \
	bgc/mortality.c \
	bgc/check_balance.c \
	bgc/summary.c \
	bgc/nleaching.c
  MODULE_HEADERS_ = include/noah.h \
	include/spa.h \
	include/bgc.h 
  EXECUTABLE = flux-pihm-bgc
  MSG = "... Compiling Flux-PIHM-BGC ..."
endif

ifeq ($(MAKECMDGOALS),flux-pihm-enkf)
  CC = mpicc
  SFLAGS = -D_ENKF_ -D_NOAH_
  MODULE_SRCS_ = enkf/read_enkf.c \
	enkf/enkf_func.c \
	enkf/perturb.c \
	enkf/EnKF.c \
	enkf/obs_oper.c \
  	noah/coupling.c \
  	noah/module_sf_noahlsm.c \
	spa/spa.c \
	noah/lsm_func.c 
  MODULE_HEADERS_ = include/enkf.h \
  	include/noah.h \
  	include/spa.h 
  EXECUTABLE = flux-pihm-enkf
  MSG = "... Compiling Flux-PIHM-EnKF ..."
endif

ifeq ($(MAKECMDGOALS),pihm-cycles)
  SFLAGS = -D_CYCLES_
  MODULE_SRCS_= 
  MODULE_HEADERS_ = 
  EXECUTABLE = pihm-cycles
  MSG = "... Compiling Flux-PIHM ..."
endif

SRCS = $(patsubst %,$(SRCDIR)/%,$(SRCS_))
HEADERS = $(patsubst %,$(SRCDIR)/%,$(HEADERS_))
OBJS = $(SRCS:.c=.o)

MODULE_SRCS = $(patsubst %,$(SRCDIR)/%,$(MODULE_SRCS_))
MODULE_HEADERS = $(patsubst %,$(SRCDIR)/%,$(MODULE_HEADERS_))
MODULE_OBJS = $(MODULE_SRCS:.c=.o)

.PHONY: all clean help sundials

help:		## Show this help
	@echo
	@echo "Makefile for MM-PIHM"
	@echo
	@echo "USAGE:"
	@echo
	@fgrep -h "##" $(MAKEFILE_LIST) | fgrep -v fgrep | sed -e 's/\\$$//' | sed -e 's/##//'
	@echo
	@echo "NOTE: Please always \"make clean\" when switching from one module to another!"
	@echo

all:		## Install sundials and compile PIHM
all:	sundials pihm

sundials:	## Install sundials library
sundials:
	cd sundials; ./configure; make; make install; cd ../
	@echo "SUNDIALS library installed."
pihm: 		## Compile PIHM
pihm:	$(OBJS)
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(LFLAGS) $(LIBS)

flux-pihm: 	## Complile Flux-PIHM (PIHM with land surface module, adapted from Noah LSM)
flux-pihm: $(OBJS) $(MODULE_OBJS)
	@echo
	@echo $(MSG)
	@echo
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(MODULE_OBJS) $(LFLAGS) $(LIBS)

rt-flux-pihm:	## Complile RT-Flux-PIHM (Reactive Transport Flux PIHM) for hydrogeochemical coupling.
rt-flux-pihm: $(OBJS) $(MODULE_OBJS)
	@echo
	@echo $(MSG)
	@echo
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(MODULE_OBJS) $(LFLAGS) $(LIBS)


flux-pihm-bgc: 	## Compile Flux-PIHM-BGC (Flux-PIHM with Biogeochemical module, adapted from Biome-BGC)
flux-pihm-bgc: $(OBJS) $(MODULE_OBJS)
	@echo
	@echo $(MSG)
	@echo
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(MODULE_OBJS) $(LFLAGS) $(LIBS)

flux-pihm-enkf: 	## Compile Flux-PIHM-EnKF (Flux-PIHM EnKF system)
flux-pihm-enkf:	$(OBJS) $(MODULE_OBJS)
	@echo
	@echo $(MSG)
	@echo
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(MODULE_OBJS) $(LFLAGS) $(LIBS)

pihm-cycles: 	## Compile PIHM-Cycles (Flux-PIHM with crop module, adapted from Cycles)
pihm-cycles: $(OBJS) $(MODULE_OBJS)
	@echo
	@echo $(MSG)
	@echo
	@$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o $(EXECUTABLE) $(OBJS) $(MODULE_OBJS) $(LFLAGS) $(LIBS)

tool:
	$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -o convert src/tool/convert.c

%.o: %.c $(HEADERS) $(MODULE_HEADERS)
	$(CC) $(CFLAGS) $(SFLAGS) $(INCLUDES) -c $<  -o $@


clean:		## Clean executables and objects
	@echo
	@echo "... Cleaning ..."
	@echo
	@$(RM) $(SRCDIR)/*.o $(SRCDIR)/*/*.o *~ pihm flux-pihm flux-pihm-bgc rt-flux-pihm flux-pihm-enkf
