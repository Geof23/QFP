#!/bin/bash -l
#SBATCH --time 48:00:00
#SBATCH --nodes 1
#SBATCH --ntasks 28
#SBATCH --cpus-per-task 1
#SBATCH --array=1-6
#SBATCH --output=autobisect-%A_%a.log
#SBATCH --account soc-kp
#SBATCH --partition soc-kp


### % SBATCH --account owner-guest
### % SBATCH --partition kingspeak-guest

set -e
set -u
set -x

ml singularity
ml binutils
ml

scratch=/scratch/kingspeak/serial/u0415196
sexec="singularity exec $HOME/singularity/archflit-mfem.simg"

function finish {
  local resultsdir="$HOME/mfem-results-$SLURM_JOB_ID"
  cd /tmp/mfem/flit_tests
  mkdir -p "$resultsdir/gt"
  find . -maxdepth 1 -type f -name ground-truth\* -exec cp {} "$resultsdir/gt/" \;
  mkdir -p "$resultsdir/sqlite"
  find . -maxdepth 1 -type f -name \*.sqlite -exec cp {} "$resultsdir/sqlite/" \;
  mkdir -p "$resultsdir/bisect"
  find . -maxdepth 1 -mindepth 1 -name \*bisect\* -exec cp -r {} "$resultsdir/bisect/" \;
  
  cd "$HOME"
  rm -rf /tmp/mfem
  rm -rf /tmp/hypre-2.11.2
  rm -rf /tmp/metis-4.0.3
}
trap finish EXIT

# Setup and build all of the tests
rm -rf /tmp/{mfem,hypre,metis}
$sexec cp -r /opt/mfem /tmp
$sexec cp -r /opt/hypre-2.11.2 /tmp/hypre
$sexec cp -r /opt/metis-4.0.3 /tmp/metis

cd /tmp/mfem/flit_tests

echo '#!/bin/bash' > icpc-wrap
echo '#!/bin/bash' > g++-wrap
echo '#!/bin/bash' > clang++-wrap
echo '#!/bin/bash' > mpic++-wrap
echo "$sexec "'icpc "$@"'    >> icpc-wrap
echo "$sexec "'g++ "$@"'     >> g++-wrap
echo "$sexec "'clang++ "$@"' >> clang++-wrap
echo "$sexec "'mpic++ "$@"' >> mpic++-wrap
chmod +x icpc-wrap g++-wrap clang++-wrap mpic++-wrap

sed -i \
  -e "s,'icpc','./icpc-wrap',g" \
  -e "s,'g++','./g++-wrap',g" \
  -e "s,'clang++','./clang++-wrap',g" \
  flit-config.toml
sed -i -e '/RUN_WRAPPER/d' custom.mk
echo "RUN_WRAPPER := srun -n 1 --mem-per-cpu=0 --exclusive $sexec" >> custom.mk

grep wrap flit-config.toml

# Update the autogenerated Makefile
flit update
sed -i -e 's,mpic++,./mpic++-wrap,g' Makefile

export MFEM_HOME=/tmp/mfem
# This is the homes within the singularity container
export HYPRE_HOME=/tmp/hypre
export METIS_HOME=/tmp/metis

# Now that we have wrapped everything with singularity, we can just call make (hopefully)
# We compile everything using all resources, then run everything using only 20 cores
# to have better timing metrics
#make -j56 ground-truth.csv

# To save time, we can copy previous runs of the gtrun
cp $HOME/mfem-results-clang/gt/* ./
make gt -j56
make ground-truth.csv --touch

# Run variable-clang
#cp ~/mfem-results/variable-clang.csv ./
#sed -i -e 's,clang++,./clang++-wrap,g' variable-clang.csv
#flit import ./variable-clang.csv -D ./variable-clang.sqlite
#flit bisect \
#  --verbose \
#  --auto-sqlite-run variable-clang.sqlite \
#  --parallel 28 \
#  --jobs 20 \
#  --delete

# Run variable-gcc
cp ~/mfem-results/variable-${SLURM_ARRAY_TASK_ID}.csv ./
sed -i -e 's|,g++,|,./g++-wrap,|g' variable-${SLURM_ARRAY_TASK_ID}.csv
sed -i -e 's|,clang++,|,./clang++-wrap,|g' variable-${SLURM_ARRAY_TASK_ID}.csv
sed -i -e 's|,icpc,|,./icpc-wrap,|g' variable-${SLURM_ARRAY_TASK_ID}.csv
flit import ./variable-${SLURM_ARRAY_TASK_ID}.csv -D ./variable-${SLURM_ARRAY_TASK_ID}.sqlite
flit bisect \
  --verbose \
  --auto-sqlite-run variable-${SLURM_ARRAY_TASK_ID}.sqlite \
  --parallel 28 \
  --jobs 20 \
  --delete

# Since the tests are run using mpirun, we can't run tests naively,
# otherwise we will only be able to use two physical cores at
# a time, and all jobs will be using those two physical cores.
# So instead, we use srun for each one in parallel

# Run the tests
# The --mem-per-cpu=0 is special and indicates to have steps within the job
# allocation should share the total memory allocation.  Each one will have
# access to the full memory allocation at the same time.
#find results -type f -executable | \
#  parallel -j20 --ungroup \
#    srun -n 1 --mem-per-cpu=0 --exclusive \
#      $sexec \
#        bash -c \
#          "\"make -j1 {}-out-comparison.csv && \
#             cp {}-out {}-out-comparison.csv ~/mfem-results/\""

