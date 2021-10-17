# Geometric Flip Schelling Experiments #

This repository contains the code to reproduce our experiments
in the paper

> **The Impact of Geometry on Monochrome Regions in the Flip Schelling
> Process**  
> *Thomas Bläsius, Tobias Friedrich, Martin S. Krejca, Louise Molitor*

## Running the Experiments ##

**Note:** The experiments will take some time and use up to 10 cores. If
you want to change the number of cores, edit the line
`run.use_cores(10)` in the file `experiments.py` accordingly.

### Easy Mode ###

If you are on a Linux environment and satisfies all [dependencies](#dependencies), the
experiments can be run using just one command.

```console
python experiments.py pre experiments post
```

This will compile the c++ code, run the experiments, and create the
plots.  The resulting data and the plots can be found in `eval/data/`
and `eval/tex/master.pdf`, respectively.

### Hard Mode ###

On non-Linux systems, you might run into some problems with the above
command.  Instead use the following three steps to compile the code,
run the experiments, and create the plots.

#### Compiling ####

Use cmake to compile the code in `code/`.  Make sure to use
`code/release/` as build directory so that the experiment script can
find it.  Usually this should work somewhat like this:

```console
mkdir code/release
cd code/release
cmake ..
make
```

This will create the command line interface for running the flip
Schelling process on different types of random networks.  If you want
to use this directly (instead of running our set of experiments), call
`run --help` for more information and see `experiments.py` for example
calls.

#### Running the Experiments ####

To actually run the experiments, call 
```console
python experiments.pyexperiments
```
  This will take some time.  The resulting data is stored
in csv-files in `eval/data/`.

To run only a subset of experiments, specify the experiments you want
to run when calling `experiments.py`, e.g., call `python
experiments.py intro, visualization` to run the experiments for the
intro and the visualization.

#### Create the Plots ####

First, run the R-scripts in the `eval/` folder (`crit-avg-deg.R`,
`extended.R`, `intro-plot.R`, `visualization.R`).  This creates
pdf-plots in `eval/pdf/` as well as tex-plots in `eval/tex/`.  To
compile the tex plots, run LaTeX for the file `eval/tex/master.tex`.

## Dependencies ##

  * **cmake, c++:** To compile the c++ code, you need cmake and a c++
    compiler that supports c++20.
