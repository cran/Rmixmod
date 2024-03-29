% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/GaussianParameter.R
\docType{class}
\name{GaussianParameter-class}
\alias{GaussianParameter-class}
\title{Constructor of [\code{\linkS4class{GaussianParameter}}] class}
\description{
This class defines parameters of a Gaussian Mixture Model. Inherits the [\code{\linkS4class{Parameter}}] class.
}
\details{
\describe{
  \item{mean}{a numeric vector containing mean of each cluster.}
  \item{variance}{a vector of matrix containing variance matrix of each cluster.}
}
}
\examples{
new("GaussianParameter")

getSlots("GaussianParameter")
}
