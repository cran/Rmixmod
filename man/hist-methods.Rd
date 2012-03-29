\docType{methods}
\name{hist}
\alias{hist}
\alias{hist,Mixmod-method}
\title{Histograms of a class [\code{\linkS4class{Mixmod}}]}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{Mixmod}}]}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Histograms of data from a [\code{\linkS4class{Mixmod}}]
  object using parameters to plot densities in a
  quantitative case and probablities in a qualitative case.
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
## for quantitative case
  data(geyser)
  xem1 <- mixmodCluster(geyser,3)
  hist(xem1)

  ## for qualitative case
  data(birds)
  xem2 <- mixmodCluster(birds,2,factor=c(2,5,6,3,5,4))
  hist(xem2)
}
\seealso{
  \code{\link{hist}}
}

