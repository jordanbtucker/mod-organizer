#!/bin/bash


SCRIPTNAME=/tmp/bbcodescript.sed


> $SCRIPTNAME

./retrieveissues.sh > issues_generated.bbcode
./retrievetodo.sh > todo_generated.bbcode
./extract_changelog.sh 0.11.4 > changelog_generated.bbcode


for bbcfile in *.bbcode
do
  echo "/#include $bbcfile/{ 
    r $bbcfile
    d
  }" >> $SCRIPTNAME
done

sed -f $SCRIPTNAME template.bbcode

rm $SCRIPTNAME

