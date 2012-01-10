###################################################################################
##                                  Parameter.R                                  ##
###################################################################################


###################################################################################
##' Constructor of [\code{\linkS4class{Parameter}}] class
##'
##' This class defines parameters of a Mixture Model.
##'
##' \describe{
##'
##'   \item{proportions}{a numeric vector containing proportions of the mixture model.}
##'
##' }
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name Parameter-class
##' @rdname Parameter-class
##' @exportClass Parameter
##'
setClass(
    Class="Parameter",
    representation=representation(
        proportions = "numeric",
        "VIRTUAL"
    ),
    prototype=prototype(
        proportions = numeric(0)
    )
)
###################################################################################
