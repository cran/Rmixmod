###################################################################################
##                               MixmodClustering.R                              ##
###################################################################################

###################################################################################
##' @include Mixmod.R
##' @include Model.R
##' @include Parameter.R
##' @include Algo.R
NULL
###################################################################################


###################################################################################
##' Constructor of [\code{\linkS4class{MixmodClustering}}] class
##' 
##' This is a class to run clustering with mixmod. Inherits the [\code{\linkS4class{Mixmod}}] class.
##'
##' \describe{
##'
##'   \item{nbClusterInBestModel}{integer. Number of class for the best model.}
##'
##'   \item{knownPartition}{matrix. This option is to be used when a partition of data is already known.}
##'
##'   \item{labels}{vector of integers.}
##'
##'   \item{proba}{a matrix of probabilities.}
##'
##' }
##'
##' @examples
##'   ## A quantitative example with the famous iris data set
##'   data(iris)
##'   ## with default values
##'   new("MixmodClustering", data=as.matrix(iris[1:4]))
##'
##'   getSlots("Mixmod")
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name MixmodClustering-class
##' @rdname MixmodClustering-class
##' @exportClass MixmodClustering
##'
setClass(
  Class="MixmodClustering",
  representation=representation(
    nbClusterInBestModel = "integer",
    knownPartition = "matrix",
    labels = "integer",
    proba = "matrix"
  ),
  contains=c("Mixmod"),
  prototype=prototype(
    nbClusterInBestModel = integer(0),
    knownPartition = matrix(nrow=0,ncol=0),
    labels = integer(0),
    proba = matrix(nrow=0,ncol=0)
  ),
  # define validity function
  validity=function(object){
    # test if there exists known partitions
    if (length(object@knownPartition)>0){
      if (length(object@nbCluster) > 1){
        warning("more than one cluster, knownPartition will be ignored")
        object@knownPartition = NULL
      }else if ( (nrow(object@knownPartition)!= object@nbSample) | ( ncol(object@knownPartition) != object@nbCluster) ){
        warning("knownPartition is not a [nbSample,nbCluster] matrix, knownPartition will be ignored")
        object@knownPartition = NULL
      }
    }
    # check criterion parameter
    if ( (object@criterion!="BIC") & (object@criterion!="ICL") & (object@criterion!="NEC") ){
      stop(cat(object@criterion, "is not a valid criterion name !\n"))
    }
    # check parameters validity
    if ( (object@dataType == "quantitative") & is(object@parameters, "MultinomialParameter") ){
      stop("MultinomialParameter can't suit for quantitative data !\n")
    }
    # check parameters validity
    if ( (object@dataType == "qualitative") & is(object@parameters, "GaussianParameter") ){
      stop("GaussianParameter can't suit for qualitative data !\n")
    }
    return(TRUE)
  }
)
###################################################################################

###################################################################################
##' Create an instance of the [\code{\linkS4class{MixmodClustering}}] class
##'
##' This function computes an optimal mixture model according to the criteria furnished, 
##' and the list of model defined in [\code{\linkS4class{Model}}], using the algorithm specified in [\code{\linkS4class{Algo}}].
##' 
##' @param data matrix containing quantitative or qualitative data. Rows correspond to observations and columns correspond to variables.
##' @param dataType character. It defines whether data are quantitative or qualitative. Possible values are "quantitative" or "qualitative". If missing, it will be guessed.
##' @param nbCluster numeric listing the number of custers. Default is a list from 2 to 8 clusters.
##' @param models a [\code{\linkS4class{Model}}] object defining the list of models to run.
##' @param algo a [\code{\linkS4class{Algo}}] object containing the algorithm to run. Call mixmodAlgo() method by default.
##' @param criterion character defining the criterion to select the best model. Possible values: "BIC", "ICL", "NEC". Default is "BIC".
##' @param weight numeric vector with n (number of individuals) rows. Weight is optionnal. This option is to be used when weight is associated to the data.
##' @param knownPartition matrix of size nbSample*nbCluster. it will be used for semi-supervised classification when labels are known. One line with only values of zero will designate a sample for who label isn't known.
##'
##' @examples
##'   ## A quantitative example with the famous geyser data set
##'   data(geyser)
##'   ## with default values
##'   mixmodClustering(geyser)
##'
##'   ## An example with the famous titanic data set
##'   data(birds)
##'   mixmodClustering(data=birds, criterion= "BIC", model = mixmodMultinomialModel(), nbCluster = 2:8)
##'
##'   ## use graphics functions
##'   xem <- mixmodClustering(geyser)
##'   plot(xem)
##'   hist(xem)
##'
##'   ## get summary
##'   summary(xem)
##'
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @return Returns an instance of the [\code{\linkS4class{MixmodClustering}}] class.
##' @export
##'
mixmodClustering <- function(data, dataType, nbCluster=2:8, models, algo=mixmodAlgo(), criterion="BIC", weight, knownPartition) {
  # check options
  if(!missing(data)){
    # if dataType is missing, try to guess whether data are quantitative or qualitative
    if(missing(dataType)){
      nbModality <- max( apply( as.matrix(data), 2, function(x) { nlevels(as.factor(x)) } ) / nrow(as.matrix(data)) )
      if ( nbModality < 0.25 ) dataType <- "qualitative"
      else dataType <- "quantitative"
    }
    if( dataType == "quantitative" ){
      if(missing(models)) models = new("GaussianModel")
    }else if ( dataType == "qualitative" ){
      if(missing(models)) models = new("MultinomialModel")
    }else{
      stop("unknown dataType --> dataType must be 'quantitative' or 'qualitative'!")
    }
  }else{
    stop("data is missing !")
  } 
  if(missing(weight)){
    weight <- numeric(0)
  }
  if(missing(knownPartition)){
    knownPartition <- matrix(nrow=0,ncol=0)
  }
  # create Mixmod object
  xem <- new( "MixmodClustering", data, dataType, nbCluster, models, algo, criterion, weight, knownPartition)
  # call clusteringMain
  .Call("clusteringMain", xem, PACKAGE="Rmixmod")
  
  # return MixmodClustering object
  return(xem)
}
###################################################################################


###################################################################################
##' Create an instance of the [\code{\linkS4class{MixmodClustering}}] class using new/initialize.
##' 
##' Initialization method. Used internally in the `Rmixmod' package.
##' 
##' @seealso \code{\link{initialize}}
##'
##' @keywords internal
##'
##' @rdname initialize-methods
##'
setMethod(
    f="initialize",
    signature=c("MixmodClustering"),
    definition=function(.Object,data,dataType,nbCluster,models,algo,criterion,weight,knownPartition){
        if(!missing(data)){
            # if dataType is missing, try to guess whether data are quantitative or qualitative
            if(missing(dataType)){
              nbModality <- max( apply( as.matrix(data), 2, function(x) { nlevels(as.factor(x)) } ) / nrow(as.matrix(data)) )
              # if the number of modality is greater than 25% for one variable, then we consider data as quantitative
              if ( nbModality < 0.25 ) dataType <- "qualitative"
              else dataType <- "quantitative"
            }
            if( dataType == "quantitative" ){
              .Object@dataType <- "quantitative"
              .Object@parameters = new("GaussianParameter")
              if(missing(models)){
                .Object@models = new("GaussianModel")
              }else{
                .Object@models <- models
              }
            }else if ( dataType == "qualitative" ){
              .Object@dataType <- "qualitative" 
              .Object@parameters <- new("MultinomialParameter")
              if(missing(models)){
                .Object@models = new("MultinomialModel")
              }else{
                .Object@models <- models
              }
            }else{
              stop("unknown dataType --> dataType must be 'quantitative' or 'qualitative'!")
            }
            .Object@data <- as.matrix(data)
            .Object@nbSample <- nrow(data)
            .Object@nbVariable <- ncol(data)
        }else{
          stop("data is missing !")
        } 
        if(!missing(nbCluster)){
            .Object@nbCluster <- nbCluster
        }else{
            .Object@nbCluster <- 2:8
        }
        if(missing(algo)){
          .Object@algo = new("Algo")
        }else{
          .Object@algo <- algo
        }
        if(missing(criterion)){
            .Object@criterion <- "BIC"
        }else{
            .Object@criterion <- criterion
        }
        if(!missing(weight)){
          .Object@weight <- weight
        }
        if(!missing(knownPartition)){
          .Object@knownPartition <- knownPartition
        }
        validObject(.Object)        
        return(.Object)
    }
)
###################################################################################

###################################################################################
##' Print the MixmodClustering class to standard output.
##'
##' @param x A \code{MixmodClustering} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Prints to standard out.
##'
##' @name print
##' @aliases print print,MixmodClustering-method
##' @docType methods
##' @rdname print-methods
##' @export
##'
##' @seealso \code{\link{print}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' print(xem)
##' getMethod("print", "MixmodClustering")
##'
setMethod(
    f="print",
    signature=c("MixmodClustering"),
    function(x,...){
        callNextMethod()
        if(length(x@nbClusterInBestModel)>0){
          cat("**************************************************************\n")
          cat("* nbCluster for the best model = ",x@nbClusterInBestModel,"\n")
          print(x@parameters)
        }
    }
)
###################################################################################


###################################################################################
##' Show description of the MixmodClustering class to standard output.
##'
##' @param object A \code{MixmodClustering} object.
##'
##' @return NULL. Prints to standard out.
##'
##' @name show
##' @aliases show show,MixmodClustering-method
##' @docType methods
##' @rdname show-methods
##' @export
##'
##' @seealso \code{\link{show}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' xem
##' show(xem)
##' getMethod("show", "MixmodClustering")
##'
setMethod(
    f="show",
    signature=c("MixmodClustering"),
    function(object){
        callNextMethod()
        if(length(object@nbClusterInBestModel)>0){
          cat("**************************************************************\n")
          cat("* nbCluster for the best model = ",object@nbClusterInBestModel,"\n")
          show(object@parameters)
        }
    }
)
###################################################################################


###################################################################################
##' Produce result summaries of a class [\code{\linkS4class{MixmodClustering}}] 
##'
##' @param x A \code{MixmodClustering} object.
##' @param ... further arguments passed to or from other methods
##'
##' @return NULL. Summaries to standard out.
##'
##' @name summary
##' @aliases summary summary,MixmodClustering-method
##' @docType methods
##' @rdname summary-methods
##' @export
##'
##' @seealso \code{\link{summary}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' summary(xem)
##' getMethod("summary", "MixmodClustering")
##'
setMethod(
  f="summary",
  signature=c("MixmodClustering"),
  function(object, ...){
    if(length(object@nbClusterInBestModel)>0){
      cat("**************************************************************\n")
      cat("* Number of samples = ",object@nbSample,"\n")
      cat("* Problem dimension = ",object@nbVariable,"\n")
      cat("**************************************************************\n")
      cat("*          Criterion type = ",object@criterion,"\n")
      cat("*         Criterion value = ",object@criterionValue,"\n")
      cat("*       Number of cluster = ",object@nbClusterInBestModel,"\n")
      cat("*              Model Type = ",object@bestModel,"\n")      
      cat("*              Parameters =  list by cluster\n")
      summary(object@parameters)
      cat("*          Log-likelihood = ",formatC(object@likelihood,digits=4,format="f"),"\n")   
      cat("**************************************************************\n")
    }
  }
)
###################################################################################




###################################################################################
##' Plotting of a class [\code{\linkS4class{MixmodClustering}}]  
##' 
##' @param x an object of class [\code{\linkS4class{MixmodClustering}}]
##' @param ... further arguments passed to or from other methods
##'
##' @importFrom graphics plot
##' @name plot
##' @aliases plot plot,MixmodClustering-method
##' @docType methods
##' @rdname plot-methods
##' @export
##'
##' @seealso \code{\link{plot}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' plot(xem)
##' getMethod("plot", "MixmodClustering")
##'
setMethod(
  f="plot",
  signature=c("MixmodClustering"),
  function(x, y, col=x@labels, ...){
    pairs(x@data, col=col, ...)
  }
)
###################################################################################



###################################################################################
##' Histograms of a class [\code{\linkS4class{MixmodClustering}}]  
##'  
##' @param x an object of class [\code{\linkS4class{MixmodClustering}}]
##' @param ... further arguments passed to or from other methods
##'
##' @importFrom graphics hist
##' @name hist
##' @aliases hist hist,MixmodClustering-method
##' @docType methods
##' @rdname hist-methods
##' @export
##'
##' @seealso \code{\link{hist}}
##' @examples
##' data(geyser)
##' xem <- mixmodClustering(geyser)
##' hist(xem)
##' getMethod("hist", "MixmodClustering")
##'
setMethod(
  f="hist",
  signature=c("MixmodClustering"),
  function(x, ...){
    # get the number of variable
    nvar <- x@nbVariable
    if( nvar < 4 ) par( mfrow = c( 1, nvar ) )
    else{
      nrow<-round(sqrt(nvar))
      if (is.wholenumber(sqrt(nvar))) ncol<-sqrt(nvar)
      else ncol<-sqrt(nvar)+1
      par( mfrow = c( nrow, ncol ) ) 
    }
    if ( x@dataType == "qualitative" ){
      # loop over variables
      for (j in 1:nvar ){
        barplot(table(x@data[,j]), xlab=colnames(x@data)[j], main=paste("Histogram of",colnames(x@data)[j]),  ...)
      }
    }else{
      # loop over variables
      for (j in 1:nvar ){
        xaxis<-seq(min(x@data[,j]),max(x@data[,j]),by=0.0001)
        density<-matrix(nrow=x@nbClusterInBestModel,ncol=length(xaxis))
        for( k in 1:x@nbClusterInBestModel ){
          density[k,]<-x@parameters["proportions",k]*dnorm(xaxis,x@parameters["mean",k][j],sqrt(x@parameters["variance",k][j,j]))
        }
        mixture<-apply(density,2,sum)
        h<-hist(x@data[,j], xlab=colnames(x@data)[j], main=paste("Histogram of",colnames(x@data)[j]), ...)
        
        ratio<-max(h$counts)/max(mixture)
        density<-density*ratio
        mixture<-mixture*ratio
        
        lines(xaxis,mixture,col="azure4",lty=1,lwd=4)
        for( k in 1:x@nbClusterInBestModel ){
          lines(xaxis,density[k,],col=k+1,lty=2,lwd=2)
        }
      }
    }
  }
)
###################################################################################


###################################################################################
##' Extract parts of a [\code{\linkS4class{MixmodClustering}}] class
##' 
##' @param x object from which to extract element(s) or in which to replace element(s).
##' @param i the name of the element we want to extract or replace.
##' @param j if the element designing by i is complex, j specifying elements to extract or replace.
##' @param drop For matrices and arrays.  If TRUE the result is coerced to the lowest possible dimension (see the examples).  This only works for extracting elements, not for the replacement.  See drop for further details.
##'
##' @name [
##' @aliases [,MixmodClustering,ANY,numeric-method
##' @docType methods
##' @rdname extract-methods
##'
setMethod(
  f="[", 
  signature(x = "MixmodClustering", i = "ANY", j="numeric"),
  definition=function (x, i, j, drop) {
    if ( missing(j) ){
      switch(EXPR=i,
        "nbClusterInBestModel"={return(x@nbClusterInBestModel)},
        "knownPartition"={return(x@knownPartition)},
        "proba"={return(x@proba)},
        "parameters"={return(x@parameters)},
        "algo"={return(x@algo)},
        "models"={return(x@models)},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "knownPartition"={return(x@knownPartition[,j])},
        "proba"={return(x@proba[,j])},
        "labels"={return(x@labels[j])},
        stop("This attribute doesn't exist !")
      )
    }
  }
)
##################################################################################


###################################################################################
##' Replace names of [\code{\linkS4class{MixmodClustering}}] class
##'
##' @name [
##' @aliases [<-,MixmodClustering-method [<-,MixmodClustering,ANY-method
##' @docType methods
##' @rdname extract-methods
##'
setReplaceMethod(
  f="[", 
  signature(x = "MixmodClustering", value = "ANY"), 
  definition=function (x, i, j, drop) {
    if ( missing(j) ){
      switch(EXPR=i,
      "nbClusterInBestModel"={x@nbClusterInBestModel<-value},
      "knownPartition"={x@knownPartition<-value},
      "proba"={x@proba<-value},
      "parameters"={x@parameters<-value},
      "algo"={x@algo<-value},
      "models"={x@models<-value},
      stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
      "knownPartition"={x@knownPartition[,j]<-value},
      "proba"={x@proba[,j]<-value},
      "labels"={x@labels[j]<-value},
      stop("This attribute doesn't exist !")
      )
    }
  }
)
###################################################################################

