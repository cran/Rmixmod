\docType{methods}
\name{print}
\alias{print}
\alias{print,CompositeParameter-method}
\alias{print,GaussianParameter-method}
\alias{print,Mixmod-method}
\alias{print,MixmodCluster-method}
\alias{print,MixmodDAResults-method}
\alias{print,MixmodLearn-method}
\alias{print,MixmodPredict-method}
\alias{print,MixmodResults-method}
\alias{print,Model-method}
\alias{print,MultinomialParameter-method}
\alias{print,Strategy-method}
\title{Print a Rmixmod class to standard output.}
\arguments{
  \item{x}{a Rmixmod object: a
  \code{\linkS4class{Strategy}}, a
  \code{\linkS4class{Model}}, a
  \code{\linkS4class{GaussianParameter}}, a
  \code{\linkS4class{MultinomialParameter}}, a
  \code{\linkS4class{MixmodResults}}, a
  \code{\linkS4class{MixmodCluster}}, a
  \code{\linkS4class{MixmodLearn}} or a
  \code{\linkS4class{MixmodPredict}}.}

  \item{...}{further arguments passed to or from other
  methods}
}
\value{
  NULL. Prints to standard out.
}
\description{
  Print a Rmixmod class to standard output.
}
\examples{
## for strategy
  strategy <- mixmodStrategy()
  print(strategy)

  ## for Gaussian models
  gmodel <- mixmodGaussianModel()
  print(gmodel)
  ## for multinomial models
  mmodel <- mixmodMultinomialModel()
  print(mmodel)

  ## for clustering
  data(geyser)
  xem <- mixmodCluster(geyser,3)
  print(xem)
  ## for Gaussian parameters
  print(xem["bestResult"]["parameters"])

  ## for discriminant analysis
  # start by extract 10 observations from iris data set
  iris.partition<-sample(1:nrow(iris),10)
  # then run a mixmodLearn() analysis without those 10 observations
  learn<-mixmodLearn(iris[-iris.partition,1:4], iris$Species[-iris.partition])
  # print learn results
  print(learn)
  # create a MixmodPredict to predict those 10 observations
  prediction <- mixmodPredict(data=iris[iris.partition,1:4], classificationRule=learn["bestResult"])
  # print prediction results
  print(prediction)
}
\seealso{
  \code{\link{print}}
}

