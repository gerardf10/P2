#!/bin/bash
alpha0=${1:-4.95}
# Be sure that this file has execution permissions:
# Use the nautilus explorer or chmod +x run_vad.sh

# Establecemos que el código de retorno de un pipeline sea el del último programa con código de retorno
# distinto de cero, o cero si todos devuelven cero.
set -o pipefail

# Write here the name and path of your program and database
DIR_P2=$HOME/PAV/P2
DB=$DIR_P2/db.v4
CMD="$DIR_P2/bin/vad -0 $alpha0"

for filewav in $DB/*/*.wav; do
    # skip files we already processed
    [[ "${filewav##*/}" == *_stripped.wav ]] && continue

    echo "**************** $filewav ****************"

    filevad=${filewav%.wav}.vad
    # $CMD -i $filewav -o $filevad || exit 1
    filewavOut=${filewav%.wav}_stripped.wav

    $CMD -i "$filewav" -o "$filevad" -w "$filewavOut" || exit 1
done
scripts/vad_evaluation.pl $DB/*/*lab

exit 0
