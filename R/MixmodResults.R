###################################################################################
##                                MixmodResults.R                                ##
###################################################################################

###################################################################################
##' @include global.R
##' @include Parameter.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{MixmodResults}}] class
##'
##' This is a class to contain results from MIXMOD library.
##'  
##' \describe{
##'   \item{nbCluster}{integer. It indicates the number of components.}
##'   \item{model}{character. Name of the model.}
##'   \item{criterion}{list of character. This option permits to select the criterion giving the best configuration of an execution.}
##'   \item{criterionValue}{numeric. Values of the criterion.}
##'   \item{parameters}{a S4 [\code{\linkS4class{Parameter}}] object. The best model parameters.}
##'   \item{likelihood}{numeric. The model likelihood.}
##'   \item{partition}{vector of integers defining the partition.}
##'   \item{proba}{a matrix of probabilities.}
##'   \item{error}{a character. The mixmod error.}
##' }
##'
##' @examples
##'   getSlots("MixmodResults")
##'
##' @name MixmodResults-class
##' @rdname MixmodResults-class
##' @exportClass MixmodResults
##'
setClass(
    Class="MixmodResults",
    representation=representation(
        nbCluster = "numeric",
        model = "character",
        criterion = "character",
        criterionValue = "numeric",
        parameters = "Parameter",
        likelihood = "numeric",
        partition = "integer",
        proba = "matrix",
        error = "character"
    ),
    prototype=prototype(
        nbCluster = numeric(0),
        model = character(0),
        criterion = character(0),
        criterionValue = numeric(0),
        likelihood = numeric(0),
        partition = integer(0),
        proba = matrix(nrow=0,ncol=0),
        error = character(0)
    )
)
###################################################################################

###################################################################################
##' @rdname print-methods
##' @aliases print print,MixmodResults-method
##'
setMethod(
  f="print",
  signature=c("MixmodResults"),
  function(x,...){ 
    cat("* nbCluster   = ", x@nbCluster,"\n")      
    cat("* model name  = ", x@model, "\n")
    if ( x@error == "No error" ){
      cat("* criterion   = ", paste(x@criterion, "(", formatC(x@criterionValue,digits=4,format="f"), ")", sep="") ); cat("\n")
      cat("* likelihood  = ", formatC(x@likelihood,digits=4,format="f"), "\n")
      print(x@parameters)
    }else{
      cat("* No results. MIXMOD library stopped with following error: ", x@error, "!\n")
    }
  }
)
###################################################################################

###################################################################################
##' @rdname show-methods
##' @aliases show show,MixmodResults-method
##'
setMethod(
  f="show",
  signature=c("MixmodResults"),
  function(object){
    cat("* nbCluster   = ", object@nbCluster,"\n")      
    cat("* model name  = ", object@model, "\n")
    if ( object@error == "No error" ){
      cat("* criterion   = ", paste(object@criterion, "(", formatC(object@criterionValue,digits=4,format="f"), ")", sep="") ); cat("\n")
      cat("* likelihood  = ", formatC(object@likelihood,digits=4,format="f"), "\n")
      show(object@parameters)
    }else{
      cat("* No results. MIXMOD library stopped with the following error: ", object@error, "!\n")
    }
  }
)
###################################################################################

###################################################################################
##' @rdname summary-methods
##' @aliases summary summary,MixmodResults-method
##'
setMethod(
  f="summary",
  signature=c("MixmodResults"),
  function(object, ...){
    cat("**************************************************************\n")
    cat("*       Number of cluster = ", object@nbCluster,"\n")
    cat("*              Model Type = ", object@model,"\n")      
    if ( object@error == "No error" ){
      cat("*               Criterion = ", paste(object@criterion, "(", formatC(object@criterionValue,digits=4,format="f"), ")", sep="") ); cat("\n")
      cat("*              Parameters =  list by cluster\n")
      summary(object@parameters)
      cat("*          Log-likelihood = ",formatC(object@likelihood,digits=4,format="f"),"\n")     
    }else{
      cat("* No results. MIXMOD library stopped with the following error: ", object@error, "!\n")
    }
    cat("**************************************************************\n")
  }
)
###################################################################################

###################################################################################
##' Define function to draw an ellipse
##'
##' @param x an object of class [\code{\linkS4class{MixmodResults}}]
##' @param i an index of one variable from data
##' @param j an index of one variable from data
##' 
##' @keywords internal
##'
ellipse<-function(x, i, j){
  # loop over the cluster
  for ( k in 1:x@nbCluster ){
    angles <- seq(0, 2*pi, length.out=200)
    ctr<-c(x@parameters@mean[k,i],x@parameters@mean[k,j])
    A<-matrix(c(x@parameters@variance[[k]][i,i],x@parameters@variance[[k]][j,i],x@parameters@variance[[k]][i,j],x@parameters@variance[[k]][j,j]), nrow=2)
    eigVal  <- eigen(A)$values
    eigVec  <- eigen(A)$vectors
    eigScl  <- eigVec %*% diag(sqrt(eigVal))  # scale eigenvectors to length = square-root
    xMat    <- rbind(ctr[1] + eigScl[1, ], ctr[1] - eigScl[1, ])
    yMat    <- rbind(ctr[2] + eigScl[2, ], ctr[2] - eigScl[2, ])
    ellBase <- cbind(sqrt(eigVal[1])*cos(angles), sqrt(eigVal[2])*sin(angles)) # normal ellipse
    ellRot  <- eigVec %*% t(ellBase)                                          # rotated ellipse
    
    
    lines((ellRot+ctr)[1, ], (ellRot+ctr)[2, ], asp=1, type="l", lwd=2)
    matlines(xMat, yMat, col=1, lty=2, lwd=1)
    points(ctr[1], ctr[2], pch=4, lwd=3)
  }
}
###################################################################################


###################################################################################
##' Plotting of a class [\code{\linkS4class{MixmodResults}}]  
##' 
##' Biplot of two variables from a quantitative data set. Use parameters and partition from a [\code{\linkS4class{MixmodResults}}] object to distinguish the different clusters.
##'
##' Ellipsoids (i.e. linear transformations of hyperspheres) 
##' centered at the mean can be drawn using the parameters computed by MIXMOD.
##' The directions of the principal axes of the ellipsoids are given by the eigenvectors of the covariance matrix \eqn{\Sigma}. 
##' The squared relative lengths of the principal axes are given by the corresponding eigenvalues.
##'
##' @param x an object of class [\code{\linkS4class{MixmodResults}}]
##' @param data a matrix or a data frame containing a quantitative data set.
##' @param variable1 character containing the name of the first variable. First column of data by default.
##' @param variable2 character containing the name of the second variable. Second column of data by default.
##' @param col a specification for the default plotting color. By default partition is used to separate clusters with different colors.
##' @param pch either an integer specifying a symbol or a single character to be used as the default in plotting points. By default partition is used to seperate clusters with different symbols.
##' @param xlab a title for the x axis. Variable1 by default.
##' @param ylab a title for the y axis. Variable2 by default.
##' @param add.ellipse a boolean. Add ellipses to graph. TRUE by default.
##' @param ... further arguments passed to or from other methods
##'
##' @examples
##'   data(geyser)
##'   xem <- mixmodCluster(geyser,3)
##'   plotCluster(xem["bestResult"], geyser)
##'
##' @seealso \code{\link{plot}}
##' @export
##'
plotCluster <- function(x, data, variable1=colnames(data)[1], variable2=colnames(data)[2], col=x@partition+1, pch=x@partition, xlab=variable1, ylab=variable2, add.ellipse=TRUE, ...){
  if ( !is(x,"MixmodResults") )
    stop("x must be a MixmodResults object!")
  if ( !is(x@parameters, "GaussianParameter") )
    stop("x must contains Gaussian parameters!")
  if ( !is.matrix(data) & !is.data.frame(data) )
    stop("data must be a data.frame or a matrix object!")
  if ( !(variable1 %in% colnames(data)) )
    stop("variable1 is unknown!")
  if ( !(variable2 %in% colnames(data)) )
    stop("variable2 is unknown!")
  
  # get variables indices
  index1<-which(colnames(data) == variable1)
  index2<-which(colnames(data) == variable2)
  
  plot(data[,index1],data[,index2],col=col,pch=pch, xlab=xlab, ylab=ylab, ...)
  if ( add.ellipse ) ellipse(x,index1,index2)
}
###################################################################################


###################################################################################
##' Histogram of a class [\code{\linkS4class{MixmodResults}}]  
##' 
##' Histograms of data object using parameters from a [\code{\linkS4class{MixmodResults}}]
##' to plot densities in a quantitative case and probablities in a qualitative case.
##'
##' For quantitative case, data with the density of each cluster and the mixture density are drawn for each variable.
##'
##' For qualitative case, each line corresponds to one variable. On the left-hand side is 
##' drawn a barplot with data. Then a barplot is drawn for each cluster with the probabilities for 
##' each modality to be in that cluster.
##'
##' @param x an object of class [\code{\linkS4class{MixmodResults}}]
##' @param data a vector, matrix or data frame containing a quantitative or a qualitative data set.
##' @param variables list of variables names to compute a histogram. All variables from data by default.
##' @param xlab a list of title for the x axis. xlab must have the same length than variables.
##' @param main a list of title for the histogram. main must have the same length than variables.
##' @param ... further arguments passed to or from other methods
##'
##' @examples
##'   ## for qualitative case
##'   data(geyser)
##'   xem1 <- mixmodCluster(geyser,3)
##'   histCluster(xem1["bestResult"], geyser)
##'
##'   ## for qualitative case
##'   data(birds)
##'   xem2 <- mixmodCluster(birds,2,factor=c(2,5,6,3,5,4))
##'   histCluster(xem2["bestResult"], birds)
##'
##' @seealso \code{\link{hist}}
##' @export
##'
histCluster <- function(x, data, variables=colnames(data), xlab=rep("",length(variables)), main=paste("Histogram of",variables), ...){
  # check the options
  if ( !is(x,"MixmodResults") )
  stop("'x' must be a MixmodResults object!")
  if ( !is.matrix(data) & !is.data.frame(data) & !is.vector(data) )
  stop("'data' must be a vector, a data.frame or a matrix object!")
  if ( length(variables) == 0 )
  stop("'variables' is empty!")
  if ( length(variables)>ncol(data) )
  stop("List of variables too long!")
  if ( sum(!(variables %in% colnames(data))) )
  stop("At least one variable is unknown!") 
  
  # get old par 
  op <- par(no.readonly = TRUE) # the whole list of settable par's.
  
  # get the indices of variables
  indices<-which(colnames(data) %in% variables)
  nvar<-length(indices)
  
  if ( is(x@parameters, "GaussianParameter") ){
    # split the layout
    if( nvar < 4 & nvar > 1) par( mfrow = c( 1, nvar ) )
    else if ( nvar >= 4 ){
      nrow<-round(sqrt(nvar))
      if (is.wholenumber(sqrt(nvar))) ncol<-sqrt(nvar)
      else ncol<-sqrt(nvar)+1
      par( mfrow = c( nrow, ncol ) ) 
    }
    i<-1
    # loop over variables
    for (j in indices ){
      
      xaxis<-seq(min(data[,j]),max(data[,j]),by=0.0001)
      density<-matrix(nrow=x@nbCluster,ncol=length(xaxis))
      
      # loop over the clusters to generate densities
      for( k in 1:x@nbCluster ){
        density[k,]<-x@parameters["proportions",k]*dnorm(xaxis,x@parameters["mean",k][j],sqrt(x@parameters["variance",k][j,j]))
      }
      # generate mixture density
      mixture<-apply(density,2,sum)
      h<-hist(data[,j], xlab=xlab[i], main=main[i], ...)
      
      ratio<-max(h$counts)/max(mixture)
      density<-density*ratio
      mixture<-mixture*ratio
      
      lines(xaxis,mixture,col="azure4",lty=1,lwd=4)
      for( k in 1:x@nbCluster ){
        lines(xaxis,density[k,],col=k+1,lty=2,lwd=2)
      }
      i<-i+1
    }
  }
  else if ( is(x@parameters, "MultinomialParameter") ){
    # split the layout
    par( mfrow = c( nvar, x@nbCluster+1 ) )
    par(mar = par("mar")*.75)
    i<-1
    # loop over variables
    for (j in indices ){
      f<-x@parameters@factor[j]
      t<-table(data[,j])
      t[which(is.na(t[1:f]))]<-0
      names(t)<-1:f
      freq<-barplot(as.vector(t), names.arg=names(t), xlab=xlab[i], main="", ...)
      title(main[i],cex.main=1)
      for ( k in 1:x@nbCluster){
        center_k<-x@parameters@center[k,j]
        proba_k<-x@parameters@scatter[[k]][j,]
        proba_k[center_k]<-1-proba_k[center_k]
        prob<-barplot(proba_k[1:f], names.arg=names(t), main="", col=k+1, ylim=c(0,1), ...)
        title(paste("Cluster",k),cex.main=1)
        text(prob,proba_k[1:f]+(max(proba_k)/10),ifelse(proba_k[1:f]<0.01,format(proba_k[1:f],scientific=TRUE,digits=2),round(proba_k[1:f],2)),xpd=TRUE,font=2)
      }
      i<-i+1
    }
  }
  else{
    stop("Uknown type of parameters!")
  }
  par(op)
}
###################################################################################


###################################################################################
##' @rdname extract-methods
##' @aliases [,MixmodResults-method
##'
setMethod(
  f="[", 
  signature(x = "MixmodResults"),
  definition=function (x, i, j, drop) {
    if ( missing(j) ){
      switch(EXPR=i,
        "nbCluster"={return(x@nbCluster)},
        "criterion"={return(x@criterion)},
        "criterionValue"={return(x@criterionValue)},
        "model"={return(x@model)},
        "likelihood"={return(x@likelihood)},
        "parameters"={return(x@parameters)},
        "partition"={return(x@partition)},
        "proba"={return(x@proba)},
        "error"={return(x@error)},
        stop("This attribute doesn't exist !")
      )
    }else{
      switch(EXPR=i,
        "criterion"={return(x@criterion[j])},
        "criterionValue"={return(x@criterionValue[j])},
        stop("This attribute doesn't exist !")
      )
    }
  }
)
##################################################################################
