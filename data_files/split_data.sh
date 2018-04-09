#! /bin/bash

f1="data_algo1.csv"
f2="data_algo2.csv"

cat $f1 | grep "Preprocessing_Time" > pre_pro_1.csv
cat $f1 | grep -v "Preprocessing_Time" > stats_1.csv

cat $f2 | grep "Preprocessing_Time" > pre_pro_2.csv
cat $f2 | grep -v "Preprocessing_Time" > stats_2.csv

f=stats_2.csv

num_lines=$(cat $f | wc -l)
num_graphs=$(expr $num_lines / 1000)

cp $f temp1.csv
for ((i=1; i<=$num_graphs; i+=1))
do
    # each for loop reads 50 lines from the top, 5 times
    x=$(expr $i \* 1000)
    L=$(expr $num_lines - $x)
    tail -$L temp1.csv > temp2.csv
    head -200 temp1.csv >> stats_2_10.csv
    head -400 temp1.csv | tail -200 >> stats_2_20.csv
    head -600 temp1.csv | tail -200 >> stats_2_30.csv
    head -800 temp1.csv | tail -200 >> stats_2_40.csv
    head -1000 temp1.csv | tail -200 >> stats_2_50.csv
    cp temp2.csv temp1.csv
done

rm temp*
