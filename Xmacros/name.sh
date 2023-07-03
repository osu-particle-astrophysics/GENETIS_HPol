#!/bin/bash

for i in *.txt;
do
  name=`echo ${i} | sed "s+${i}+\.${i}+"`
  mv $i $name
done
