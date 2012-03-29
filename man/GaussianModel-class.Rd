\name{GaussianModel-class}
\alias{GaussianModel-class}
\title{Constructor of [\code{\linkS4class{GaussianModel}}] class}
\description{
  This class defines a gaussian Model. Inherits the
  [\code{\linkS4class{Model}}] class.
}
\details{
  \describe{ \item{family}{character defining a family of
  models.} }
}
\examples{
new("GaussianModel")
  new("GaussianModel",family="general")

  getSlots("GaussianModel")
}

