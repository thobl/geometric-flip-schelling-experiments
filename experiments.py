import run

run.use_cores(10)

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
        "model": ["rgg_torus", "rgg_square", "er_p"],
        "n": 25000,
        "avg_deg": [2, 4, 8, 16, 32],
        "seed": list(range(0, 1000)),
        "repetitions": 1,
        "iterations": 40,
    },
    stdout_file=output_dir + "extended.csv",
    header_command=header,
)

run.run()
