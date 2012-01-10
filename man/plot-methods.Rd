\docType{methods}
\name{plot}
\alias{plot}
\alias{plot,MixmodClustering-method}
\title{Plotting of a class [\code{\linkS4class{MixmodClustering}}]}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{MixmodClustering}}]}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Plotting of a class
  [\code{\linkS4class{MixmodClustering}}]
}
\examples{
data(geyser)
xem <- mixmodClustering(geyser)
plot(xem)
getMethod("plot", "MixmodClustering")
}
\seealso{
  \code{\link{plot}}
}

