% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/Mixmod.R
\docType{methods}
\name{barplot}
\alias{barplot}
\alias{barplot,Mixmod-method}
\title{Barplot of a class [\code{\linkS4class{Mixmod}}]}
\usage{
\S4method{barplot}{Mixmod}(height, ...)
}
\arguments{
\item{height}{an object of class [\code{\linkS4class{Mixmod}}] (???)}

\item{...}{further arguments passed to or from other methods}
}
\description{
Barplot of qualitative data from a [\code{\linkS4class{Mixmod}}] object using parameters
to plot probabilities of modalities.
}
\details{
Each line corresponds to one variable. Barplot is drawn for each cluster with the probabilities for
each modality to be in that cluster.
}
\examples{
data(birds)
xem2 <- mixmodCluster(birds, 2)
barplot(xem2)
barplot(xem2, variables = c(2, 3, 4))
barplot(xem2, variables = c("eyebrow", "collar"))
}
\seealso{
\code{\link{barplot}}
}
