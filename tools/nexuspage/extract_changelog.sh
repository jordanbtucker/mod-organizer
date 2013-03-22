#!/bin/bash

if [ $# -lt 1 ]
then
  echo "prints changelog up to (including) a specified version"
  echo "Issue: this fails if the specified version is the latest version"
  echo "Usage: $0 <version>"
  exit 1
fi

sed -n -e "1,/\[$1\]/p;/\[$1\]/,/^$/{/\[$1\]/b; /^$/b; p }" changelog.dat |grep -v "^-" | sed -e 's/^\[\([0-9.]*\)\]/\[b\]\1\[\/b\]\[list\]/;s/^*/[*]/;s/^$/[\/list]\n/'
echo "[/list]"
#sed -n -e "1,/\[$1\]/p;/\[$1\]/,/^$/{/\[$1\]/b; /^$/b; p }" changelog.dat |grep "^[\[*]" | sed -e 's/^[([0-9.]*)]/[b]\1[/b][list]/;s/^*/[*]/;s/^$/[/list]\n/'
