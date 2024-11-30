#!/bin/bash
flags=( \
-b -e -E -n -s -t -T -v \
"--number-nonblank" "--number" "--squeeze-blank")

files=(t_1.txt t_2.txt t_3.txt)
COUNT=0
SUCCESS=0
FAIL=0
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m'





touch result_cols.txt

for flag1 in "${flags[@]}"; do
    for flag2 in "${flags[@]}"; do
        for flag3 in "${flags[@]}"; do
            for f1 in "${files[@]}"; do
                for f2 in "${files[@]}"; do

                    if [ $flag1 != $flag2 ] \
                    && [ $flag1 != $flag3 ] \
                    && [ $flag2 != $flag3 ] \
                    && [ $f1 != $f2 ]
                    then

                        (( COUNT++ ))
                        ./s21_cat \
                        $flag1 $flag2 $flag3 $f1 $f2 > diff_my.txt

                        cat \
                        $flag1 $flag2 $flag3 $f1 $f2 > diff_vanila.txt


                        if diff diff_my.txt diff_vanila.txt > /dev/null; then
                            (( SUCCESS++ ))
                            echo -e "${GREEN}${COUNT}\tSUCCESS\t \
                            $flag1\t$flag2\t$flag3\t \
                            $f1\t$f2\t$f3${NC}" \
                            >> result_cols.txt
                        else
                            (( FAIL++ ))
                            echo -e "${RED}${COUNT}\tFAIL\t \
                            $flag1\t$flag2\t$flag3\t \
                            $f1\t$f2\t$f3${NC}" \
                            >> result_cols.txt
                        fi

                    fi
                done
            done
        done
    done
done

cat result_cols.txt | column -t
tabs -10
echo -e "${NC}ALL:\t$COUNT"
echo -e "${GREEN}SUCCESS:\t$SUCCESS${NC}"
echo -e "${RED}FAIL:\t$FAIL${NC}"
tabs -0

rm -f diff_my.txt diff_vanila.txt result_cols.txt