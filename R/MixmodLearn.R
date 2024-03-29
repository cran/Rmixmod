##################################################################################
#                              MixmodLearn.R                                    ##
##################################################################################

#' @include global.R
#' @include Mixmod.R
#' @include MixmodDAResults.R
#' @include GaussianParameter.R
#' @include MultinomialParameter.R
#' @include GaussianModel.R
#' @include MultinomialModel.R
NULL

#' Constructor of [\code{\linkS4class{MixmodLearn}}] class
#'
#' This is a class to run discriminant analysis with mixmod. Inherits the [\code{\linkS4class{Mixmod}}] class.
#'
#' \describe{
#'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodDAResults}}] object containing the best model results.}
#'   \item{nbCVBlocks}{integer which defines the number of block to perform the Cross Validation.}
#' }
#' @examples
#' ## A quantitative example with the famous iris data set
#' new("MixmodLearn", data = iris[1:4], knownLabels = iris$Species)
#'
#' getSlots("MixmodLearn")
#' @name MixmodLearn-class
#' @rdname MixmodLearn-class
#' @exportClass MixmodLearn
#'
setClass(
  Class = "MixmodLearn",
  representation = representation(
    nbCVBlocks = "numeric",
    bestResult = "MixmodDAResults"
  ),
  contains = c("Mixmod"),
  prototype = prototype(
    nbCVBlocks = 10
  ),
  # define validity function
  validity = function(object) {
    # check whether the knownLabels is missing
    if (length(object@knownLabels) == 0) {
      stop("labels are missing!")
    }
    # check the number of cluster
    if ((min(as.integer(object@knownLabels)) <= 0)) {
      stop("Each observation in knownLabels must have a valid cluster affectation !")
    }
    # check criterion parameter
    if (sum(object@criterion %in% c("BIC", "CV")) != length(object@criterion)) {
      stop(cat(object@criterion[which(!(object@criterion %in% c("BIC", "CV")))], "is not a valid criterion name !\n"))
    }
    # check number of CV blocks
    if (!is.wholenumber(object@nbCVBlocks) | (object@nbCVBlocks < 1)) {
      stop("the number of CV blocks is not valid!")
    }
    return(TRUE)
  }
)

#' Create an instance of the [\code{\linkS4class{MixmodLearn}}] class using new/initialize.
#'
#' Initialization method. Used internally in the `Rmixmod' package.
#'
#' @seealso \code{\link{initialize}}
#'
#' @keywords internal
#'
#' @rdname initialize-methods
#'
setMethod(
  f = "initialize",
  signature = c("MixmodLearn"),
  #    mixmodLearn <- function(data, knownLabels, dataType=NULL, models=NULL, criterion="CV", nbCVBlocks=10, weight=NULL) {
  definition = function(.Object, data = NULL, knownLabels = NULL, dataType = NULL, models = NULL, criterion = "CV",
                        nbCVBlocks = 10, weight = NULL, seed = -1, xmlIn = NULL, xmlOut = NULL, trace = 0, massiccc = 0) {
    if (!missing(xmlIn)) {
      if (!missing(criterion) || !missing(nbCVBlocks)) {
        stop("xmlIn argument is mutually exclusive with all other arguments but xmlOut, seed and trace")
      }
    } else { # i.e without xmlIn
      if (missing(knownLabels)) {
        stop("knownLabels is missing !")
      }
      if (!is.vector(knownLabels) & !is.factor(knownLabels)) {
        stop("knownLabels must be a vector or a factor")
      }
      .Object@knownLabels <- knownLabels
    }

    # call initialize method of abstract class Mixmod
    # .Object<-callNextMethod(.Object, data, ...)
    .Object <- callNextMethod(.Object,
      data = data, dataType = dataType, models = models, weight = weight,
      knownLabels = knownLabels, xmlIn = xmlIn, xmlOut = xmlOut, seed = seed, trace = trace, massiccc = massiccc
    )
    if (length(.Object@data)) {
      # create MixmodDAResults object
      if (.Object@dataType == "quantitative") {
        .Object@bestResult <- new("MixmodDAResults")
        .Object@bestResult@parameters <- new("GaussianParameter")
      } else if (.Object@dataType == "qualitative") {
        .Object@bestResult <- new("MixmodDAResults")
        .Object@bestResult@parameters <- new("MultinomialParameter")
      } else if (.Object@dataType == "composite") {
        .Object@bestResult <- new("MixmodDAResults")
        .Object@bestResult@parameters <- new("CompositeParameter")
        .Object@bestResult@parameters@factor <- .Object@factor
      }
      # create nbCluster
      if (length(.Object@knownLabels)) {
        .Object@nbCluster <- max(as.integer(.Object@knownLabels))
      }
      .Object@criterion <- criterion
      .Object@nbCVBlocks <- nbCVBlocks
      # if (is.na(criterion)) {
      #    .Object@criterion <- "CV"
      # }else{
      #    .Object@criterion <- criterion
      # }
      # if (is.na(nbCVBlocks)) {
      #  .Object@nbCVBlocks <- 10
      # } else {
      #  .Object@nbCVBlocks <- nbCVBlocks
      # }
      validObject(.Object)
    } else if (!missing(xmlIn)) {
      .Object@bestResult@parameters <- new("CompositeParameter")
    }
    return(.Object)
  }
)

#' mixmodLearn
#'
#' TODO: describe...
#'
#' @param ... ...
#'
#' @export
#'
mixmodLearn <- function(...) {

  # create Mixmod object
  xem <- new("MixmodLearn", ...)
  # call learnMain
  .Call("learnMain", xem, PACKAGE = "Rmixmod")
  # mixmod error?
  if (xem@error) warning("All models got errors!")

  # return MixmodLearn object
  return(xem)
}

#' Create an instance of the [\code{\linkS4class{MixmodLearn}}] class
#'
#' This function computes the first step of a discriminant analysis. It will find the best classification rule by running an
#' M step from the training observations.
#'
#' @param data frame containing quantitative,qualitative or heterogeneous data. Rows correspond to observations and columns
#' correspond to variables.
#' @param knownLabels an integer vector or a factor of size number of observations. Each cell corresponds to a cluster
#' affectation. So the maximum value is the number of clusters.
#' @param dataType character. Type of data is "quantitative", "qualitative" or "composite". Set as NULL by default, type will
#'  be guessed depending on variables type (in case of homogeneous data). 'composite' type must be specified explicitly.
#' @param models a [\code{\linkS4class{Model}}] object defining the list of models to run. For quantitative data, the model
#' "Gaussian_pk_Lk_C" is called (see mixmodGaussianModel() to specify other models). For qualitative data, the model
#' "Binary_pk_Ekjh" is called (see mixmodMultinomialModel() to specify other models).
#' @param criterion list of character defining the criterion to select the best model. Possible values: "BIC", "CV" or
#' c("CV","BIC"). Default is "CV".
#' @param nbCVBlocks integer which defines the number of block to perform the Cross Validation. This value will be ignored if
#' the CV criterion is not chosen. Default value is 10.
#' @param weight numeric vector with n (number of individuals) rows. Weight is optional. This option is to be used when weight
#' is associated to the data.
#'
#' @examples
#' ## A quantitative example with the famous iris data set
#' learn.iris <- mixmodLearn(iris[1:4], iris$Species)
#' ## get summary
#' summary(learn.iris)
#'
#' ## A qualitative example with the famous birds data set
#' data(birds)
#' birds.partition <- as.integer(c(rep(1, 34), rep(2, 35)))
#' learn.birds <- mixmodLearn(data = birds, knownLabels = birds.partition)
#' ## get summary
#' summary(learn.birds)
#'
#' ## A composite example with a heterogeneous data set
#' data(heterodatatrain)
#' learn.hetero <- mixmodLearn(heterodatatrain[-1], knownLabels = heterodatatrain$V1)
#' ## get summary
#' summary(learn.hetero)
#' @author Florent Langrognet and Remi Lebret and Christian Poli ans Serge Iovleff, with contributions from C. Biernacki and
#' G. Celeux and G. Govaert \email{contact@@mixmod.org}
#' @return Returns an instance of the [\code{\linkS4class{MixmodLearn}}] class. Those two attributes will contain all outputs:
#' \describe{
#'   \item{results}{a list of [\code{\linkS4class{MixmodResults}}] object containing all the results sorted in ascending order
#' according to the given criterion.}
#'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodResults}}] object containing the best model results.}
#' }
mixmodLearn.default <- function(data, knownLabels, dataType = NULL, models = NULL, criterion = "CV", nbCVBlocks = 10,
                                weight = NULL) {
  stop("mixmodLearn.default: not implemented\n")
}

# old_mixmodLearn <- function(data, knownLabels, dataType=NULL, models=NULL, criterion="CV", nbCVBlocks=10, weight=NULL) {
#  # check options
#  if (missing(data)) {
#    stop("data is missing !")
#  }
#  if ( missing(knownLabels)) {
#    stop("knownLabels is missing !")
#  }
#  if (!is.data.frame(data) & !is.vector(data) & !is.vector(data)) {
#    stop("data must be a data.frame or a vector or a factor")
#  }
#
#  if (!is.vector(knownLabels) & !is.factor(knownLabels)) {
#    stop("knownLabels must be a vector or a factor")
#  }
#
#  # create Mixmod object
#  xem <- new( "MixmodLearn", data=data, knownLabels=knownLabels, dataType=dataType, models=models, criterion=criterion,
# nbCVBlocks=nbCVBlocks, weight=weight )
#  # call learnMain
#  .Call("learnMain", xem, PACKAGE="Rmixmod")
#  # mixmod error?
#  if ( xem@error ) warning( "All models got errors!")
#
#  # return MixmodLearn object
#  return(xem)
# }

#' @rdname print-methods
#' @aliases print print,MixmodLearn-method
#'
setMethod(
  f = "print",
  signature = c("MixmodLearn"),
  function(x, ...) {
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")
    callNextMethod()
    if (!x@error) {
      cat("\n\n")
      cat("****************************************\n")
      cat("*** BEST MODEL OUTPUT:\n")
      cat(paste("*** According to the", x@criterion[1], "criterion\n"))
      cat("****************************************\n")
      print(x@bestResult)
    } else {
      cat("\n\n")
      cat("****************************************\n")
      cat("*** NO OUTPUT - All models got errors !\n")
      cat("****************************************\n")
    }
    return(invisible())
  }
)

#' @rdname show-methods
#' @aliases show show,MixmodLearn-method
#'
setMethod(
  f = "show",
  signature = c("MixmodLearn"),
  function(object) {
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")
    callNextMethod()
    if (!object@error) {
      cat("\n\n")
      cat("****************************************\n")
      cat("*** BEST MODEL OUTPUT:\n")
      cat(paste("*** According to the", object@criterion[1], "criterion\n"))
      cat("****************************************\n")
      show(object@bestResult)
    } else {
      cat("\n\n")
      cat("****************************************\n")
      cat("*** NO OUTPUT - All models got errors !\n")
      cat("****************************************\n")
    }
    return(invisible())
  }
)

#' @rdname extract-methods
#' @aliases [,MixmodLearn-method
#'
setMethod(
  f = "[",
  signature(x = "MixmodLearn"),
  definition = function(x, i, j, drop) {
    if (missing(j)) {
      switch(EXPR = i,
        "data" = {
          return(x@data)
        },
        "dataType" = {
          return(x@dataType)
        },
        "nbCluster" = {
          return(x@nbCluster)
        },
        "criterion" = {
          return(x@criterion)
        },
        "models" = {
          return(x@models)
        },
        "knownLabels" = {
          return(x@knownLabels)
        },
        "weight" = {
          return(x@weight)
        },
        "bestResult" = {
          return(x@bestResult)
        },
        "results" = {
          return(x@results)
        },
        "error" = {
          return(x@error)
        },
        stop("This attribute doesn't exist !")
      )
    } else {
      switch(EXPR = i,
        "nbCluster" = {
          return(x@nbCluster[j])
        },
        "criterion" = {
          return(x@criterion[j])
        },
        "data" = {
          return(x@data[, j])
        },
        "knownLabels" = {
          return(x@knownLabels[, j])
        },
        "weight" = {
          return(x@weight[j])
        },
        "results" = {
          return(x@results[[j]])
        },
        stop("This attribute doesn't exist !")
      )
    }
  }
)

# ' @name [
#' @rdname extract-methods
#' @aliases [<-,MixmodLearn-method
#'
setReplaceMethod(
  f = "[",
  signature(x = "MixmodLearn"),
  definition = function(x, i, j, value) {
    if (missing(j)) {
      switch(EXPR = i,
        "data" = {
          x@data <- value
        },
        "dataType" = {
          x@dataType <- value
        },
        "criterion" = {
          x@criterion <- value
        },
        "models" = {
          x@models <- value
        },
        "knownLabels" = {
          x@knownLabels <- value
        },
        "weight" = {
          x@weight <- value
        },
        stop("This attribute doesn't exist !")
      )
    } else {
      switch(EXPR = i,
        "data" = {
          x@data[, j] <- value
        },
        "criterion" = {
          x@criterion[j] <- value
        },
        "knownLabels" = {
          x@knownLabels[j] <- value
        },
        "weight" = {
          x@weight[j] <- value
        },
        stop("This attribute doesn't exist !")
      )
    }
    validObject(x)
    return(x)
  }
)
