\docType{methods}
\name{show}
\alias{show}
\alias{show,GaussianParameter-method}
\alias{show,Mixmod-method}
\alias{show,MixmodCluster-method}
\alias{show,MixmodDAResults-method}
\alias{show,MixmodLearn-method}
\alias{show,MixmodPredict-method}
\alias{show,MixmodResults-method}
\alias{show,Model-method}
\alias{show,MultinomialParameter-method}
\alias{show,Strategy-method}
\title{Show description of a Rmixmod class to standard output.}
\arguments{
  \item{object}{a Rmixmod object: a
  \code{\linkS4class{Strategy}}, a
  \code{\linkS4class{Model}}, a
  \code{\linkS4class{GaussianParameter}}, a
  \code{\linkS4class{MultinomialParameter}}, a
  \code{\linkS4class{MixmodResults}}, a
  \code{\linkS4class{MixmodCluster}}, a
  \code{\linkS4class{MixmodLearn}} or a
  \code{\linkS4class{MixmodPredict}}.}
}
\value{
  NULL. Prints to standard out.
}
\description{
  Show description of a Rmixmod class to standard output.
}
\examples{
## for strategy
  strategy <- mixmodStrategy()
  show(strategy)

  ## for Gaussian models
  gmodel <- mixmodGaussianModel()
  show(gmodel)
  ## for multinomial models
  mmodel <- mixmodMultinomialModel()
  show(mmodel)

  ## for clustering
  data(geyser)
  xem <- mixmodCluster(geyser,3)
  show(xem)
  ## for Gaussian parameters
  show(xem["bestResult"]["parameters"])

  ## for discriminant analysis
  # start by extract 10 observations from iris data set
  iris.partition<-sample(1:nrow(iris),10)
  # then run a mixmodLearn() analysis without those 10 observations
  learn<-mixmodLearn(iris[-iris.partition,1:4], iris$Species[-iris.partition])
  # create a MixmodPredict to predict those 10 observations
  prediction <- mixmodPredict(data=iris[iris.partition,1:4], classificationRule=learn["bestResult"])
  # show results
  show(prediction)
}
\seealso{
  \code{\link{show}}
}

