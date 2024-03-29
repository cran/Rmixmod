% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/Rmixmod.R
\docType{data}
\name{heterodata}
\alias{heterodata}
\title{Composite data with training and testing set}
\format{
A data frame with 200 individuals (rows) and 5 variables (columns).

\describe{

  \item{\code{V1}}{categorical variable with two modalities (1 & 2).}

  \item{\code{V2}}{categorical variable with two modalities (1 & 2).}

  \item{\code{V3}}{categorical variable with two modalities (1 & 2).}

  \item{\code{V4}}{numeric variable.}

  \item{\code{V5}}{numeric variable.}
}
}
\description{
The data set is made up of 5 variables: 3 categorical variables and 2 quantitative variables.
The original data set contains 200 individuals. The training data set has 300 individuals while the testing data set has
100 individuals.
}
\examples{
data(heterodata)
summary(heterodata)
}
\seealso{
\code{\link{heterodatatrain}} and \code{\link{heterodatatest}}
}
\keyword{datasets}
