#!/bin/bash
      
for file in *.sh;
do
  name=`echo $file | sed "s+${file}+\.${file}+"`
  mv $file $name
done
