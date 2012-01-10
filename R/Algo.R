###################################################################################
##                               Algo.R                                          ##
###################################################################################


###################################################################################
##' Create an instance of [\code{\linkS4class{Algo}}] class 
##'
##' This class will contain all the parameters needed by the estimation algorithms.
##'
##' There are different ways to initialize an algorithm :
##'
##'    \describe{
##'
##'        \item{random}{Initialization from a random position is a standard way to
##'        initialize an algorithm. This random initial position is obtained by
##'        choosing at random centers in the data set. This simple strategy is
##'        repeated \eqn{5} times (the user can choose the number of times) from
##'        different random positions and the position that maximises the
##'        likelihood is selected.}
##'
##'        \item{partition}{This option initializes the strategy from a
##'        specified classification (full or partial) of the observations.
##'        This option provides the possibility to use mixmod for
##'        Discriminant Analysis and in this case, partition must be full.}
##'
##'        \item{parameter}{This option starts the strategy with specified initial values
##'        of the unknown mixture model parameters, i.e. the mixing
##'        proportions and the parameters of the distribution.}
##'
##'        \item{smallEM}{A maximum of \eqn{50} iterations of the EM algorithm according to the process : \eqn{n_i} numbers of iterations
##'        of EM are done (with random initialization) until the \code{smallEM} stop criterion value has been reached. 
##'        This action is repeated until the sum of \eqn{n_i}
##'
##'        reaches \eqn{50} iterations (or if in one action \eqn{50} iterations are reached before the stop criterion value).\\
##'        It appears that repeating runs of EM is generally profitable since using a single run
##'        of EM can often lead to suboptimal solutions.}
##'
##'        \item{CEM}{\eqn{10} repetitions of \eqn{50} iterations of the CEM algorithm are done.
##'        One advantage of initializing an algorithm with CEM lies in the fact
##'        that CEM converges generally in a small number of iterations. Thus,
##'        without consuming a large amount of CPU times, several runs of CEM are
##'        performed. Then EM is run with the best solution among the \eqn{10} repetitions.}
##'
##'        \item{SEMMax}{A run of \eqn{500} iterations of SEM. The idea is that an SEM sequence is
##'        expected to enter rapidly in the neighbourhood of the global maximum
##'        of the likelihood function.}
##'
##'    }
##'
##' Defining the algorithms used in the strategy, the stopping rule and when to stop.
##'    \itemize{
##'        \item Algorithms :
##'           \describe{
##'               \item{EM}{Expectation Maximisation}
##'               \item{CEM}{Classification EM}
##'               \item{SEM}{Stochastic EM}
##'           }
##'        \item Stopping rules for the algorithm :
##'           \describe{
##'               \item{nbIterationInAlgo}{Sets the maximum number of iterations}
##'               \item{epsilonInAlgo}{Sets relative increase of the log-likelihood criterion}
##'           }
##'        \item Default values are \eqn{200} \code{nbIterationInAlgo} of \code{EM} with an \code{epsilonInAlgo} value of \eqn{10-3}.
##'    }
##' 
##' @param name character string with the estimation algorithm.  Possible values: "EM", "SEM", "CEM". Default value is "EM".
##' @param nbTry integer defining the number of tries. nbTry must be a positive integer. Option available only if \code{init} is "random" or "smallEM" or "CEM" or "SEMMax". Default value: 1.
##' @param initMethod a character string with the method of initialization of the algorithm specified in the \code{name} argument. Possible values: "random", "smallEM", "CEM", "SEMMax". Default value: "smallEM".
##' @param nbTryInInit integer defining number of tries in \code{initMethod} algorithm. nbTryInInit must be a positive integer. Option available only if \code{init} is "smallEM" or "CEM". Default value: 50.
##' @param nbIterationInInit integer defining the number of "EM" or "SEM" iterations in \code{initMethod}. nbIterationInInit must be a positive integer. Only available if \code{initMethod} is "smallEM" or "SEMMax". Default values: 5 if \code{initMethod} is "smallEM" and 100 if \code{initMethod} is "SEMMax".
##' @param nbIterationInAlgo integer defining the number of iterations if you want to use nbIteration as rule to stop the algorithm(s). Default value: 200. 
##' @param epsilonInInit real defining the epsilon value in the initialization step. Only available if \code{initMethod} is "smallEM". Default value: 0.001.
##' @param epsilonInAlgo real defining the epsilon value for the algorithm. Warning: epsilonInAlgo doesn't have any sens if \code{name} is SEM. Default value: 0.001.
##'
##' @examples
##'    mixmodAlgo()
##'    mixmodAlgo(name="CEM",initMethod="random",nbTry=10,epsilonInInit=0.00001)
##'
##' @references  Biernacki, C., Celeux, G., Govaert, G., 2003. "Choosing starting values for the EM algorithm for getting the highest likelihood in multivariate gaussian mixture models". Computational Statistics and Data Analysis 41, 561-575.
##'
##' @return an object of [\code{\linkS4class{Algo}}]
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @export
##'
mixmodAlgo <- function( name="EM", nbTry=1, initMethod="smallEM", nbTryInInit=50, nbIterationInInit=5, nbIterationInAlgo=200, epsilonInInit=0.001, epsilonInAlgo=0.001 ){
  # create a new class Algo
  new("Algo", name=name, nbTry=nbTry, initMethod=initMethod, nbTryInInit=nbTryInInit, nbIterationInInit=nbIterationInInit, nbIterationInAlgo=nbIterationInAlgo, epsilonInInit=epsilonInInit, epsilonInAlgo=epsilonInAlgo)
}
###################################################################################


###################################################################################
##' Constructor of [\code{\linkS4class{Algo}}] class
##'
##' @examples
##'   new("Algo")
##'   new("Algo", name="SEM", initMethod="SEMMax")
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name Algo-class
##' @rdname Algo-class
##' @exportClass Algo
##'
setClass(
    Class="Algo",
    representation=representation(
        name = "character",
        nbTry = "numeric",
        initMethod = "character",
        nbTryInInit = "numeric",
        nbIterationInInit = "numeric",
        nbIterationInAlgo = "numeric",
        epsilonInInit = "numeric",
        epsilonInAlgo = "numeric"
    ),
    prototype=prototype(
        name = "EM",
        nbTry = 1,
        initMethod = "smallEM",
        nbTryInInit = 50,
        nbIterationInInit = 5,
        nbIterationInAlgo = 200,
        epsilonInInit = 0.001,
        epsilonInAlgo = 0.001
    ),
    # validity function
    validity=function(object){
      # for name
      if( (object@name != "EM") & (object@name != "SEM") & (object@name != "CEM") ){
         stop("name is not valid")
      } 
      # for 'initMethod'
      if( (object@initMethod != "smallEM") & (object@initMethod != "random") & (object@initMethod != "CEM") & (object@initMethod != "SEMMax") ){
        stop("initMethod name is not valid")
      }
      # for 'nbTry'
      if (!is.wholenumber(object@nbTry)) stop("nbTry must be an integer")
      if (object@nbTry < 1) stop("nbTry must be positive")
      # for 'nbTryInInit'
      if ( (object@initMethod == "smallEM") | (object@initMethod == "CEM") ){
        if (!is.wholenumber(object@nbTryInInit)) stop("nbTryInInit must be an integer")
        if (object@nbTryInInit < 1) stop("nbTryInInit must be positive")
      }
      # for 'epsilonInInit'
      if ( (object@initMethod == "smallEM") ){
          if (!is.double(object@epsilonInInit)) stop("epsilonInInit must be a real")
          if ( (object@epsilonInInit > 1) | (object@epsilonInInit < 0) ) stop("epsilonInInit must be less than one and positive")
      } 
      # for 'nbIterationInInit'
      if ( (object@initMethod == "smallEM") | (object@initMethod == "SEMMax") ){
        if (!is.wholenumber(object@nbIterationInInit)) stop("nbIterationInInit must be an integer")
        if(object@nbIterationInInit < 1) stop("nbIterationInInit must be positive")
        # change default value if necessary
        if ( (object@initMethod == "SEMMax") & (object@nbIterationInInit == 5) ) object@nbIterationInInit <- 100
      }
      # for 'nbIterationInAlgo'
      if (!is.wholenumber(object@nbIterationInAlgo)) stop("nbIterationInAlgo must be an integer")
      if (object@nbIterationInAlgo < 1) stop("nbIterationInAlgo must be positive")
      # for 'epsilonInAlgo'
      if ( (object@initMethod != "SEM") ){
        if (!is.double(object@epsilonInAlgo)) stop("epsilonInAlgo must be a real")
        if( (object@epsilonInAlgo > 1) | (object@epsilonInAlgo < 0) ) stop("epsilonInAlgo must be less than one and positive")
      }
      return(TRUE)
    }
)
###################################################################################


###################################################################################
##' Print the Algo class to standard output.
##'
##' @param x A \code{Algo} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,Algo-method
##' @docType methods
##' @rdname print-methods
##' @export
##'
##' @seealso \code{\link{print}}
##' @examples
##' algo <- mixmodAlgo()
##' print(algo)
##' getMethod("print", "Algo")
##'
setMethod(
  f="print",
  signature=c("Algo"),
  function(x,...){
    cat("************************************\n")
    cat("*** MIXMOD Algorithm :\n")
    cat("* algorithm            = ", x@name, "\n")
    cat("* number of tries      = ", x@nbTry, "\n")
    cat("* number of iterations = ", x@nbIterationInAlgo, "\n")
    cat("* epsilon              = ", x@epsilonInAlgo, "\n")
    cat("*** Initialization strategy:\n")
    cat("* algorithm            = ", x@initMethod, "\n")
    cat("* number of tries      = ", x@nbTryInInit, "\n")
    cat("* number of iterations = ", x@nbIterationInInit, "\n")
    cat("* epsilon              = ", x@epsilonInInit, "\n")
    cat("************************************\n")
  }
)
###################################################################################


###################################################################################
##' Show description of the Algo class to standard output.
##'
##' @param object A \code{Algo} object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,Algo-method
##' @docType methods
##' @rdname show-methods
##' @export
##'
##' @seealso \code{\link{show}}
##' @examples
##' algo <- mixmodAlgo()
##' algo
##' show(algo)
##' getMethod("show", "Algo")
##'
setMethod(
  f="show",
  signature=c("Algo"),
  function(object){
    cat("************************************\n")
    cat("*** MIXMOD Algorithm :\n")
    cat("* algorithm            = ", object@name, "\n")
    cat("* number of tries      = ", object@nbTry, "\n")
    cat("* number of iterations = ", object@nbIterationInAlgo, "\n")
    cat("* epsilon              = ", object@epsilonInAlgo, "\n")
    cat("*** Initialization strategy:\n")
    cat("* algorithm            = ", object@initMethod, "\n")
    cat("* number of tries      = ", object@nbTryInInit, "\n")
    cat("* number of iterations = ", object@nbIterationInInit, "\n")
    cat("* epsilon              = ", object@epsilonInInit, "\n")
    cat("************************************\n")
  }
)
###################################################################################

