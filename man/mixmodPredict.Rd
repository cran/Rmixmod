\name{mixmodPredict}
\alias{mixmodPredict}
\title{Create an instance of the [\code{\linkS4class{MixmodPredict}}] class}
\usage{
  mixmodPredict(data, classificationRule, ...)
}
\arguments{
  \item{data}{matrix or data frame containing
  quantitative,qualitative or composite data. Rows
  correspond to observations and columns correspond to
  variables.}

  \item{classificationRule}{a
  [\code{\linkS4class{MixmodResults}}] object which
  contains the classification rule computed in the
  mixmodLearn() or mixmodCluster() step.}
  \item{...}{internal}
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
  paste("accuracy= ",mean(as.integer(iris$Species[remaining.obs]) == prediction["partition"])*100
        ,"\%",sep="")

  ## A composite example with a heterogeneous data set
  data(heterodatatrain)
  ## Learning with training data
  learn <- mixmodLearn(heterodatatrain[-1],knownLabels=heterodatatrain$V1)
  ## Prediction on the testing data
  data(heterodatatest)
  prediction <- mixmodPredict(heterodatatest[-1],learn["bestResult"])
  # compare prediction with real results
  paste("accuracy= ",mean(heterodatatest$V1 == prediction["partition"])*100,"\%",sep="")
}
\author{
  Florent Langrognet and Remi Lebret and Christian Poli and Serge Iovleff,
  with contributions from C. Biernacki and G. Celeux and G.
  Govaert \email{contact@mixmod.org}
}

