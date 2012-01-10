##' Rmixmod a MIXture MODelling package
##'
##' Rmixmod is a package based on the existing MIXMOD software. MIXMOD is a tool for fitting a mixture model of multivariate gaussian or multinomial components to a given data set with either a clustering, a density estimation or a discriminant analysis point of view.
##'
##' \tabular{ll}{
##'   Package: \tab Rmixmod\cr 
##'   Type: \tab Package\cr 
##'   Version: \tab 1.0\cr
##'   Date: \tab 2012-01-10\cr 
##'   License: \tab GPL Version 3\cr 
##'   LazyLoad: \tab yes\cr
##' }
##'
##' The general purpose of the package is to discover, or explain, group structures in multivariate data sets with unknown (cluster analysis or clustering) or known class discriminant analysis or classification). It is an exploratory data analysis tool for solving clustering and classification problems. But it can also be regarded as a semi-parametric tool to estimate densities with Gaussian mixture distributions and multinomial distributions.
##'
##' Mathematically, mixture probability density function (pdf) \eqn{f} is a weighted sum of \eqn{K} components densities : 
##'
##' \deqn{
##'   f({\bf x}_i|\theta) = \sum_{k=1}^{K}p_kh({\bf x}_i|\lambda_k)
##' }
##' where \eqn{h(.|{\lambda}_k)} denotes a \eqn{d}-dimensional distribution parametrized by \eqn{\lambda_k}. The parameters are the mixing proportions \eqn{p_k} and the component of the distribution \eqn{\lambda_k}.\cr
##'
##' In the Gaussian case, \eqn{h} is the density of a Gaussian distribution with mean \eqn{\mu_k} and variance matrix \eqn{\Sigma_k}, and thus \eqn{\lambda_k = (\mu_k,\Sigma_k)}.
##'
##' In the qualitative case, \eqn{h} is a multinomial distribution and \eqn{\lambda_k=(a_k,\epsilon_k)} is the parameter of the distribution.
##'
##' Estimation of the mixture parameters is performed either through maximum likelihood via the EM (\emph{Expectation Maximization}, Dempster et al. 1977), the SEM (\emph{Stochastic EM}, Celeux and Diebolt 1985) algorithm or through classification maximum likelihood via the CEM  algorithm (\emph{Clustering EM}, Celeux and Govaert 1992). These three algorithms can be chained to obtain original fitting strategies (e.g. CEM then EM with results of CEM) to use advantages of each of them in the estimation process. As mixture problems usually have multiple relative maxima, the program will produce different results, depending on the initial estimates supplied by the user. If the userdoes not input his own initial estimates, some initial estimates procedures are proposed (random centers for instance).
##'
##' It is possible to constrain some input parameters. For example, dispersions can be equal between classes, etc.
##'
##' In the Gaussian case, fourteen models are implemented. They are based on the eigenvalue decomposition, are most generally used. They depend on constraints on the variance matrix such as same variance matrix between clusters, spherical variance matrix... and they are suitable for data sets in any dimension.
##'
##' In the qualitative case, five multinomial models are available. They are based on a reparametrization of the multinomial probabilities. 
##'
##' In both cases, the models and the number of clusters can be chosen by different criteria : BIC (Bayesian Information Criterion), ICL (Integrated Completed Likelihood, a classification version of BIC), NEC (Entropy Criterion), Cross-Validation (CV) or Double Cross-Validation (DCV).
##'
##'
##' @name Rmixmod-package
##' @aliases Rmixmod
##' @rdname Rmixmod-package
##' @docType package
##' @keywords package
##'
##' @author
##' Author: Remi Lebret
##' Maintainer: Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##'
##' @references Biernacki C., Celeux G., Govaert G., Langrognet F., 2006. "Model-Based Cluster and Discriminant Analysis with the MIXMOD Software". Computational Statistics and Data Analysis, vol. 51/2, pp. 587-600.
##'
##' @examples
##'   # load quantitative data set
##'   data(geyser)
##'   # Clustering in gaussian case
##'   xem1<-mixmodClustering(geyser)
##'   summary(xem1)
##'   plot(xem1)
##'   hist(xem1)
##'
##'   # load qualitative data set
##'   data(birds)
##'   # Clustering in multinomial case
##'   xem2<-mixmodClustering(birds)
##'   summary(xem2)
##'   plot(xem2)
##'   hist(xem2)
##'
##' @useDynLib Rmixmod
##' @exportPattern "^[[:alpha:]]+"
NULL

##' Quantitative data: Old Faithful Geyser
##' 
##' The file geyser.rda contains 272 observations from the Old Faithful Geyser in the Yellowstone National Park. Each observation consists of two measurements : the duration (in minutes) of the eruption and the waiting time (in minutes) to the next eruption.
##'
##' Old Faithful erupts more frequently than any other big geyser, although it is not the largest nor the most regular geyser in the park. Its average interval between two eruptions is about 76 minutes, varying from 45 - 110 minutes. An eruption lasts from 1.1/2 to 5 minutes, expels 3,700 - 8,400 gallons (14,000 - 32,000 liters) of boiling water, and reaches heights of 106 - 184 feet (30 - 55m). It was named for its consistent performance by members of the Washburn Expedition in 1870. Old Faithful is still as spectacular and predictable as it was a century ago.
##'
##' @format A data frame with 272 observations on the following 2 variables.
##'
##' \describe{
##'
##'   \item{\code{V1}}{a numeric vector}
##'
##'   \item{\code{V2}}{a numeric vector}
##'
##' }
##'
##' @source \url{http://www.geyserstudy.org/geyser.aspx?pGeyserNo=OLDFAITHFUL}
##'
##' @references 
##' Hardle, W. (1991). "Smoothing Techniques with Implementation in S". Springer-Verlag, New York. 
##' Azzalini, A. and Bowman, A. W. (1990). "A look at some data on the Old Faithful geyser". Applied Statistics 39, 357-365.
##'
##' @name geyser
##' @docType data
##' @keywords datasets
##' 
##' @examples
##'   data(geyser)
NULL

##' Qualitative data: Survival of passengers on the Titanic
##' 
##' For each person on board the fatal maiden voyage of the ocean liner Titanic, this dataset records: sex, age [adult/child], economic status [first/second/third class, or crew] and whether or not that person survived.
##' Values are aligned and delimited by blanks. There are no missing values.
##'
##' The sinking of the Titanic is a famous event, and new books are still being published about it.  Many well-known facts-from the proportions of first-class passengers to the "women and children first" policy, and the fact that that policy was not entirely successful in saving the women and children in the third class-are reflected in the survival rates for various classes of passenger.
##'
##' These data were originally collected by the British Board of Trade in their investigation of the sinking.  Note that there is not complete agreement among primary sources as to the exact numbers on board, rescued, or lost.
##'
##' Due in particular to the very successful film "Titanic", the last years saw a rise in public interest in the Titanic.  Very detailed data about the passengers is now available on the Internet, at sites such as "Encyclopedia Titanica" (\url{http://www.rmplc.co.uk/eduweb/sites/phind}).
##'
##' @format A data frame with 2201 observations on the following 4 variables.
##'
##' \describe{
##'
##'   \item{\code{Class}}{0 = crew, 1 = first, 2 = second, 3 = third, which denote the economic status of the subject}
##'
##'   \item{\code{Age}}{1 = adult, 0 = child, which denote if the subject is an adult or a child}
##'
##'   \item{\code{Sex}}{1 = male, 0 = female, which denote the sex of the subject}
##'
##'   \item{\code{Survived}}{1 = yes, 0 = no, which denote if the subject lived through the fatal maiden voyage of the ocean liner Titanic} 
##'
##' }
##'
##' @source 
##' The source provides a data set recording class, sex, age, and survival status for each person on board of the Titanic, and is based on data originally collected by the British Board of Trade and reprinted in:
##' British Board of Trade (1990), "Report on the Loss of the Titanic (S.S.)".  British Board of Trade Inquiry Report (reprint).  Gloucester, UK: Allan Sutton Publishing.
##'
##' @name titanic
##' @docType data
##' @keywords datasets
##' 
##' @examples
##'   data(titanic)
NULL

##' Qualitative data : morphological description of birds
##' 
##' The dataset contains details on the morphology of birds (puffins). Each individual (bird) is described by 6 qualitative variables. One variable for the gender and 5 variables giving a morphological description of the birds. There is 153 puffins divided in 3 sub-classes: dichrous (84 birds), lherminieri (34) and subalaris (35).
##'
##' @format A data frame with 153 observations on the following 6 variables.
##'
##' \describe{
##'
##'   \item{\code{V1}}{a numeric vector defining the gender (2 modalities, male or female).}
##'
##'   \item{\code{V2}}{a numeric vector describing the eyebrow stripe (5 modalities).}
##'
##'   \item{\code{V3}}{a numeric vector describing the breast (6 modalities).}
##'
##'   \item{\code{V4}}{a numeric vector describing the stripes (3 modalities).}
##'
##'   \item{\code{V5}}{a numeric vector describing the feathers (5 modalities).}
##'
##'   \item{\code{V6}}{a numeric vector describing the edging (4 modalities).}
##'
##' }
##'
##' @name birds
##' @docType data
##' @keywords datasets
##' 
##' @examples
##'   data(birds)
NULL
