\name{mixmodMultinomialModel}
\alias{mixmodMultinomialModel}
\title{Create an instance of the [\code{\linkS4class{MultinomialModel}}] class}
\usage{
  mixmodMultinomialModel(free.proportions = TRUE,
  equal.proportions = TRUE, variable.independency,
  component.independency)
}
\arguments{
  \item{free.proportions}{logical to include models with
  free proportions. Default is TRUE.}

  \item{equal.proportions}{logical to include models with
  equal proportions. Default is FALSE.}

  \item{variable.independency}{logical to include models
  where \eqn{[\varepsilon_k^j]} is independent of the
  variable \eqn{j}. Optionnal.}

  \item{component.independency}{logical to include models
  where \eqn{[\varepsilon_k^j]} is independent of the
  component \eqn{k}. Optionnal.}
}
\value{
  an object of [\code{\linkS4class{MultinomialModel}}]
  containing some of the 10 Binary Models: \tabular{rlll}{
  Model \tab Prop. \tab Var. \tab Comp. \cr Binary_p_E \tab
  Equal \tab TRUE \tab TRUE \cr Binary_p_Ej \tab \tab FALSE
  \tab TRUE \cr Binary_p_Ek \tab \tab TRUE \tab FALSE \cr
  Binary_p_Ekj \tab \tab FALSE \tab FALSE \cr Binary_p_Ekjh
  \tab \tab FALSE \tab FALSE \cr Binary_pk_E \tab Free \tab
  TRUE \tab TRUE \cr Binary_pk_Ej \tab \tab FALSE \tab TRUE
  \cr Binary_pk_Ek \tab \tab TRUE \tab FALSE \cr
  Binary_pk_Ekj \tab \tab FALSE \tab FALSE \cr
  Binary_pk_Ekjh \tab \tab FALSE \tab FALSE \cr }
}
\description{
  Define a list of gaussian model to test in MIXMOD.
}
\details{
  In the multinomial mixture model, the multinomial
  distribution is associated to the \eqn{j}th variable of
  the \eqn{k}th component is reparameterized by a center
  \eqn{a_k^j} and the dispersion \eqn{\varepsilon_k^j}
  around this center. Thus, it allows us to give an
  interpretation similar to the center and the variance
  matrix used for continuous data in the Gaussian mixture
  context. In the following, this model will be denoted by
  \eqn{[\varepsilon_k^j]}. In this context, three other
  models can be easily deduced. We note
  \eqn{[\varepsilon_k]} the model where
  \eqn{\varepsilon_k^j} is independent of the variable
  \eqn{j}, \eqn{[\varepsilon^j]} the model where
  \eqn{\varepsilon_k^j} is independent of the component
  \eqn{k} and, finally, \eqn{[\varepsilon]} the model where
  \eqn{\varepsilon_k^j} is independent of both the variable
  $j$ and the component \eqn{k}.  In order to maintain some
  unity in the notation, we will denote also
  \eqn{[\varepsilon_k^{jh}]} the most general model
  introduced at the previous section.
}
\examples{
mixmodMultinomialModel()
mixmodMultinomialModel(equal.proportions=TRUE)
}
\author{
  Remi Lebret \email{remi.lebret@math.univ-lille1.fr}
}
\references{
  C. Biernacki, G. Celeux, G. Govaert, F. Langrognet.
  "Model-Based Cluster and Discriminant Analysis with the
  MIXMOD Software". Computational Statistics and Data
  Analysis, vol. 51/2, pp. 587-600. (2006)
}

