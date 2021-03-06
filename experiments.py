import run
import math

run.use_cores(10)

run.group("pre")

run.add("compile", "cmake -S code -B code/release && make -C code/release", {})

run.group("experiments")

output_dir = "eval/data/"

command = '"code/release/run" -m [[model]] -n [[n]] -d [[avg_deg]] -s [[seed]] -i [[iterations]] --no-header'
header = '"code/release/run" --only-header'

run.add(
    "intro",
    command + " --skip-intermediate",
    {
        "seed": list(range(0, 1000)),
        "model": ["rgg_torus", "er_p"],
        "n": [5000, 10000, 15000, 20000, 25000],
        "avg_deg": [2, 4, 8, 16, 32],
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
        "iterations": 30,
    },
    stdout_file=output_dir + "extended.csv",
    header_command=header,
)

run.add(
    "crit_avg_deg",
    command + " --skip-intermediate",
    {
        "seed": list(range(0, 1000)),
        "model": "rgg_torus",
        "n": [x ** 2 for x in [30, 40, 50, 60, 70, 80, 90, 100]],
        "factor": [
            beg + (end - beg) * i / steps
            for (beg, end, steps) in [(0.5, 3.5, 10)]
            for i in range(steps + 1)
        ],
        "avg_deg": lambda a: round(math.sqrt(float(a["n"])) * float(a["factor"])),
        "iterations": 200,
    },
    stdout_file=output_dir + "crit_avg_deg.csv",
    header_command=header,
)

run.add(
    "visualization",
    command + " --ipe-output [[ipe]] && ipetoipe -pdf [[ipe]]",
    {
        "model": "rgg_torus",
        "n": 500,
        "avg_deg": 16,
        "iterations": 20,
        "seed": 17,
        "ipe": output_dir + "visualization.ipe",
    },
    stdout_file=output_dir + "visualization.csv",
    header_command=header,
)


run.group("post")

run.add(
    "plots",
    "cd eval && Rscript [[script]] && cd ..",
    {
        "script": [
            "intro-plot.R",
            "extended.R",
            "visualization.R",
            "crit-avg-deg.R",
        ]
    },
)

run.add("latex", "cd eval/tex && pdflatex master.tex cd ../..", {})

if __name__ == "__main__":
    run.run()
