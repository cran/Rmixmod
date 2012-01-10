\docType{package}
\name{Rmixmod-package}
\alias{Rmixmod}
\alias{Rmixmod-package}
\title{Rmixmod a MIXture MODelling package}
\description{
  Rmixmod is a package based on the existing MIXMOD
  software. MIXMOD is a tool for fitting a mixture model of
  multivariate gaussian or multinomial components to a
  given data set with either a clustering, a density
  estimation or a discriminant analysis point of view.
}
\details{
  \tabular{ll}{ Package: \tab Rmixmod\cr Type: \tab
  Package\cr Version: \tab 1.0\cr Date: \tab 2012-01-10\cr
  License: \tab GPL Version 3\cr LazyLoad: \tab yes\cr }

  The general purpose of the package is to discover, or
  explain, group structures in multivariate data sets with
  unknown (cluster analysis or clustering) or known class
  discriminant analysis or classification). It is an
  exploratory data analysis tool for solving clustering and
  classification problems. But it can also be regarded as a
  semi-parametric tool to estimate densities with Gaussian
  mixture distributions and multinomial distributions.

  Mathematically, mixture probability density function
  (pdf) \eqn{f} is a weighted sum of \eqn{K} components
  densities :

  \deqn{ f({\bf x}_i|\theta) = \sum_{k=1}^{K}p_kh({\bf
  x}_i|\lambda_k) } where \eqn{h(.|{\lambda}_k)} denotes a
  \eqn{d}-dimensional distribution parametrized by
  \eqn{\lambda_k}. The parameters are the mixing
  proportions \eqn{p_k} and the component of the
  distribution \eqn{\lambda_k}.\cr

  In the Gaussian case, \eqn{h} is the density of a
  Gaussian distribution with mean \eqn{\mu_k} and variance
  matrix \eqn{\Sigma_k}, and thus \eqn{\lambda_k =
  (\mu_k,\Sigma_k)}.

  In the qualitative case, \eqn{h} is a multinomial
  distribution and \eqn{\lambda_k=(a_k,\epsilon_k)} is the
  parameter of the distribution.

  Estimation of the mixture parameters is performed either
  through maximum likelihood via the EM (\emph{Expectation
  Maximization}, Dempster et al. 1977), the SEM
  (\emph{Stochastic EM}, Celeux and Diebolt 1985) algorithm
  or through classification maximum likelihood via the CEM
  algorithm (\emph{Clustering EM}, Celeux and Govaert
  1992). These three algorithms can be chained to obtain
  original fitting strategies (e.g. CEM then EM with
  results of CEM) to use advantages of each of them in the
  estimation process. As mixture problems usually have
  multiple relative maxima, the program will produce
  different results, depending on the initial estimates
  supplied by the user. If the userdoes not input his own
  initial estimates, some initial estimates procedures are
  proposed (random centers for instance).

  It is possible to constrain some input parameters. For
  example, dispersions can be equal between classes, etc.

  In the Gaussian case, fourteen models are implemented.
  They are based on the eigenvalue decomposition, are most
  generally used. They depend on constraints on the
  variance matrix such as same variance matrix between
  clusters, spherical variance matrix... and they are
  suitable for data sets in any dimension.

  In the qualitative case, five multinomial models are
  available. They are based on a reparametrization of the
  multinomial probabilities.

  In both cases, the models and the number of clusters can
  be chosen by different criteria : BIC (Bayesian
  Information Criterion), ICL (Integrated Completed
  Likelihood, a classification version of BIC), NEC
  (Entropy Criterion), Cross-Validation (CV) or Double
  Cross-Validation (DCV).
}
\examples{
# load quantitative data set
data(geyser)
# Clustering in gaussian case
xem1<-mixmodClustering(geyser)
summary(xem1)
plot(xem1)
hist(xem1)

# load qualitative data set
data(birds)
# Clustering in multinomial case
xem2<-mixmodClustering(birds)
summary(xem2)
plot(xem2)
hist(xem2)
}
\author{
  Author: Remi Lebret Maintainer: Remi Lebret
  \email{remi.lebret@math.univ-lille1.fr}
}
\references{
  Biernacki C., Celeux G., Govaert G., Langrognet F., 2006.
  "Model-Based Cluster and Discriminant Analysis with the
  MIXMOD Software". Computational Statistics and Data
  Analysis, vol. 51/2, pp. 587-600.
}
\keyword{package}

