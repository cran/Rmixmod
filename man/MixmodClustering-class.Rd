\name{MixmodClustering-class}
\alias{MixmodClustering-class}
\title{Constructor of [\code{\linkS4class{MixmodClustering}}] class}
\description{
  This is a class to run clustering with mixmod. Inherits
  the [\code{\linkS4class{Mixmod}}] class.
}
\details{
  \describe{

  \item{nbClusterInBestModel}{integer. Number of class for
  the best model.}

  \item{knownPartition}{matrix. This option is to be used
  when a partition of data is already known.}

  \item{labels}{vector of integers.}

  \item{proba}{a matrix of probabilities.}

  }
}
\examples{
## A quantitative example with the famous iris data set
data(iris)
## with default values
new("MixmodClustering", data=as.matrix(iris[1:4]))

getSlots("Mixmod")
}
\author{
  Remi Lebret \email{remi.lebret@math.univ-lille1.fr}
}

