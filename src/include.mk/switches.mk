##Compiler setting targets
# taken from: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
# among other places
#more comp settings, taken from here:
#https://software.intel.com/sites/default/files/article/326703/fp-control-2012-08.pdf
#see psql: qfp: switch_desc for description of flags
#also, psql: qfp: 

#individual flags
## optls

O0 	        := -O0
O1 	        := -O1
O2 	        := -O2
O3 	        := -O3

#switches

ASSOCMATH       := -fassociative-math
AVX 	        := -mavx
COMPTRANS 	:= -mp1
DEFFLAGS 	:=
DISFMA 		:= -no-fma
ENAFMA 		:= -fma
FASTEXPREC      := -fexcess-precision=fast
FASTM 	        := -ffast-math
FINMATH         := -ffinite-math-only
FLUSHDEN 	:= -ftz
FMAGCC 		:= -mavx2 -mfma
FMAICC 		:= -march=core-avx2
FORTRULES 	:= -fcx-fortran-rules
FPCONT 	        := -ffp-contract=on
FPMODDBL 	:= -fp-model=double
FPMODEXC 	:= -fp-model=except
FPMODEXT 	:= -fp-model=extended
FPMODFST1 	:= -fp-model fast=1
FPMODFST2  	:= -fp-model fast=2
FPMODPRE 	:= -fp-model=precise
FPMODSRC 	:= -fp-model=source
FPMODSTR 	:= -fp-model=strict
FPTRAP 		:= -fp-trap=common
FSTORE 	        := -ffloat-store
LIMITEDRANGE 	:= -fcx-limited-range
MCONSTS	        := -fmerge-all-constants
NOFLUSHDEN 	:= -no-ftz
NOPRECDIV 	:= -no-prec-div
NOTRAP 	        := -fno-trapping-math
PRECDIV 	:= -prec-div
RECIPMATH       := -freciprocal-math 
ROUNDINGMATH    := -frounding-math
ROUNDUSR 	:= -fp-port
SIGNALNAN       := -fsignaling-nans
SINGLEPRECCONST := -fsingle-precision-constant
SSE 	        := -mfpmath=sse -mtune=native
STDEXPREC       := -fexcess-precision=standard
UNSOPTS         := -funsafe-math-optimizations
USEFASTM 	:= --use_fast_math

# Collections 

OPCODES := O0 O1 O2 O3

#NOTE: gcc disables ASSOCMATH @ O0
SWITCHES_GCC :=  ASSOCMATH AVX DEFFLAGS FASTEXPREC FINMATH FMAGCC FORTRULES \
	   FPCONT FSTORE LIMITEDRANGE MCONSTS NOTRAP RECIPMATH ROUNDINGMATH \
	   SIGNALNAN SSE UNSOPTS

#NOTE: Clang not honoring ASSOCMATH (issues warning with 3.9)
# see: https://llvm.org/bugs/show_bug.cgi?id=27372

SWITCHES_CLANG := ASSOCMATH AVX DEFFLAGS FASTEXPREC FINMATH FMAGCC FMAICC FPCONT \
	     FSTORE MCONSTS NOTRAP RECIPMATH ROUNDINGMATH SIGNALNAN \
	     SINGLEPRECCONST SSE STDEXPREC UNSOPTS

SWITCHES_INTEL :=  AVX COMPTRANS DEFFLAGS DISFMA ENAFMA FLUSHDEN FMAGCC FMAICC \
	     FPMODDBL FPMODEXT FPMODFST1 FPMODFST2 FPMODPRE FPMODSRC FPMODSTR \
	     FSTORE LIMITEDRANGE MCONSTS NOFLUSHDEN NOPRECDIV PRECDIV \
	     ROUNDINGMATH ROUNDUSR SINGLEPRECCONST SSE USEFASTM
