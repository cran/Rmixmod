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
  Histograms of quantitative data from a
  [\code{\linkS4class{Mixmod}}] object using parameters to
  plot densities.
}
\details{
  Data with the density of each cluster and the mixture
  density are drawn for each variable.
}
\examples{
data(geyser)
  xem1 <- mixmodCluster(geyser,3)
  hist(xem1)
}
\seealso{
  \code{\link{hist}}
}

