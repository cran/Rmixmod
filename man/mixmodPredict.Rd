\name{mixmodPredict}
\alias{mixmodPredict}
\title{Create an instance of the [\code{\linkS4class{MixmodPredict}}] class}
\usage{
  mixmodPredict(data, classificationRule)
}
\arguments{
  \item{data}{matrix or data frame containing quantitative
  or qualitative data. Rows correspond to observations and
  columns correspond to variables.}

  \item{classificationRule}{a
  [\code{\linkS4class{MixmodResults}}] object which
  contains the classification rule computed in the
  mixmodLearn() or mixmodCluster() step.}
}
\value{
  Returns an instance of the
  [\code{\linkS4class{MixmodPredict}}] class which contains
  predicted partition and probabilities.
}
\description{
  This function computes the second step of a discriminant
  analysis. The aim of this step is to assign remaining
  observations to one of the groups.
}
\examples{
# start by extract 10 observations from iris data set
  remaining.obs<-sample(1:nrow(iris),10)
  # then run a mixmodLearn() analysis without those 10 observations
  learn<-mixmodLearn(iris[-remaining.obs,1:4], iris$Species[-remaining.obs])
  # create a MixmodPredict to predict those 10 observations
  prediction <- mixmodPredict(data=iris[remaining.obs,1:4], classificationRule=learn["bestResult"])
  # show results
  prediction
  # compare prediction with real results
  as.integer(iris$Species[remaining.obs]) == prediction["partition"]
}
\author{
  Remi Lebret and Serge Iovleff and Florent Langrognet,
  with contributions from C. Biernacki and G. Celeux and G.
  Govaert \email{contact@mixmod.org}
}

