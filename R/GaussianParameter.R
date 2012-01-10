###################################################################################
##                             GaussianParameter.R                               ##
###################################################################################

###################################################################################
##' @include Parameter.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{GaussianParameter}}] class
##' 
##' This class defines parameters of a Gaussian Mixture Model. Inherits the [\code{\linkS4class{Parameter}}] class.
##' 
##' \describe{
##'
##'   \item{mean}{a numeric vector containing mean of each cluster.}
##'
##'   \item{variance}{a vector of matrix containing variance matrix of each cluster.}
##'
##' }
##'
##' @examples
##'   new("GaussianParameter")
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name GaussianParameter-class
##' @rdname GaussianParameter-class
##' @exportClass GaussianParameter
##'
setClass(
    Class="GaussianParameter",
    representation=representation(
        mean = "numeric",
        variance = "list"
    ),
    prototype=prototype(
        mean = numeric(0),
        variance = list(0)
    ),
    contains=c("Parameter")
)
###################################################################################


###################################################################################
##' Print the GaussianParameter class to standard output.
##'
##' @param x A \code{GaussianParameter} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,GaussianParameter-method
##' @docType methods
##' @rdname print-methods
##' @export
##'
##' @seealso \code{\link{print}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' print(xem@@parameters)
##' getMethod("print", "GaussianParameter")
##'
setMethod(
    f="print",
    signature=c("GaussianParameter"),
    function(x,...){
      if(length(x@proportions)>0){
        cat("************************************\n")
        for(k in 1:length(x@proportions)){
          cat("*** Cluster",k,"\n")
          cat("* proportion = ", formatC(x@proportions[k],digits=4,format="f"), "\n")
          cat("* means      = ", formatC(x@mean[k,],digits=4,format="f"), "\n")
          if ( nrow(x@variance[[k]])>1 ){
            cat("* variances = |",formatC(x@variance[[k]][1,],digits=4,width=10,format="f"),"|\n")
            for ( i in 2:nrow(x@variance[[k]])){
              cat("              |", formatC(x@variance[[k]][i,],digits=4,width=10,format="f"),"|\n")
            }
          }else{
            cat("* variances = ",formatC(object@variance[[k]],digits=4,format="f"),"\n")
          }
        }
        cat("************************************\n")
      }
    }
)
###################################################################################


###################################################################################
##' Show description of the GaussianParameter class to standard output.
##'
##' @param object A \code{GaussianParameter} object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,GaussianParameter-method
##' @docType methods
##' @rdname show-methods
##' @export
##'
##' @seealso \code{\link{show}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' show(xem@@parameters)
##' getMethod("show", "GaussianParameter")
##'
setMethod(
    f="show",
    signature=c("GaussianParameter"),
    function(object){
      if(length(object@proportions)>0){
        cat("************************************\n")
        for(k in 1:length(object@proportions)){
          cat("*** Cluster",k,"\n")
          cat("* proportion = ", formatC(object@proportions[k],digits=4,format="f"), "\n")
          cat("* means      = ", formatC(object@mean[k,],digits=4,format="f"), "\n")
          if ( nrow(object@variance[[k]])>1 ){
            cat("* variances = |",formatC(object@variance[[k]][1,],digits=4,width=10,format="f"),"|\n")
            for ( i in 2:nrow(object@variance[[k]])){
              cat("              |", formatC(object@variance[[k]][i,],digits=4,width=10,format="f"),"|\n")
            }
          }else{
            cat("* variances = ",formatC(object@variance[[k]],digits=4,format="f"),"\n")
          }
        }
        cat("************************************\n")
      }
    }
)
###################################################################################


###################################################################################
##' Produce result summaries of a class [\code{\linkS4class{GaussianParameter}}] 
##'
##' @param x A \code{GaussianParameter} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Summaries to standard out.
##'
##' @name summary
##' @aliases summary summary,GaussianParameter-method
##' @docType methods
##' @rdname summary-methods
##' @export
##'
##' @seealso \code{\link{summary}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' summary(xem@@parameters)
##' getMethod("summary", "GaussianParameter")
##'
setMethod(
  f="summary",
  signature=c("GaussianParameter"),
  function(object, ...){
    if(length(object@proportions)>0){
      for ( k in 1:length(object@proportions) ){
        cat("*                  Cluster ", k,": \n")
        cat("                         Proportion = ",formatC(object@proportions[k],digits=4,format="f"),"\n")
        cat("                              Means = ",formatC(object@mean[k,],digits=4,format="f"),"\n")
        if ( nrow(object@variance[[k]])>1 ){
          cat("                          Variances = |",formatC(object@variance[[k]][1,],digits=4,width=10,format="f"),"|\n")
          for ( i in 2:nrow(object@variance[[k]])){
            cat("                                      |", formatC(object@variance[[k]][i,],digits=4,width=10,format="f"),"|\n")
          }
        }else{
          cat("                          Variances = ",formatC(object@variance[[k]],digits=4,format="f"),"\n")
        }
      }
    }
  }
)
###################################################################################


###################################################################################
##' Extract parts of a [\code{\linkS4class{GaussianParameter}}] class
##'
##' @name [
##' @aliases [,GaussianParameter,ANY,numeric-method
##' @docType methods
##' @rdname extract-methods
##'
setMethod(
  f="[", 
  signature(x = "GaussianParameter", i = "ANY", j="numeric"),
  definition=function(x,i,j,drop){
    if ( missing(j) ){
      switch(EXPR=i,
        "proportions"={return(x@proportions)},
        "mean"={return(x@mean)},
        "variance"={return(x@variance)},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "proportions"={return(x@proportions[j])},
        "mean"={return(x@mean[j,])},
        "variance"={return(x@variance[[j]])},
        stop("This attribute doesn't exist !")
      )
    }
  }
)
###################################################################################


###################################################################################
##' Replace names of [\code{\linkS4class{GaussianParameter}}] class
##'
##' @name [
##' @aliases [<-,GaussianParameter-method [<-,GaussianParameter,ANY-method
##' @docType methods
##' @rdname extract-methods
##'
setReplaceMethod(
  f="[", 
  signature(x = "GaussianParameter", value = "ANY"), 
  definition=function(x,i,j,drop){
    if ( missing(j) ){
      switch(EXPR=i,
        "proportions"={x@proportions<-value},
        "mean"={x@mean<-value},
        "variance"={x@variance<-value},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "proportions"={x@proportions[j]<-value},
        "mean"={x@mean[j,]<-value},
        "variance"={x@variance[[j]]<-value},
        stop("This attribute doesn't exist !")
        )
    }
    validObject(x)
    return(x)
  }
)
###################################################################################

