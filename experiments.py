import run
import math

run.use_cores(10)
run.group("all")

output_dir = "eval/data/"

command = "code/release/run -m [[model]] -n [[n]] -d [[avg_deg]] -s [[seed]] -r [[repetitions]] -i [[iterations]] --no-header"
header = "code/release/run --only-header"

run.add(
    "intro",
    command,
    {
        "model": ["rgg_torus", "er_p"],
        "n": [5000, 10000, 15000, 20000, 25000],
        "avg_deg": [2, 4, 8, 16, 32],
        "seed": 17,
        "repetitions": 1000,
        "iterations": 1,
    },
    stdout_file=output_dir + "intro.csv",
    header_command=header,
)

run.add(
    "extended",
    command,
    {
        "seed": list(range(0, 1000)),
        "model": ["rgg_torus", "er_p"],
        "n": 25000,
        "avg_deg": [2, 4, 8, 16, 32],
        "repetitions": 1,
        "iterations": 30,
    },
    stdout_file=output_dir + "extended.csv",
    header_command=header,
)

run.add(
    "crit_avg_deg",
    command + " --skip-intermediate",
    {
        # "seed": list(range(0, 500)),
        "seed": list(range(500, 1000)),
        "model": "rgg_torus",
        "n": [x ** 2 for x in [30, 40, 50, 60, 70, 80, 90, 100]],
        "factor": [
            beg + (end - beg) * i / steps
            for (beg, end, steps) in [(0.5, 3.5, 10)]
            for i in range(steps + 1)
        ],
        "avg_deg": lambda a: round(math.sqrt(float(a["n"])) * float(a["factor"])),
        "repetitions": 1,
        "iterations": 200,
    },
    stdout_file=output_dir + "crit_avg_deg_extra.csv",
    header_command=header,
)

run.run()
