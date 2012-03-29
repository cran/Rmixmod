\name{histCluster}
\alias{histCluster}
\title{Histogram of a class [\code{\linkS4class{MixmodResults}}]}
\usage{
  histCluster(x, data, variables = colnames(data),
    xlab = rep("", length(variables)),
    main = paste("Histogram of", variables), ...)
}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{MixmodResults}}]}

  \item{data}{a vector, matrix or data frame containing a
  quantitative or a qualitative data set.}

  \item{variables}{list of variables names to compute a
  histogram. All variables from data by default.}

  \item{xlab}{a list of title for the x axis. xlab must
  have the same length than variables.}

  \item{main}{a list of title for the histogram. main must
  have the same length than variables.}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Histograms of data object using parameters from a
  [\code{\linkS4class{MixmodResults}}] to plot densities in
  a quantitative case and probablities in a qualitative
  case.
}
\details{
  For quantitative case, data with the density of each
  cluster and the mixture density are drawn for each
  variable.

  For qualitative case, each line corresponds to one
  variable. On the left-hand side is drawn a barplot with
  data. Then a barplot is drawn for each cluster with the
  probabilities for each modality to be in that cluster.
}
\examples{
## for qualitative case
  data(geyser)
  xem1 <- mixmodCluster(geyser,3)
  histCluster(xem1["bestResult"], geyser)

  ## for qualitative case
  data(birds)
  xem2 <- mixmodCluster(birds,2,factor=c(2,5,6,3,5,4))
  histCluster(xem2["bestResult"], birds)
}
\seealso{
  \code{\link{hist}}
}

