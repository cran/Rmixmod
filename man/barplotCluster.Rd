\name{barplotCluster}
\alias{barplotCluster}
\title{Barplot of a class [\code{\linkS4class{MixmodResults}}]}
\usage{
  barplotCluster(x, data, variables = colnames(data),
    main = paste("Barplot of", variables), ...)
}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{MixmodResults}}]}

  \item{data}{a vector, matrix or data frame containing a
  qualitative data set.}

  \item{variables}{list of variables names to compute a
  barplot. All variables from data by default.}

  \item{main}{a list of title for the barplot. main must
  have the same length than variables.}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Barplot of qualitative data object using parameters from
  a [\code{\linkS4class{MixmodResults}}] to plot
  probablities of modalities.
}
\details{
  Each line corresponds to one variable. A barplot is drawn
  for each cluster with the probabilities for each modality
  to be in that cluster.
}
\examples{
data(birds)
  xem2 <- mixmodCluster(birds,2)
  barplotCluster(xem2["bestResult"], birds)
}
\seealso{
  \code{\link{barplot}}
}

