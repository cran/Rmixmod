\name{CompositeParameter-class}
\alias{CompositeParameter-class}
\title{Constructor of [\code{\linkS4class{CompositeParameter}}] class}
\description{
  This class defines parameters of a Heterogeneous Mixture
  Model. Inherits the [\code{\linkS4class{Parameter}}]
  class.
}
\details{
  \describe{ \item{g_parameter}{an object of class
  CompositeParameter} \item{m_parameter}{an object of class
  MultinomialParameter} }
}
\examples{
new("CompositeParameter")

  getSlots("CompositeParameter")
}

