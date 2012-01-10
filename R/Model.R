###################################################################################
##                                   Model.R                                     ##
###################################################################################


###################################################################################
##' Constructor of [\code{\linkS4class{Model}}] class
##'
##' This class defines the Mixmod models.
##'
##' \describe{
##'
##'   \item{listModels}{character containing a list of models.}
##'
##'   \item{free.proportions}{logical to include models with free proportions. Default is TRUE.}
##'
##'   \item{equal.proportions}{logical to include models with equal proportions. Default is FALSE.}
##'
##' }
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name Model-class
##' @rdname Model-class
##' @exportClass Model
##'
setClass(
    Class="Model",
    representation=representation(
        listModels = "character",
        free.proportions = "logical",
        equal.proportions = "logical",
        "VIRTUAL"
    ),
    prototype=prototype(
        listModels = character(0),
        free.proportions = logical(0),
        equal.proportions = logical(0)
    )
)
###################################################################################


###################################################################################
##' Print the Model class to standard output.
##'
##' @param x A \code{Model} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,Model-method
##' @docType methods
##' @rdname print-methods
##' @export
##'
##' @seealso \code{\link{print}}
##' @examples
##' gaussianModel <- mixmodGaussianModel()
##' print(gaussianModel)
##'
##' multinomialModel <- mixmodMultinomialModel()
##' print(multinomialModel)
##'
setMethod(
  f="print",
  signature=c("Model"),
  function(x,...){
    cat("************************************\n")
    cat("*** MIXMOD Models :\n")
    cat("* list = ", x@listModels, "\n")
    if ( x@free.proportions  & x@equal.proportions )
      cat("* This list includes models with free and equal proportions.\n")
    else if ( x@free.proportions  & !x@equal.proportions )
      cat("* This list includes only models with free proportions.\n")
    else if ( !x@free.proportions  & x@equal.proportions )
      cat("* This list includes only models with equal proportions.\n")
    cat("************************************\n")
  }
)
###################################################################################


###################################################################################
##' Show description of the Model class to standard output.
##'
##' @param object A \code{Model} object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,Model-method
##' @docType methods
##' @rdname show-methods
##' @export
##'
##' @seealso \code{\link{show}}
##' @examples
##' gaussianModel <- mixmodGaussianModel()
##' gaussianModel
##' show(gaussianModel)
##'
##' multinomialModel <- mixmodMultinomialModel()
##' multinomialModel
##' show(multinomialModel)
##'
setMethod(
  f="show",
  signature=c("Model"),
  function(object){
    cat("************************************\n")
    cat("*** MIXMOD Models :\n")
    cat("* list = ", object@listModels, "\n")
    if ( object@free.proportions  & object@equal.proportions )
    cat("* This list includes models with free and equal proportions.\n")
    else if ( object@free.proportions  & !object@equal.proportions )
    cat("* This list includes only models with free proportions.\n")
    else if ( !object@free.proportions  & object@equal.proportions )
    cat("* This list includes only models with equal proportions.\n")
    cat("************************************\n")
  }
)
###################################################################################

