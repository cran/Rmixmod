\name{mixmodClustering}
\alias{mixmodClustering}
\title{Create an instance of the [\code{\linkS4class{MixmodClustering}}] class}
\usage{
  mixmodClustering(data, dataType, nbCluster = 2:8, models,
  algo = mixmodAlgo(), criterion = "BIC", weight,
  knownPartition)
}
\arguments{
  \item{data}{matrix containing quantitative or qualitative
  data. Rows correspond to observations and columns
  correspond to variables.}

  \item{dataType}{character. It defines whether data are
  quantitative or qualitative. Possible values are
  "quantitative" or "qualitative". If missing, it will be
  guessed.}

  \item{nbCluster}{numeric listing the number of custers.
  Default is a list from 2 to 8 clusters.}

  \item{models}{a [\code{\linkS4class{Model}}] object
  defining the list of models to run.}

  \item{algo}{a [\code{\linkS4class{Algo}}] object
  containing the algorithm to run. Call mixmodAlgo() method
  by default.}

  \item{criterion}{character defining the criterion to
  select the best model. Possible values: "BIC", "ICL",
  "NEC". Default is "BIC".}

  \item{weight}{numeric vector with n (number of
  individuals) rows. Weight is optionnal. This option is to
  be used when weight is associated to the data.}

  \item{knownPartition}{matrix of size nbSample*nbCluster.
  it will be used for semi-supervised classification when
  labels are known. One line with only values of zero will
  designate a sample for who label isn't known.}
}
\value{
  Returns an instance of the
  [\code{\linkS4class{MixmodClustering}}] class.
}
\description{
  This function computes an optimal mixture model according
  to the criteria furnished, and the list of model defined
  in [\code{\linkS4class{Model}}], using the algorithm
  specified in [\code{\linkS4class{Algo}}].
}
\examples{
## A quantitative example with the famous geyser data set
data(geyser)
## with default values
mixmodClustering(geyser)

## An example with the famous titanic data set
data(birds)
mixmodClustering(data=birds, criterion= "BIC", model = mixmodMultinomialModel(), nbCluster = 2:8)

## use graphics functions
xem <- mixmodClustering(geyser)
plot(xem)
hist(xem)

## get summary
summary(xem)
}
\author{
  Remi Lebret \email{remi.lebret@math.univ-lille1.fr}
}

