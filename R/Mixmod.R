###################################################################################
##                               Mixmod.R                                        ##
###################################################################################

###################################################################################
##' @include global.R
##' @include MixmodResults.R
##' @include GaussianModel.R
##' @include MultinomialModel.R
NULL
###################################################################################


###################################################################################
##' Constructor of [\code{\linkS4class{Mixmod}}] class
##' 
##' This is a class to run mixmod library.
##'
##' \describe{
##'   \item{data}{numeric vector, matrix, or data frame of observations. Either qualitative or quantitative.}
##'   \item{dataType}{character. It defines whether data are quantitative or qualitative.}
##'   \item{nbCluster}{integer. It indicates the number of classes.}
##'   \item{knownPartition}{numeric. It contains the known partition.}
##'   \item{weight}{numeric vector with n (number of individuals) rows. Weight is optionnal. This option is to be used when weight is associated to the data.}
##'   \item{nbVariable}{integer. The number of variables.}
##'   \item{nbSample}{integer. The number of observations.}
##'   \item{criterion}{list of character. This option permits to select the criterion giving the best configuration of an execution.}
##'   \item{models}{a S4 [\code{\linkS4class{Model}}] object. Defining the list of models to be tested.}
##'   \item{error}{logical. Say if at least one model finished with no error in MIXMOD.}
##'   \item{results}{a list of S4 [\code{\linkS4class{MixmodResults}}] object containing all results. Results are sorted into a ascending order according to the first criterion (descending order for the CV criterion). This order can be changed by using the sortByCriterion() method.}
##' }
##'
##' @examples
##'   getSlots("Mixmod")
##'
##' @name Mixmod-class
##' @rdname Mixmod-class
##' @exportClass Mixmod
##'
setClass(
  Class="Mixmod",
  representation=representation(
    data = "matrix",
    dataType = "character",
    factor = "numeric",
    nbCluster = "numeric",
    knownPartition = "integer",
    weight = "numeric",
    nbVariable = "integer",
    nbSample = "integer",
    criterion = "character",
    models = "Model",
    error = "logical",
    results = "list",
    "VIRTUAL"
  ),
  prototype=prototype(
    data = matrix(nrow=0,ncol=0),
    dataType = character(0),
    factor = numeric(0),
    nbCluster = numeric(0),
    knownPartition = integer(0),
    weight = numeric(0),
    nbVariable = integer(0),
    nbSample = integer(0),
    criterion = character(0),
    error = TRUE,
    results = list()
  ),
  # define validity function
  validity=function(object){
    # check for missing values
    if ( sum(is.na(object@data)) ){
      stop("data contains NA. Mixmod cannot deal with missing values!")
    }
    # check qualitative data set
    if ( object@dataType == "qualitative" ){
      # check factors
      if ( length(object@factor) != ncol(object@data) )
        stop("number of factors too small!")
      if ( sum(is.na(object@factor)) )
        stop("factor contains NA!")
      if ( sum(!is.wholenumber(object@factor)) )
        stop("factor must be integer!")
      if ( sum(is.na(object@data)) )
        stop("data contains NA!")
      if ( sum(!is.wholenumber(object@data)) )
        stop("data contains real values!")
      # get max of modalities
      #max_mod<-apply(object@data,2,max)
      #if ( sum(max_mod>object@factor) )
      #  stop("At least one modality within 'data' is greater than the number of modalities in 'factor'!")
      # get min of modalities
      #min_mod<-apply(object@data,2,min)
      #if ( sum(min_mod<1) )
      #  stop("At least one modality within 'data' is lower than 1!")
    }
    # check data type
    if ( (object@dataType != "quantitative") & (object@dataType != "qualitative") ){
      stop("unknown dataType --> dataType must be 'quantitative' or 'qualitative'!")
    }
    # check models validity
    if ( (object@dataType == "quantitative") & is(object@models, "MultinomialModel") ){
      stop("MultinomialModel can't suit for quantitative data !\n")
    }
    if ( (object@dataType == "qualitative") & is(object@models, "GaussianModel") ){
      stop("GaussianModel can't suit for qualitative data !\n")
    }    
    # check dimensions of knownPartition
    if(length(object@knownPartition)>0){
      if ( (length(object@knownPartition)!= object@nbSample) ){
        stop("the length of knownPartition is not similar to the number of observations!")
      }   
    }
    # check if weight isn't null and equal to the number of subjects
    if(length(object@weight)>0){
      if(length(object@weight) != object@nbSample){
        stop("weight must be equal to the number of sample !")            
      }
    }
    return(TRUE)
  }
)
###################################################################################


###################################################################################
##' Create an instance of the [\code{\linkS4class{Mixmod}}] class using new/initialize.
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
  signature=c("Mixmod"),
  definition=function(.Object,data,dataType,factor,models,weight,knownPartition){
    # set missing parameters as NULL
    if (missing(dataType)) dataType<-NULL
    if (missing(factor)) factor<-NULL
    if (missing(models)) models<-NULL
    if (missing(weight)) weight<-NULL
    if (missing(knownPartition)) knownPartition<-NULL
    
    if(!missing(data)){
      if ( is.null(dataType) ){
        if ( isQualitative(data) ){ .Object@dataType <- "qualitative" }
        else{ .Object@dataType <- "quantitative" }
      }
      else{
        .Object@dataType <- dataType
      }
      if ( is.null(factor) ){
        if ( .Object@dataType == "qualitative" ){ 
          .Object@factor <- nbFactorFromData(data)
        }
      }else{
        .Object@factor <- factor
      }
      
      if ( is.factor(data) ) data<-as.interger(data)
      else if ( !is.vector(data) ){
        # loop over columns to check whether type is factor
        for ( j in 1: ncol(data) ){
          if ( is.factor(data[,j]) ) data[,j] <- as.integer(data[,j])
        }
      }
      
      # for quantitative data
      if( .Object@dataType == "quantitative" ){
        if( is.null(models) ){
          .Object@models = new("GaussianModel", listModels="Gaussian_pk_Lk_C", family="general", free.proportions=TRUE, equal.proportions=FALSE)
        }else{
          .Object@models <- models
        }
      }
      # for qualitative data
      else if ( .Object@dataType == "qualitative" ){
        if( is.null(models) ){
          .Object@models = new("MultinomialModel", listModels="Binary_pk_Ekjh", free.proportions=TRUE, equal.proportions=FALSE)
        }else{
          .Object@models <- models
        }
      }
      .Object@data <- as.matrix(data)
      .Object@nbSample <- nrow(data)
      .Object@nbVariable <- ncol(data)      
      if(!is.null(weight)){
        .Object@weight <- weight
      }
      if(!is.null(knownPartition)){
        knownPartition[knownPartition==0]<-NA
        .Object@knownPartition <- as.integer(as.factor(knownPartition))
        .Object@knownPartition[which(is.na(.Object@knownPartition))]<-as.integer(0)
      }
      # call validity function
      validObject(.Object)
    }
    return(.Object)
  }
)
###################################################################################



###################################################################################
##' @rdname print-methods
##' @aliases print print,Mixmod-method
##'
setMethod(
  f="print",
  signature=c("Mixmod"),
  function(x,...){
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
    if ( length(x@knownPartition)>0 ){
      cat("* knownPartition = ", x@knownPartition, "\n")
    }
  }
)
###################################################################################

###################################################################################
##' @rdname show-methods
##' @aliases show show,Mixmod-method
##'
setMethod(
  f="show",
  signature=c("Mixmod"),
  function(object){
    cat("* nbCluster = ", object@nbCluster, "\n")
    cat("* criterion = ", object@criterion, "\n")
    show(object@models)
    if(length(object@weight)!=0){ 
      cat("* weight  = ", object@weight, "\n")
    }
    if(length(object@data)!=0){
      nrowShow <- min(10,nrow(object@data))
      ncolShow <- min(10,ncol(object@data))
      cat("* data (limited to a 10x10 matrix) =\n")
      print(formatC(object@data[1:nrowShow,1:ncolShow]),quote=FALSE)
    }else{}
    cat("* ... ...\n")  
    if ( length(object@knownPartition)>0 ){
      if ( length(object@knownPartition)>10 ){
        cat("* knownPartition = ", object@knownPartition[1:10], " ...\n")
      }else{
        cat("* knownPartition = ", object@knownPartition, "\n")
      }
    }
  }
)
###################################################################################


###################################################################################
##' @rdname summary-methods
##' @aliases summary summary,Mixmod-method
##'
setMethod(
  f="summary",
  signature=c("Mixmod"),
  function(object, ...){
    if ( !object@error ){
      cat("**************************************************************\n")
      cat("* Number of samples    = ",object@nbSample,"\n")
      cat("* Problem dimension    = ",object@nbVariable,"\n")
      summary(object@bestResult)
    }
    return(invisible())
  }
)
###################################################################################

###################################################################################
##' Plotting of a class [\code{\linkS4class{Mixmod}}]  
##' 
##' Plotting data from a [\code{\linkS4class{Mixmod}}] object using parameters and partition
##' to distinguish the different clusters.
##'
##' For quantitative case, ellipsoids (i.e. linear transformations of hyperspheres) 
##' centered at the mean are drawn using the parameters computed by MIXMOD.
##' The directions of the principal axes of the ellipsoids are given by the eigenvectors of the covariance matrix \eqn{\Sigma}. 
##' The squared relative lengths of the principal axes are given by the corresponding eigenvalues.
##' A 1-dimensional representation of variables with the densities is drawn on the diagonal.
##'
##' For qualitative case, a Multiple Correspondance Analysis is performed to get a
##' 2-dimensional representation of the data set. Overlaping means that observations are similar.
##'
##' @param x an object of class [\code{\linkS4class{Mixmod}}]
##' @param ... further arguments passed to or from other methods
##'
##' @importFrom graphics plot
##' @name plot
##' @aliases plot plot,Mixmod-method
##' @docType methods
##' @rdname plot-methods
##' @exportMethod plot
##'
##' @seealso \code{\link{plot}}
##' @examples
##'   ## for quantitative case
##'   data(geyser)
##'   xem <- mixmodCluster(geyser,3)
##'   plot(xem)
##'
##'   ## for qualitative case
##'   data(birds)
##'   xem2 <- mixmodCluster(birds,2,factor=c(2,5,6,3,5,4))
##'   plot(xem2)
##'
setMethod(
  f="plot",
  signature=c("Mixmod"),
  function(x, y, ...){
    # for quantitative data
    if ( x@dataType == "quantitative" ){
      # create layout
      if ( x@nbVariable == 1 ){
        stop("data has only one variable. Try hist() function to get a 1D representation of x.")
      }
      else{ 
      
        # get old par 
        op <- par(no.readonly = TRUE) # the whole list of settable par's.
        # changing marging
        par(mar = rep(2.5,4))
        # decreasing font size
        par(cex = .75)
        # create layout matrix
        #par( mfrow = c(x@nbVariable, x@nbVariable) )
        split.screen(c(x@nbVariable, x@nbVariable))
        # create histogram on diagonal
        for ( i in 1:x@nbVariable ){
          #par(mfg=c(i,i))
          screen(i+((i-1)*x@nbVariable))
          histCluster(x@bestResult, x@data, variables=colnames(x@data)[i])
        }
        # create biplots
        for ( i in 2:x@nbVariable ){
          for( j in 1:(i-1) ){
            #par(mfg=c(i,j))
            screen(j+((i-1)*x@nbVariable))
            plotCluster(x@bestResult, x@data, variable1=colnames(x@data)[j], variable2=colnames(x@data)[i], ...)
          }
        }
        close.screen(all = TRUE)
        # restore plotting parameters
        par(op)
      }
    }
    # for qualitative data
    else if ( x@dataType == "qualitative" ){
      # create layout matrix
      par( mfrow = c(1, 1) )
      
      # get binary matrix from x
      matX <- matrix2binary(as.data.frame(x@data))
      # get number of observations
      n <- dim(matX)[1]
      # get number of variables
      p <- ncol(x@data)
      
      Dc <- drop((rep(1, n)) %*% matX)
      Y <- t(t(matX)/(sqrt(p * Dc)))
      Y.svd <- svd(Y)
      individuals <- Y %*% Y.svd$v[, 2:3]/p

      # plotting the first 2 axes  
      plot( jitter(individuals[,2], factor=10) ~ jitter(individuals[,1], factor=10), 
      col=x@bestResult@partition+1, pch=x@bestResult@partition, xlab='Axis 1', ylab='Axis 2', main='Multiple Correspondance Analysis', ... )
    }
    invisible()
  }
)
###################################################################################


###################################################################################
##' Histograms of a class [\code{\linkS4class{Mixmod}}]  
##'
##' Histograms of data from a [\code{\linkS4class{Mixmod}}] object using parameters
##' to plot densities in a quantitative case and probablities in a qualitative case.
##'
##' For quantitative case, data with the density of each cluster and the mixture density are drawn for each variable.
##'
##' For qualitative case, each line corresponds to one variable. On the left-hand side is 
##' drawn a barplot with data. Then a barplot is drawn for each cluster with the probabilities for 
##' each modality to be in that cluster.
##'  
##' @param x an object of class [\code{\linkS4class{Mixmod}}]
##' @param ... further arguments passed to or from other methods
##'
##' @importFrom graphics hist
##' @name hist
##' @aliases hist hist,Mixmod-method
##' @docType methods
##' @rdname hist-methods
##' @exportMethod hist
##'
##' @seealso \code{\link{hist}}
##' @examples
##'   ## for quantitative case
##'   data(geyser)
##'   xem1 <- mixmodCluster(geyser,3)
##'   hist(xem1)
##'
##'   ## for qualitative case
##'   data(birds)
##'   xem2 <- mixmodCluster(birds,2,factor=c(2,5,6,3,5,4))
##'   hist(xem2)
##'
setMethod(
  f="hist",
  signature=c("Mixmod"),
  function(x, ...){
    histCluster(x@bestResult, x@data, ...)
    invisible()
  }
)
###################################################################################


###################################################################################
##' @rdname sortByCriterion-methods
##' @aliases sortByCriterion,Mixmod,character-method
##'
setMethod(
  f="sortByCriterion",
  signature=c("Mixmod","character"),
  definition=function(object,criterion){
    # check whether the number of criterion is greater than one
    if ( length(object@criterion) == 1 ){
      stop("Mixmod object contains only one criterion")
    }
    if ( !(criterion %in% object@criterion) ){
      stop(paste("No results for the criterion",criterion,"!"))
    }
    # sort by criterion only if there are more than one result
    if ( length(object@results) == 1 ){
      stop("only one result in Mixmod object")
    }
    # get criterion index
    index<-which(object@criterion == criterion) 
    # set the first result as the best one
    best <- object@results[[1]]@criterionValue[index]
    # loop over results
    for ( i in 2:length(object@results) ){
      x <- object@results[[i]]
      if ( x@error != "No error" ) break
      crit <- x@criterionValue[index]
      j <- i
      while ( j > 1 ){
        if ( criterion == "CV" ){
          if ( object@results[[j-1]]@criterionValue[index] < crit ){
            object@results[[j]] <- object@results[[j-1]]
          }else{
            break
          }
        }else{
          if ( object@results[[j-1]]@criterionValue[index] > crit ){
            object@results[[j]] <- object@results[[j-1]]
          }else{
            break
          }
        }
        j <- j-1
      }
      object@results[[j]] <- x 
    }
    object@bestResult<-object@results[[1]]
    return(object)
  }
)
###################################################################################

