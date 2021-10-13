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
        "model": ["rgg_torus", "er_p"],
        "n": 25000,
        "avg_deg": [2, 4, 8, 16, 32, 64],
        "seed": list(range(0, 1000)),
        "repetitions": 1,
        "iterations": 30,
    },
    stdout_file=output_dir + "extended.csv",
    header_command=header,
)

run.add(
    "conv_avg_deg",
    command,
    {
        "model": "rgg_torus",
        "n": 25000,
        "avg_deg": [
            2,
            3,
            5,
            7,
            10,
            20,
            30,
            50,
            100,
            200,
            300,
            400,
            450,
            500,
            700,
        ],
        "seed": list(range(0, 100)),
        "repetitions": 1,
        "iterations": 300,
    },
    stdout_file=output_dir + "conv_avg_deg.csv",
    header_command=header,
)

run.add(
    "crit_avg_deg",
    command,
    {
        "model": "rgg_torus",
        "n": [1000, 2000, 4000, 8000, 16000],
        "avg_deg_offset": [-100, -75, -50, -25, 0, 25, 50, 100],
        "avg_deg": lambda args: (
            max(round(math.sqrt(10 * int(args["n"]))) + int(args["avg_deg_offset"]), 5)
        ),
        ## "avg_deg": [50, 100, 150, 200, 250, 300, 350, 400, 450, 500],
        "seed": list(range(0, 500)),
        "repetitions": 1,
        "iterations": 200,
    },
    stdout_file=output_dir + "crit_avg_deg.csv",
    header_command=header,
)

run.run()
