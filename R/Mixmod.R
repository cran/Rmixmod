##############################################################
##                        Mixmod.R                          ##
##############################################################

###################################################################################
##' @include Model.R
##' @include Parameter.R
##' @include Algo.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{Mixmod}}] class
##'
##' This is a class to interface mixmod kernel with R.
##'  
##' \describe{
##'
##'   \item{data}{numeric vector, matrix, or data frame of observations. Either qualitative or quantitative.}
##'
##'   \item{nbCluster}{integer. It indicates the number of classes.}
##'
##'   \item{nbSample}{integer. The number of observations.}
##'
##'   \item{nbVariable}{integer. The number of variables.}
##'
##'   \item{weight}{numeric vector with n (number of individuals) rows. Weight is optionnal. This option is to be used when weight is associated to the data.}
##'
##'   \item{dataType}{character. It defines whether data are quantitative or qualitative.}
##'
##'   \item{criterion}{character. This option permits to select the criterion giving the best configuration of an execution.}
##'
##'   \item{models}{a S4 [\code{\linkS4class{Model}}] object. Defining the list of models to be tested.}
##'
##'   \item{algo}{a S4 [\code{\linkS4class{Algo}}] object. Defining the algorithm used in the strategy for the models selection.}
##'
##'   \item{parameters}{a S4 [\code{\linkS4class{Parameter}}] object. The best model parameters.}
##'
##'   \item{likelihood}{numeric. The best model likelihood.}
##'
##'   \item{bestModel}{character. Name of the best model.}
##'
##'   \item{criterionValue}{numeric. Value of the criterion used to select the best model.}
##'
##' }
##'
##' @examples
##' getSlots("Mixmod")
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name Mixmod-class
##' @rdname Mixmod-class
##' @exportClass Mixmod
##'
setClass(
    Class="Mixmod",
    representation=representation(
        data = "matrix",
        nbCluster = "numeric",
        nbSample = "integer",
        nbVariable = "integer",
        weight = "numeric",
        dataType = "character",
        criterion = "character",
        models = "Model",
        algo = "Algo",
        parameters = "Parameter",
        likelihood = "numeric",
        bestModel = "character",
        criterionValue = "numeric",
        "VIRTUAL"
    ),
    prototype=prototype(
        data = matrix(nrow=0,ncol=0),
        nbCluster = numeric(0),
        nbSample = integer(0),
        nbVariable = integer(0),
        weight = numeric(0),
        dataType = character(0),
        likelihood = numeric(0),
        bestModel = character(0),
        criterion = character(0),
        criterionValue = numeric(0)
    ),
    # validity function
    validity=function(object){
        # check if weight isn't null and equal to the number of subjects
        if(length(object@weight)>0){
            if(length(object@weight) != nbSample){
                stop("weight must be equal to the number of sample !")            
            }
        }
        return(TRUE)
    }
)
###################################################################################


###################################################################################
##' Print the class to standard output.
##'
##' @param x An object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,Mixmod-method
##' @docType methods
##' @rdname print-methods
##'
setMethod(
    f="print",
    signature=c("Mixmod"),
    function(x,...){
        cat("****************************************\n")
        cat("*** INPUT:\n")
        print(x@algo)
        cat("* nbCluster = ", x@nbCluster, "\n")
        cat("* criterion = ", x@criterion, "\n")
        print(x@models)
        if(length(x@weight)!=0){ 
          cat("* weight  = ", x@weight, "\n")
          }
        if(length(x@data)!=0){
          cat("* data      =\n")
          print(x@data)
        }
        else{}
        if(length(x@bestModel)!=0){
          cat("\n\n****************************************\n")
          cat("*** OUTPUT:\n")
          cat("* bestModel      = ", x@bestModel, "\n")
          cat("* criterionValue = ", formatC(x@criterionValue,digits=4,format="f"), "\n")
          cat("* likelihood     = ", formatC(x@likelihood,digits=4,format="f"), "\n")
        }
        cat("****************************************\n")
    }
)
###################################################################################


###################################################################################
##' Show description of the class to standard output.
##'
##' @param object An object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,Mixmod-method
##' @docType methods
##' @rdname show-methods
##'
setMethod(
    f="show",
    signature=c("Mixmod"),
    function(object){
        cat("****************************************\n")
        cat("*** INPUT:\n")
        show(object@algo)
        cat("* nbCluster = ", object@nbCluster, "\n")
        cat("* criterion = ", object@criterion, "\n")
        show(object@models)
        if(length(object@weight)!=0){ 
          cat("* weight  = ", x@weight, "\n")
        }
        if(length(object@data)!=0){
            nrowShow <- min(10,nrow(object@data))
            ncolShow <- min(10,ncol(object@data))
            cat("* data (limited to a 10x10 matrix) =\n")
            print(formatC(object@data[1:nrowShow,1:ncolShow]),quote=FALSE)
        }else{}
        cat("* ... ...\n")
        if(length(object@bestModel)!=0){
          cat("\n\n****************************************\n")
          cat("*** OUTPUT:\n")
          cat("* bestModel      = ", object@bestModel, "\n")
          cat("* criterionValue = ", formatC(object@criterionValue,digits=4,format="f"), "\n")
          cat("* likelihood     = ", formatC(object@likelihood,digits=4,format="f"), "\n")
        }
        cat("****************************************\n")
    }
)
###################################################################################
