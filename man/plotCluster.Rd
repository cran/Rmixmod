\name{plotCluster}
\alias{plotCluster}
\title{Plotting of a class [\code{\linkS4class{MixmodResults}}]}
\usage{
  plotCluster(x, data, variable1 = colnames(data)[1],
    variable2 = colnames(data)[2], col = x@partition + 1,
    pch = x@partition, xlab = variable1, ylab = variable2,
    add.ellipse = TRUE, ...)
}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{MixmodResults}}]}

  \item{data}{a matrix or a data frame containing a
  quantitative data set.}

  \item{variable1}{character containing the name of the
  first variable. First column of data by default.}

  \item{variable2}{character containing the name of the
  second variable. Second column of data by default.}

  \item{col}{a specification for the default plotting
  color. By default partition is used to separate clusters
  with different colors.}

  \item{pch}{either an integer specifying a symbol or a
  single character to be used as the default in plotting
  points. By default partition is used to seperate clusters
  with different symbols.}

  \item{xlab}{a title for the x axis. Variable1 by
  default.}

  \item{ylab}{a title for the y axis. Variable2 by
  default.}

  \item{add.ellipse}{a boolean. Add ellipses to graph. TRUE
  by default.}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Biplot of two variables from a quantitative data set. Use
  parameters and partition from a
  [\code{\linkS4class{MixmodResults}}] object to
  distinguish the different clusters.
}
\details{
  Ellipsoids (i.e. linear transformations of hyperspheres)
  centered at the mean can be drawn using the parameters
  computed by MIXMOD. The directions of the principal axes
  of the ellipsoids are given by the eigenvectors of the
  covariance matrix \eqn{\Sigma}. The squared relative
  lengths of the principal axes are given by the
  corresponding eigenvalues.
}
\examples{
data(geyser)
  xem <- mixmodCluster(geyser,3)
  plotCluster(xem["bestResult"], geyser)
}
\seealso{
  \code{\link{plot}}
}

