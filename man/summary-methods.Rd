\docType{methods}
\name{summary}
\alias{summary}
\alias{summary,GaussianParameter-method}
\alias{summary,Mixmod-method}
\alias{summary,MixmodPredict-method}
\alias{summary,MixmodResults-method}
\alias{summary,MultinomialParameter-method}
\title{Produce result summaries of a Rmixmod class}
\arguments{
  \item{x}{Either a \code{\linkS4class{Mixmod}} object, a
  \code{\linkS4class{Parameter}} object or a
  \code{\linkS4class{MixmodResults}} object.}

  \item{...}{further arguments passed to or from other
  methods}
}
\value{
  NULL. Summaries to standard out.
}
\description{
  Produce result summaries of a Rmixmod class
}
\examples{
data(geyser)
  xem <- mixmodCluster(geyser,3)
  summary(xem)
  summary(xem["bestResult"])
  summary(xem["bestResult"]["parameters"])
}
\seealso{
  \code{\link{summary}}
}

