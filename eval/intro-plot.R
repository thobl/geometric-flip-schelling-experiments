library(ggplot2)
library(tikzDevice)

source("theme.R")

tbl <- read.csv("measurements.csv")
tbl$monochrome_fraction <- tbl$monochrome / tbl$m;
tbl$color_changes_fraction <- tbl$color_changes / tbl$m;
tbl$minority_count_fraction <- tbl$minority_count / tbl$m;
tbl <- tbl[tbl$deg_avg_exp >= 2, ]

tbl <- tbl[tbl$iteration == 1, ]
tbl <- tbl[tbl$model != "rgg", ]

model_labs <- c("Erd{\\H o}s-R\\'enyi graphs",
                "random geometric graphs")
names(model_labs) <- c("er_p", "rgg_torus")

percentile <- function(perc) {
    # perc is the percentile which should be computed for the numeric vector x
    function(x) quantile(x, perc * 1e-2, na.rm = TRUE)
}

si_latex_labels <- function(x) {
    paste0(x / 1000, "\\,k")
}

p <- ggplot(tbl,
            aes(x = n,
                y = monochrome_fraction,
                color = factor(deg_avg_exp))) +
    stat_summary(fun = mean, fun.max = percentile(75), fun.min = percentile(25),
                 geom = "errorbar", width = 0) +
    stat_summary(fun = mean, geom = "line") +
    stat_summary(fun = mean, geom = "point", size = 0.7) +
    facet_grid(cols = vars(model), labeller = labeller(model = model_labs)) +
    xlab("number of vertices $n$") +
    ylab("fraction of monochrome edges") +
    scale_x_continuous(labels = si_latex_labels) +
    scale_color_manual(name = "average\ndegree", values = colors) +
    theme(legend.position = "right")

ggsave("pdf/intro-plot.pdf", plot = p, width = 6, height = 3)

tikz(file = "tex/intro-plot.tex", width = 4.7, height = 2.1)
print(p)
dev.off()
