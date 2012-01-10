\docType{methods}
\name{print}
\alias{print}
\alias{print,Algo-method}
\alias{print,GaussianParameter-method}
\alias{print,Mixmod-method}
\alias{print,MixmodClustering-method}
\alias{print,Model-method}
\alias{print,MultinomialParameter-method}
\title{Print the Algo class to standard output.}
\arguments{
  \item{x}{A \code{Algo} object.}

  \item{...}{further arguments passed to or from other
  methods}

  \item{x}{A \code{Model} object.}

  \item{...}{further arguments passed to or from other
  methods}

  \item{x}{A \code{GaussianParameter} object.}

  \item{...}{further arguments passed to or from other
  methods}

  \item{x}{An object.}

  \item{...}{further arguments passed to or from other
  methods}

  \item{x}{A \code{MixmodClustering} object.}

  \item{...}{further arguments passed to or from other
  methods}

  \item{x}{A \code{MultinomialParameter} object.}

  \item{...}{further arguments passed to or from other
  methods}
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
  Print the Algo class to standard output.

  Print the Model class to standard output.

  Print the GaussianParameter class to standard output.

  Print the class to standard output.

  Print the MixmodClustering class to standard output.

  Print the MultinomialParameter class to standard output.
}
\examples{
algo <- mixmodAlgo()
print(algo)
getMethod("print", "Algo")
gaussianModel <- mixmodGaussianModel()
print(gaussianModel)

multinomialModel <- mixmodMultinomialModel()
print(multinomialModel)
data(geyser)
xem <- mixmodClustering(geyser)
print(xem@parameters)
getMethod("print", "GaussianParameter")
data(geyser)
xem <- mixmodClustering(geyser)
print(xem)
getMethod("print", "MixmodClustering")
data(birds)
xem <- mixmodClustering(birds)
print(xem@parameters)
getMethod("print", "MultinomialParameter")
}
\seealso{
  \code{\link{print}}

  \code{\link{print}}

  \code{\link{print}}

  \code{\link{print}}

  \code{\link{print}}
}

