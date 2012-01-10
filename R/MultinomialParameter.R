###################################################################################
##                        MultinomialParameter.R                                 ##
###################################################################################

###################################################################################
##' @include Parameter.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{MultinomialParameter}}] class
##' 
##' This class defines parameters of a Multinomial Mixture Model. Inherits the [\code{\linkS4class{Parameter}}] class.
##' 
##' \describe{
##'
##'   \item{center}{a numeric vector containing center of each cluster.}
##'
##'   \item{scatter}{a vector of matrix containing dispersion matrix of each cluster.}
##'
##'   \item{factor}{a character vector containing the modalities.}
##'
##' }
##'
##' @examples
##'   new("MultinomialParameter")
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name MultinomialParameter-class
##' @rdname MultinomialParameter-class
##' @exportClass MultinomialParameter
##'
setClass(
    Class="MultinomialParameter",
    representation=representation(
        center = "numeric",
        scatter = "numeric",
        factor = "factor"
    ),
    prototype=prototype(
        center = numeric(0),
        scatter =  numeric(0),
        factor = factor()
    ),
    contains=c("Parameter")
)
###################################################################################



###################################################################################
##' Print the MultinomialParameter class to standard output.
##'
##' @param x A \code{MultinomialParameter} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,MultinomialParameter-method
##' @docType methods
##' @rdname print-methods
##' @export
##'
##' @seealso \code{\link{print}}
##' @examples
##' data(birds)
##' xem <- mixmodClustering(birds)
##' print(xem@@parameters)
##' getMethod("print", "MultinomialParameter")
##'
setMethod(
    f="print",
    signature=c("MultinomialParameter"),
    function(x,...){
      if(length(x@proportions)>0){
        cat("************************************\n")
        cat("* modalities   = ", x@factor, "\n")
        for(i in 1:length(x@proportions)){
          cat("*** Cluster",i,"\n")
          cat("* proportion = ", formatC(x@proportions[i],digits=4,format="f"), "\n")
          cat("* center     = ", formatC(x@center[i,],digits=4,format="f"), "\n")
          if ( length(x@scatter) > 1 ){            
            if( length(dim(x@scatter)) == 2 ){
              cat("* scatter    = ", formatC(x@scatter[i,],digits=4,format="f"), "\n")
            }else if( length(x@scatter[[i]]) > 1 ){          
              if ( nrow(x@scatter[[i]])>1 ){
                cat("* scatter    = |",formatC(x@scatter[[i]][1,],digits=4,width=10,format="f"),"|\n")
                for ( j in 2:nrow(x@scatter[[i]])){
                  cat("               |", formatC(x@scatter[[i]][j,],digits=4,width=10,format="f"),"|\n")
                }
              }else{
                cat("* scatter    = ",formatC(x@scatter[[i]],digits=4,format="f"),"\n")
              }
            }else{
              cat("* scatter    = ", formatC(x@scatter[i],digits=4,format="f"), "\n")
            }
          }else{
            cat("* scatter    = ", formatC(x@scatter,digits=4,format="f")," \n")
          }
        }
        cat("************************************\n")
      }
    }
)
###################################################################################


###################################################################################
##' Show description of the MultinomialParameter class to standard output.
##'
##' @param object A \code{MultinomialParameter} object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,MultinomialParameter-method
##' @docType methods
##' @rdname show-methods
##' @export
##'
##' @seealso \code{\link{show}}
##' @examples
##' data(birds)
##' xem <- mixmodClustering(birds)
##' show(xem@@parameters)
##' getMethod("show", "MultinomialParameter")
##'
setMethod(
    f="show",
    signature=c("MultinomialParameter"),
    function(object){
      if(length(object@proportions)>0){
        cat("************************************\n")
        cat("* number of modalities = ", object@factor, "\n")
        for(i in 1:length(object@proportions)){
          cat("*** Cluster",i,"\n")
          cat("* proportion = ", formatC(object@proportions[i],digits=4,format="f"), "\n")
          cat("* center     = ", formatC(object@center[i,],digits=4,format="f"), "\n")
          if ( length(object@scatter) > 1 ){
            if( length(dim(object@scatter)) == 2 ){
              cat("* scatter    = ", formatC(object@scatter[i,],digits=4,format="f"), "\n")
            }else if( length(object@scatter[[i]]) > 1 ){
              if ( nrow(object@scatter[[i]])>1 ){
                cat("* scatter    = |",formatC(object@scatter[[i]][1,1:object@factor[1]],digits=4,width=10,format="f"),"|\n")
                for ( j in 2:nrow(object@scatter[[i]])){
                  cat("               |", formatC(object@scatter[[i]][j,1:object@factor[j]],digits=4,width=10,format="f"),"|\n")
                }
              }else{
                cat("* scatter    = ",formatC(object@scatter[[i]],digits=4,format="f"),"\n")
              }
            }else{
              cat("* scatter    = ", formatC(object@scatter[i],digits=4,format="f"), "\n")
            }
          }else{
            cat("* scatter    = ", formatC(object@scatter,digits=4,format="f")," \n")
          }
        }
        cat("************************************\n")
      }
    }
)

###################################################################################
##' Produce result summaries of a class [\code{\linkS4class{MultinomialParameter}}] 
##'
##' @param x A \code{MultinomialParameter} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Summaries to standard out.
##'
##' @name summary
##' @aliases summary summary,MultinomialParameter-method
##' @docType methods
##' @rdname summary-methods
##' @export
##'
##' @seealso \code{\link{summary}}
##' @examples
##' data(birds)
##' xem <- mixmodClustering(birds)
##' summary(xem@@parameters)
##' getMethod("summary", "MultinomialParameter")
##'
setMethod(
  f="summary",
  signature=c("MultinomialParameter"),
  function(object, ...){
    if(length(object@proportions)>0){
      for ( k in 1:length(object@proportions) ){
        cat("*                  Cluster ", k,": \n")
        cat("                         Proportion = ",formatC(object@proportions[k],digits=4,format="f"),"\n")
        cat("                             Center = ",formatC(object@center[k,],digits=4,format="f"),"\n")
        if ( length(object@scatter) > 1 ){
          if( length(dim(object@scatter)) == 2 ){
            cat("                            Scatter = ", formatC(object@scatter[k,],digits=4,format="f"), "\n")
          }else if( length(object@scatter[[k]]) > 1 ){
            if ( nrow(object@scatter[[k]])>1 ){
              cat("                            Scatter = |",formatC(object@scatter[[k]][1,],digits=4,width=10,format="f"),"|\n")
              for ( j in 2:nrow(object@scatter[[k]])){
                cat("                                      |", formatC(object@scatter[[k]][j,],digits=4,width=10,format="f"),"|\n")
              }
            }else{
              cat("                            Scatter = ",formatC(object@scatter[[k]],digits=4,format="f"),"\n")
            }
          }else{
            cat("                            Scatter = ", formatC(object@scatter[k],digits=4,format="f"), "\n")
          }
        }else{
          cat("                            Scatter = ", formatC(object@scatter,digits=4,format="f")," \n")
        }
      }
    }
  }
)
###################################################################################


###################################################################################
##' Extract parts of a [\code{\linkS4class{MultinomialParameter}}] class
##'
##' @name [
##' @aliases [,MultinomialParameter,ANY,numeric-method
##' @docType methods
##' @rdname extract-methods
##'
setMethod(
  f="[", 
  signature(x = "MultinomialParameter", i = "ANY", j="numeric"),
  definition=function(x,i,j,drop){
    if ( missing(j) ){
      switch(EXPR=i,
        "proportions"={return(x@proportions)},
        "center"={return(x@center)},
        "scatter"={return(x@scatter)},
        stop("This attribute doesn't exist !")
      )
    }else{
        switch(EXPR=i,
        "proportions"={return(x@proportions[j])},
        "center"={return(x@center[j,])},
        "scatter"={return(x@scatter[j])},
        stop("This attribute doesn't exist !")
        )
    }
  }
)
###################################################################################


###################################################################################
##' Replace names of [\code{\linkS4class{MultinomialParameter}}] class
##'
##' @name [
##' @aliases [<-,MultinomialParameter-method [<-,MultinomialParameter,ANY-method
##' @docType methods
##' @rdname extract-methods
##'
setReplaceMethod(
  f="[", 
  signature(x = "MultinomialParameter", value = "ANY"), 
  definition=function(x,i,j,drop){
    if ( missing(j) ){
      switch(EXPR=i,
      "proportions"={x@proportions<-value},
      "center"={x@center<-value},
      "scatter"={x@scatter<-value},
      stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
      "proportions"={x@proportions[j]<-value},
      "center"={x@center[j,]<-value},
      "scatter"={x@scatter[j]<-value},
      stop("This attribute doesn't exist !")
      )
    }
    validObject(x)
    return(x)
  }
)
###################################################################################

