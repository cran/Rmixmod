\name{MixmodLearn-class}
\alias{MixmodLearn-class}
\title{Constructor of [\code{\linkS4class{MixmodLearn}}] class}
\description{
  This is a class to run discriminant analysis with mixmod.
  Inherits the [\code{\linkS4class{Mixmod}}] class.
}
\details{
  \describe{ \item{bestResult}{a S4
  [\code{\linkS4class{MixmodDAResults}}] object containing
  the best model results.} \item{nbCVBlocks}{integer which
  defines the number of observation per block to perform
  the Cross Validation.} }
}
\examples{
## A quantitative example with the famous iris data set
  new("MixmodLearn", data=iris[1:4], knownPartition=iris$Species)

  getSlots("MixmodLearn")
}

