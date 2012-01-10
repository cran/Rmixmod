\name{Mixmod-class}
\alias{Mixmod-class}
\title{Constructor of [\code{\linkS4class{Mixmod}}] class}
\description{
  This is a class to interface mixmod kernel with R.
}
\details{
  \describe{

  \item{data}{numeric vector, matrix, or data frame of
  observations. Either qualitative or quantitative.}

  \item{nbCluster}{integer. It indicates the number of
  classes.}

  \item{nbSample}{integer. The number of observations.}

  \item{nbVariable}{integer. The number of variables.}

  \item{weight}{numeric vector with n (number of
  individuals) rows. Weight is optionnal. This option is to
  be used when weight is associated to the data.}

  \item{dataType}{character. It defines whether data are
  quantitative or qualitative.}

  \item{criterion}{character. This option permits to select
  the criterion giving the best configuration of an
  execution.}

  \item{models}{a S4 [\code{\linkS4class{Model}}] object.
  Defining the list of models to be tested.}

  \item{algo}{a S4 [\code{\linkS4class{Algo}}] object.
  Defining the algorithm used in the strategy for the
  models selection.}

  \item{parameters}{a S4 [\code{\linkS4class{Parameter}}]
  object. The best model parameters.}

  \item{likelihood}{numeric. The best model likelihood.}

  \item{bestModel}{character. Name of the best model.}

  \item{criterionValue}{numeric. Value of the criterion
  used to select the best model.}

  }
}
\examples{
getSlots("Mixmod")
}
\author{
  Remi Lebret \email{remi.lebret@math.univ-lille1.fr}
}

