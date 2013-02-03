#!/bin/bash

#wget http://issue.tannin.eu/tbg/modorganizer/issues/find/predefined_search/1/search/1/format/csv -O issues.csv
wget -q "http://issue.tannin.eu/tbg/modorganizer/issues/find/format/csv/issues/find?filters%5Btext%5D%5Boperator%5D=%3D&filters%5Btext%5D%5Bvalue%5D=&template=results_normal&template_parameter=&issues_per_page=0&groupby=issuetype&grouporder=asc&filters%5Bstate%5D%5B0%5D%5Boperator%5D=%3D&filters%5Bstate%5D%5B0%5D%5Bvalue%5D=0&filters%5Bissuetype%5D%5B3%5D%5Boperator%5D=%21%3D&filters%5Bissuetype%5D%5B3%5D%5Bvalue%5D=1" -O todo.csv

echo "[list]"
grep "Confirmed" todo.csv | awk -v FS='","|^"|"$' '{print "[*] [url=http://issue.tannin.eu/tbg/modorganizer/issues/"$3"]"$4"[/url]"}'
echo "[/list]"
