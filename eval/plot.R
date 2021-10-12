library(ggplot2)
library(sitools)
library(tikzDevice)

tbl <- read.csv("measurements_1k.csv")
tbl$monochrome_after_fraction <- tbl$monochrome_after / tbl$m;
tbl$monochrome_before_fraction <- tbl$monochrome_before / tbl$m
tbl <- tbl[tbl$deg_avg_exp >= 2,]

theme_set(theme_bw())
colors <- c("#D55E00", "#E69F00", "#009E73", "#0072B2",
            "#56B4E9", "#F0E442", "#999999", "#CC79A7")

model_labs <- c("G(n, m)",
                "Erd{\\H o}s-R\\'enyi graphs",
                "random geometric graphs")
names(model_labs) <- c("er_m", "er_p", "rgg")

percentile <- function(perc) {
    # perc is the percentile which should be computed for the numeric vector x
    function(x) quantile(x, perc * 1e-2, na.rm = TRUE)
}

si_latex_labels <- function(x) {
    paste0(x / 1000, "\\,k")
}

p <- ggplot(tbl, aes(x = n,
                     y = monochrome_after_fraction,
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

ggsave("plot.pdf", plot = p, width = 6, height = 3)

tikz(file = "tex/plot.tex", width = 4.7, height = 2.1)
print(p)
dev.off()
