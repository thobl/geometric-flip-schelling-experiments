theme_set(theme_bw())
colors <- c("#D55E00", "#E69F00", "#009E73", "#0072B2",
            "#F0E442", "#CC79A7", "#999999", "#56B4E9")

add_fractions <- function(tbl) {
    tbl$monochrome_fraction <- tbl$monochrome / tbl$m;
    tbl$color_changes_fraction <- tbl$color_changes / tbl$n;
    tbl$minority_count_fraction <- tbl$minority_count / tbl$n;
    return(tbl)
}

percentile <- function(perc) {
    ## perc is the percentile which should be computed for the numeric vector x
    function(x) quantile(x, perc * 1e-2, na.rm = TRUE)
}

main_plot <- function(tbl, mapping) {
    model_labs <- c("G(n, m)",
                    "Erd{\\H o}s-R\\'enyi graphs",
                    "random geometric graphs (square)",
                    "random geometric graphs")
    names(model_labs) <- c("er_m", "er_p", "rgg_square", "rgg_torus")
    labeller <- labeller(model = model_labs)

    ggplot(tbl,
           mapping = mapping) +
        stat_summary(geom = "errorbar",
                     fun = median,
                     fun.max = percentile(75),
                     fun.min = percentile(25),
                     size = 0.2, width = 0) +
        stat_summary(fun = median, geom = "line") +
        stat_summary(fun = median, geom = "point", size = 0.7) +
        facet_grid(cols = vars(model), labeller = labeller) +
        theme(legend.position = "right") +
        scale_color_manual(values = colors)
}
