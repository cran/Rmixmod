\docType{methods}
\name{hist}
\alias{hist}
\alias{hist,MixmodClustering-method}
\title{Histograms of a class [\code{\linkS4class{MixmodClustering}}]}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{MixmodClustering}}]}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Histograms of a class
  [\code{\linkS4class{MixmodClustering}}]
}
\examples{
data(geyser)
xem <- mixmodClustering(geyser)
hist(xem)
getMethod("hist", "MixmodClustering")
}
\seealso{
  \code{\link{hist}}
}

