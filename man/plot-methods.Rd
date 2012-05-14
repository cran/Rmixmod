\docType{methods}
\name{plot}
\alias{plot}
\alias{plot,Mixmod-method}
\title{Plotting of a class [\code{\linkS4class{Mixmod}}]}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{Mixmod}}]}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Plotting data from a [\code{\linkS4class{Mixmod}}] object
  using parameters and partition to distinguish the
  different clusters.
}
\details{
  For quantitative case, ellipsoids (i.e. linear
  transformations of hyperspheres) centered at the mean are
  drawn using the parameters computed by MIXMOD. The
  directions of the principal axes of the ellipsoids are
  given by the eigenvectors of the covariance matrix
  \eqn{\Sigma}. The squared relative lengths of the
  principal axes are given by the corresponding
  eigenvalues. A 1-dimensional representation of variables
  with the densities is drawn on the diagonal.

  For qualitative case, a Multiple Correspondance Analysis
  is performed to get a 2-dimensional representation of the
  data set. Bigger symbol means that observations are
  similar.
}
\examples{
## for quantitative case
  data(geyser)
  xem <- mixmodCluster(geyser,3)
  plot(xem)

  ## for qualitative case
  data(birds)
  xem2 <- mixmodCluster(birds,2)
  plot(xem2)
}
\seealso{
  \code{\link{plot}}
}

