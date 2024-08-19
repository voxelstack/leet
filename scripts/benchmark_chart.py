import json
import sys
from os import path

# Transforms a benchmark result into a box chart json.
# Used to add benchmark charts to docs.
#
# Args:
# <benchmark csv (input)>
# <chart json (output)>
# <benchmark runs to include on the chart>

src = sys.argv[1]
dst = sys.argv[2]
runs = sys.argv[3:]

chart = {
    "data": [],
    "layout": {
        "showlegend": False,
        "xaxis": {
            "title": { "text": "Time (ms)" },
            "type": "linear"
        },
        "yaxis": { "type": "category" },
        "autosize": True
    }
}

with open(src) as csv_in:
    for run in csv_in.readlines():
        [name, times] = run.rstrip().split(",")
        if name in runs:
            chart["data"].append({
                "x": list(map(int, times.split(";"))),
                "line": { "color": "#2980b9" },
                "type": "box",
                "name": name,
                "orientation": "h",
                "width": 0.15
            })

with open(dst, "w", encoding="utf-8") as json_out:
    json.dump(chart, json_out)
