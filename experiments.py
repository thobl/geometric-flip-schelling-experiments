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

# run.add(
#     "crit_avg_deg",
#     command + " --skip-intermediate",
#     {
#         "model": "rgg_torus",
#         "avg_deg_center": [100, 200, 300, 400, 500, 600],
#         "n": lambda args: int(int(args["avg_deg_center"]) ** 2 / 5),
#         "avg_deg_offset": [-1, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1],
#         "avg_deg": lambda a: round(
#             int(a["avg_deg_center"])
#             + float(a["avg_deg_offset"]) * 0.5 * (int(a["avg_deg_center"]) + 80)
#         ),
#         "seed": 17,
#         "repetitions": 1000,
#         "iterations": 300,
#     },
#     stdout_file=output_dir + "crit_avg_deg.csv",
#     header_command=header,
# )

run.add(
    "crit_avg_deg",
    command + " --skip-intermediate",
    {
        "model": "rgg_torus",
        "seed": list(range(0, 500)),
        "n": [x ** 2 for x in [30, 40, 50, 60, 70, 80, 90, 100]],
        "factor_max": 3.5,
        "factor_min": 0.5,
        "factor_step": [i / (steps - 1) for steps in [11] for i in range(steps)],
        "factor": lambda a: (
            float(a["factor_min"])
            + float(a["factor_step"])
            * (float(a["factor_max"]) - float(a["factor_min"]))
        ),
        "avg_deg": lambda a: round(math.sqrt(float(a["n"])) * float(a["factor"])),
        "repetitions": 1,
        "iterations": 200,
    },
    stdout_file=output_dir + "crit_avg_deg.csv",
    header_command=header,
)

run.run()
