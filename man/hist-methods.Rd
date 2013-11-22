\docType{methods}
\name{hist}
\alias{hist}
\alias{hist,Mixmod-method}
\title{Histograms of a class [\code{\linkS4class{Mixmod}}]}
\arguments{
  \item{x}{an object of class
  [\code{\linkS4class{Mixmod}}]}

  \item{variables}{list of variables names (or indices) to
  compute a histogram. All variables from data by default.}

  \item{...}{further arguments passed to or from other
  methods}
}
\description{
  Histograms of quantitative data from a
  [\code{\linkS4class{Mixmod}}] object using parameters to
  plot densities.
}
\details{
  Data with the density of each cluster and the mixture
  density are drawn for each variable.
}
\examples{
data(iris)
  xem <- mixmodCluster(iris[1:4],3)
  hist(xem)
  hist(xem,variables=c(1,3))
  hist(xem,variables=c("Sepal.Length","Sepal.Width"))
}
\seealso{
  \code{\link{hist}}
}

