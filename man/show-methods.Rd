\docType{methods}
\name{show}
\alias{show}
\alias{show,Algo-method}
\alias{show,GaussianParameter-method}
\alias{show,Mixmod-method}
\alias{show,MixmodClustering-method}
\alias{show,Model-method}
\alias{show,MultinomialParameter-method}
\title{Show description of the Algo class to standard output.}
\arguments{
  \item{object}{A \code{Algo} object.}

  \item{object}{A \code{Model} object.}

  \item{object}{A \code{GaussianParameter} object.}

  \item{object}{An object.}

  \item{object}{A \code{MixmodClustering} object.}

  \item{object}{A \code{MultinomialParameter} object.}
}
\value{
  NULL. Prints to standard out.

  NULL. Prints to standard out.

  NULL. Prints to standard out.

  NULL. Prints to standard out.

  NULL. Prints to standard out.

  NULL. Prints to standard out.
}
\description{
  Show description of the Algo class to standard output.

  Show description of the Model class to standard output.

  Show description of the GaussianParameter class to
  standard output.

  Show description of the class to standard output.

  Show description of the MixmodClustering class to
  standard output.

  Show description of the MultinomialParameter class to
  standard output.
}
\examples{
algo <- mixmodAlgo()
algo
show(algo)
getMethod("show", "Algo")
gaussianModel <- mixmodGaussianModel()
gaussianModel
show(gaussianModel)

multinomialModel <- mixmodMultinomialModel()
multinomialModel
show(multinomialModel)
data(geyser)
xem <- mixmodClustering(geyser)
show(xem@parameters)
getMethod("show", "GaussianParameter")
data(geyser)
xem <- mixmodClustering(geyser)
xem
show(xem)
getMethod("show", "MixmodClustering")
data(birds)
xem <- mixmodClustering(birds)
show(xem@parameters)
getMethod("show", "MultinomialParameter")
}
\seealso{
  \code{\link{show}}

  \code{\link{show}}

  \code{\link{show}}

  \code{\link{show}}

  \code{\link{show}}
}

