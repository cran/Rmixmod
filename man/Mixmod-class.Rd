\name{Mixmod-class}
\alias{Mixmod-class}
\title{Constructor of [\code{\linkS4class{Mixmod}}] class}
\description{
  This is a class to run mixmod library.
}
\details{
  \describe{ \item{data}{numeric vector, matrix, or data
  frame of observations. Either qualitative or
  quantitative.} \item{dataType}{character. It defines
  whether data are quantitative or qualitative.}
  \item{nbCluster}{integer. It indicates the number of
  classes.} \item{knownPartition}{numeric. It contains the
  known partition.} \item{weight}{numeric vector with n
  (number of individuals) rows. Weight is optionnal. This
  option is to be used when weight is associated to the
  data.} \item{nbVariable}{integer. The number of
  variables.} \item{nbSample}{integer. The number of
  observations.} \item{criterion}{list of character. This
  option permits to select the criterion giving the best
  configuration of an execution.} \item{models}{a S4
  [\code{\linkS4class{Model}}] object. Defining the list of
  models to be tested.} \item{error}{logical. Say if at
  least one model finished with no error in MIXMOD.}
  \item{results}{a list of S4
  [\code{\linkS4class{MixmodResults}}] object containing
  all results. Results are sorted into a ascending order
  according to the first criterion (descending order for
  the CV criterion). This order can be changed by using the
  sortByCriterion() method.} }
}
\examples{
getSlots("Mixmod")
}

