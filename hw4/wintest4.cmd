if /I %1 == "1" set data = ".\data\"
if /I %1 == "2" set data = ".\v2testdata\"
if /I %2 == "all" (
    for %%g in ("%data%*.in") do py -3 id3.py < %%g
) else (
    py -3 id3.py < %data% %2.in
)
PAUSE