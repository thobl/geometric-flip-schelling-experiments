library(ggplot2)
library(egg)
library(tikzDevice)

source("theme.R")

tbl <- read.csv("data/extended.csv")
tbl$monochrome_fraction <- tbl$monochrome / tbl$m;
tbl$color_changes_fraction <- tbl$color_changes / tbl$n;
tbl$minority_count_fraction <- tbl$minority_count / tbl$n;

model_labs <- c("G(n, m)",
                "Erd{\\H o}s-R\\'enyi graphs",
                "random geometric graphs (square)",
                "random geometric graphs (torus)")
names(model_labs) <- c("er_m", "er_p", "rgg_square", "rgg_torus")


percentile <- function(perc) {
    # perc is the percentile which should be computed for the numeric vector x
    function(x) quantile(x, perc * 1e-2, na.rm = TRUE)
}

p1 <- ggplot(tbl,
       aes(x = iteration,
           y = monochrome_fraction,
           color = factor(deg_avg_exp))) +
    stat_summary(fun = mean, fun.max = percentile(75), fun.min = percentile(25),
                 geom = "errorbar", width = 0) +
    stat_summary(fun = mean, geom = "line") +
    stat_summary(fun = mean, geom = "point", size = 0.2) +
    facet_grid(cols = vars(model), labeller = labeller(model = model_labs))

p2 <- ggplot(tbl,
       aes(x = iteration,
           y = minority_count_fraction,
           color = factor(deg_avg_exp))) +
    stat_summary(fun = mean, fun.max = percentile(75), fun.min = percentile(25),
                 geom = "errorbar", width = 0) +
    stat_summary(fun = mean, geom = "line") +
    stat_summary(fun = mean, geom = "point", size = 0.2) +
    facet_grid(cols = vars(model), labeller = labeller(model = model_labs))

p3 <- ggplot(tbl[tbl$iteration > 0, ],
       aes(x = iteration,
           y = color_changes_fraction,
           color = factor(deg_avg_exp))) +
    stat_summary(fun = mean, fun.max = percentile(75), fun.min = percentile(25),
                 geom = "errorbar", width = 0) +
    stat_summary(fun = mean, geom = "line") +
    stat_summary(fun = mean, geom = "point", size = 0.2) +
    facet_grid(cols = vars(model), labeller = labeller(model = model_labs))

p <- ggarrange(p1, p2, p3, nrow = 3, ncol = 1)

ggsave("pdf/extended-plot.pdf", plot = p, width = 9, height = 9)
