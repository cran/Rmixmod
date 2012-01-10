\docType{methods}
\name{summary}
\alias{summary}
\alias{summary,GaussianParameter-method}
\alias{summary,MixmodClustering-method}
\alias{summary,MultinomialParameter-method}
\title{Produce result summaries of a class [\code{\linkS4class{GaussianParameter}}]}
\arguments{
  \item{x}{A \code{GaussianParameter} object.}

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
  NULL. Summaries to standard out.

  NULL. Summaries to standard out.

  NULL. Summaries to standard out.
}
\description{
  Produce result summaries of a class
  [\code{\linkS4class{GaussianParameter}}]

  Produce result summaries of a class
  [\code{\linkS4class{MixmodClustering}}]

  Produce result summaries of a class
  [\code{\linkS4class{MultinomialParameter}}]
}
\examples{
data(geyser)
xem <- mixmodClustering(geyser)
summary(xem@parameters)
getMethod("summary", "GaussianParameter")
data(geyser)
xem <- mixmodClustering(geyser)
summary(xem)
getMethod("summary", "MixmodClustering")
data(birds)
xem <- mixmodClustering(birds)
summary(xem@parameters)
getMethod("summary", "MultinomialParameter")
}
\seealso{
  \code{\link{summary}}

  \code{\link{summary}}

  \code{\link{summary}}
}

