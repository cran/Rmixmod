###################################################################################
##                              MixmodLearn.R                                    ##
###################################################################################

###################################################################################
##' @include global.R
##' @include Mixmod.R
##' @include MixmodDAResults.R
##' @include GaussianParameter.R
##' @include MultinomialParameter.R
##' @include GaussianModel.R
##' @include MultinomialModel.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{MixmodLearn}}] class
##'
##' This is a class to run discriminant analysis with mixmod. Inherits the [\code{\linkS4class{Mixmod}}] class.
##'
##' \describe{
##'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodDAResults}}] object containing the best model results.}
##'   \item{nbCVBlocks}{integer which defines the number of observation per block to perform the Cross Validation.}
##' }
##' @examples
##'   ## A quantitative example with the famous iris data set
##'   new("MixmodLearn", data=iris[1:4], knownPartition=iris$Species)
##'
##'   getSlots("MixmodLearn")
##'
##' @name MixmodLearn-class
##' @rdname MixmodLearn-class
##' @exportClass MixmodLearn
##'
setClass(
  Class="MixmodLearn",
  representation=representation(
    nbCVBlocks = "numeric",
    bestResult = "MixmodDAResults"
  ),
  contains=c("Mixmod"),
  prototype=prototype(
    nbCVBlocks = 10
  ),    
  # define validity function
  validity=function(object){  
    # check whether the knownPartition is missing
    if ( length(object@knownPartition) == 0 ){
      stop("partition is missing!")
    }
    # check the number of cluster
    if ( (min(object@knownPartition)<=0) ){
      stop("Each observation in knownPartition must have a valid cluster affectation !")
    }
    # check criterion parameter
    if ( sum(object@criterion %in% c("BIC","CV")) != length(object@criterion) ){
      stop(cat(object@criterion[which(!(object@criterion %in% c("BIC","CV")))], "is not a valid criterion name !\n"))
    }
    # check number of CV blocks
    if ( !is.wholenumber(object@nbCVBlocks) | (object@nbCVBlocks<1) ){
      stop("the number of CV blocks is not valid!")
    }
    return(TRUE)
  }
)
###################################################################################


###################################################################################
##' Create an instance of the [\code{\linkS4class{MixmodLearn}}] class using new/initialize.
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
    signature=c("MixmodLearn"),
    definition=function(.Object,criterion,nbCVBlocks,...){
      
      # call initialize method of abstract class Mixmod  
      .Object<-callNextMethod(.Object,...)
      
      if ( length(.Object@data) ){
        # create MixmodDAResults object
        if( .Object@dataType == "quantitative" ){
          .Object@bestResult = new("MixmodDAResults")
          .Object@bestResult@parameters = new("GaussianParameter")
        }else if ( .Object@dataType == "qualitative" ){
          .Object@bestResult = new("MixmodDAResults")
          .Object@bestResult@parameters = new("MultinomialParameter")
        }
        # create nbCluster
        if(length(.Object@knownPartition)){
          .Object@nbCluster <- max(.Object@knownPartition)
        }
        if(missing(criterion)){
            .Object@criterion <- "CV"
        }else{
            .Object@criterion <- criterion
        }
        if(!missing(nbCVBlocks)){
          .Object@nbCVBlocks <- nbCVBlocks
        }
        validObject(.Object)     
      }
      return(.Object)
    }
)
###################################################################################


###################################################################################
##' Create an instance of the [\code{\linkS4class{MixmodLearn}}] class
##'
##' This function computes the first step of a discriminant analysis. It will find the best classification rule by running an M step from the training observations.
##' 
##' @param data matrix or data frame containing quantitative or qualitative data. Rows correspond to observations and columns correspond to variables.
##' @param knownPartition vector of size number of observations. Each cell corresponds to a cluster affectation. So the maximum value is the number of clusters.
##' @param dataType character. Type of data is either "quantitative" or "qualitative". Set as NULL by default, type will be guessed depending on variables type.
##' @param factor vector of integer containing the number of modalities for each qualitative variable. It is optional for qualitative data set and keep it as NULL for quantitative data set. Factors can also be generated by the nbFactorFromData() method.
##' @param models a [\code{\linkS4class{Model}}] object defining the list of models to run. For quantitative data, the model "Gaussian_pk_Lk_C" is called (see mixmodGaussianModel() to specify other models). For qualitative data, the model "Binary_pk_Ekjh" is called (see mixmodMultinomialModel() to specify other models).
##' @param criterion list of character defining the criterion to select the best model. Possible values: "BIC", "CV" or c("CV","BIC"). Default is "CV".
##' @param nbCVBlocks integer which defines the number of observation per block to perform the Cross Validation. This value will be ignored if the CV criterion is not choosen. Default value is 10.
##' @param weight numeric vector with n (number of individuals) rows. Weight is optionnal. This option is to be used when weight is associated to the data.
##'
##' @examples
##'   ## A quantitative example with the famous iris data set
##'   learn.iris<-mixmodLearn(iris[1:4], iris$Species)
##'   ## get summary
##'   summary(learn.iris)
##'
##'   ## An example with the famous birds data set
##'   data(birds)
##'   birds.partition<-c(rep(1,34),rep(2,35))
##'   learn.birds<-mixmodLearn(data=birds, knownPartition=birds.partition, factor=c(2,5,6,3,5,4))
##'   ## get summary
##'   summary(learn.birds)
##'
##' @author Remi Lebret and Serge Iovleff and Florent Langrognet, with contributions from C. Biernacki and G. Celeux and G. Govaert \email{contact@@mixmod.org}
##' @return Returns an instance of the [\code{\linkS4class{MixmodLearn}}] class. Those two attributes will contain all outputs:
##' \describe{
##'   \item{results}{a list of [\code{\linkS4class{MixmodResults}}] object containing all the results sorted in ascending order according to the given criterion.}
##'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodResults}}] object containing the best model results.}
##' }

##' @export
##'
mixmodLearn <- function(data, knownPartition, dataType=NULL, factor=NULL, models=NULL, criterion="CV", nbCVBlocks=10, weight=NULL) {
  # check options
  if(missing(data)){
    stop("data is missing !")
  } 
  if ( missing(knownPartition) ){
    stop("knownPartition is missing !")
  }
  # create Mixmod object
  xem <- new( "MixmodLearn", data=data, knownPartition=knownPartition, dataType=dataType, factor=factor, models=models, criterion=criterion, nbCVBlocks=nbCVBlocks, weight=weight )
  # call learnMain
  .Call("learnMain", xem, PACKAGE="Rmixmod")
  # mixmod error?
  if ( xem@error ) warning( "All models got errors!" )
  
  # return MixmodLearn object
  return(xem)
}
###################################################################################


###################################################################################
##' @rdname print-methods
##' @aliases print print,MixmodLearn-method
##'
setMethod(
  f="print",
  signature=c("MixmodLearn"),
  function(x,...){
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")    
    callNextMethod()
    if ( !x@error ){
      cat("\n\n")
      cat("****************************************\n")
      cat("*** BEST MODEL OUTPUT:\n")
      cat("****************************************\n")
      print(x@bestResult)
    }
    else{
      cat("\n\n")
      cat("****************************************\n")
      cat("*** NO OUTPUT - All models got errors !\n")
      cat("****************************************\n")
    }
    return(invisible())
  }
)
###################################################################################

###################################################################################
##' @rdname show-methods
##' @aliases show show,MixmodLearn-method
##'
setMethod(
  f="show",
  signature=c("MixmodLearn"),
  function(object){
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")    
    callNextMethod()
    if ( !object@error ){
      cat("\n\n")
      cat("****************************************\n")
      cat("*** BEST MODEL OUTPUT:\n")
      cat("****************************************\n")
      show(object@bestResult)
    }
    else{
      cat("\n\n")
      cat("****************************************\n")
      cat("*** NO OUTPUT - All models got errors !\n")
      cat("****************************************\n")
    }
    return(invisible())
  }
)
###################################################################################



###################################################################################
##' @rdname extract-methods
##' @aliases [,MixmodLearn-method
##'
setMethod(
  f="[", 
  signature(x = "MixmodLearn"),
  definition=function (x, i, j, drop) {
    if ( missing(j) ){
      switch(EXPR=i,
        "data"={return(x@data)},
        "dataType"={return(x@dataType)},
        "nbCluster"={return(x@nbCluster)},
        "criterion"={return(x@criterion)},
        "models"={return(x@models)},
        "knownPartition"={return(x@knownPartition)},
        "weight"={return(x@weight)},
        "bestResult"={return(x@bestResult)},
        "results"={return(x@results)},
        "error"={return(x@error)},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "nbCluster"={return(x@nbCluster[j])},
        "criterion"={return(x@criterion[j])},
        "data"={return(x@data[,j])},
        "knownPartition"={return(x@knownPartition[,j])},
        "weight"={return(x@weight[j])},
        "results"={return(x@results[[j]])},
        stop("This attribute doesn't exist !")
      )
    }
  }
)
##################################################################################


###################################################################################
##' @name [
##' @rdname extract-methods
##' @aliases [<-,MixmodLearn-method
##'
setReplaceMethod(
  f="[", 
  signature(x = "MixmodLearn"), 
  definition=function (x, i, j, value) {
    if ( missing(j) ){
      switch(EXPR=i,
        "data"={x@data<-value},
        "dataType"={x@dataType<-value},
        "criterion"={x@criterion<-value},
        "models"={x@models<-value},
        "knownPartition"={x@knownPartition<-value},
        "weight"={x@weight<-value},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "data"={x@data[,j]<-value},
        "criterion"={x@criterion[j]<-value},
        "knownPartition"={x@knownPartition[j]<-value},
        "weight"={x@weight[j]<-value},
        stop("This attribute doesn't exist !")
      )
    }
    validObject(x)
    return(x)
  }
)
###################################################################################

