##################################################################################
#                               MixmodCluster.R                                 ##
##################################################################################

#' @include global.R
#' @include Mixmod.R
#' @include MixmodResults.R
#' @include GaussianParameter.R
#' @include MultinomialParameter.R
#' @include CompositeParameter.R
#' @include GaussianModel.R
#' @include MultinomialModel.R
#' @include CompositeModel.R
#' @include Strategy.R
NULL

#' Constructor of [\code{\linkS4class{MixmodCluster}}] class
#'
#' This is a class to run clustering with mixmod. Inherits the [\code{\linkS4class{Mixmod}}] class.
#'
#' \describe{
#'   \item{strategy}{a S4 [\code{\linkS4class{Strategy}}] object. Defining the strategy used to run MIXMOD.}
#'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodResults}}] object containing the best model results.}
#' }
#'
#' @examples
#' ## A quantitative example with the famous iris data set
#' data(iris)
#' ## with default values
#' new("MixmodCluster", data = iris[1:4], nbCluster = 3)
#'
#' getSlots("MixmodCluster")
#' @name MixmodCluster-class
#' @rdname MixmodCluster-class
#' @exportClass MixmodCluster
#'
setClass(
  Class = "MixmodCluster",
  representation = representation(
    strategy = "Strategy",
    bestResult = "MixmodResults"
  ),
  contains = c("Mixmod"),
  # define validity function
  validity = function(object) {
    # check if nbCluster exists
    if (length(object@nbCluster) == 0) {
      stop("nbCluster is empty!")
    }
    if (length(object@nbCluster) > 1 && (object@strategy@initMethod == "parameter" || object@strategy@initMethod == "label")) {
      stop("parameter and label initialisations require nbCluster to be mono-valued!")
    }
    # check nbCluster parameter
    if (sum(!is.wholenumber(object@nbCluster))) {
      stop("nbCluster must contain only integer!")
    }
    # check nbCluster parameter
    if (sum(object@nbCluster < 1)) {
      stop("nbCluster must greater than 0!")
    }
    # check criterion parameter
    if (sum(object@criterion %in% c("BIC", "ICL", "NEC")) != length(object@criterion)) {
      stop(cat(object@criterion[which(!(object@criterion %in% c("BIC", "ICL", "NEC")))], "is not a valid criterion name !\n"))
    }
    # test if there exists known partitions
    if (length(object@knownLabels) > 0) {
      if (length(object@nbCluster) > 1) {
        stop("more than one cluster, knownLabels can be used only with one number of cluster!")
      } else if (max(object@knownLabels) != object@nbCluster) {
        stop("labels within knownLabels are not valid!")
      }
    }
    return(TRUE)
  }
)

#' Create an instance of the [\code{\linkS4class{MixmodCluster}}] class
#'
#' This function computes an optimal mixture model according to the criteria furnished,
#' and the list of model defined in [\code{\linkS4class{Model}}], using the algorithm specified in
#' [\code{\linkS4class{Strategy}}].
#'
#' @param ... all arguments are transfered to the MixmodCluster constructor. Valid arguments are:
#' \describe{
#' \item{data:}{ frame containing quantitative,qualitative or heterogeneous data. Rows correspond to observations and
#' columns correspond to variables.}
#' \item{nbCluster:}{ numeric listing the number of clusters.}
#' \item{dataType:}{ character. Type of data is "quantitative", "qualitative" or "composite". Set as NULL by default,
#' type will be guessed depending on variables type. }
#' \item{models:}{ a [\code{\linkS4class{Model}}] object defining the list of models to run. For quantitative data, the
#' model "Gaussian_pk_Lk_C" is called (see mixmodGaussianModel() to specify other models). For qualitative data, the model
#' "Binary_pk_Ekjh" is called (see mixmodMultinomialModel() to specify other models).}
#' \item{strategy:}{ a [\code{\linkS4class{Strategy}}] object containing the strategy to run. Call mixmodStrategy() method
#' by default.}
#' \item{criterion:}{ list of character defining the criterion to select the best model. The best model is the one with the
#' lowest criterion value. Possible values: "BIC", "ICL", "NEC", c("BIC", "ICL", "NEC"). Default is "BIC".}
#' \item{weight:}{ numeric vector with n (number of individuals) rows. Weight is optional. This option is to be used when
#' weight is associated to the data.}
#' \item{knownLabels:}{ vector of size nbSample. it will be used for semi-supervised classification when labels are known.
#' Each cell corresponds to a cluster affectation.}
#' }
# ` @param data frame containing quantitative,qualitative or heterogeneous data. Rows correspond to observations and columns
# ` correspond to variables.
# ` @param nbCluster numeric listing the number of clusters.
# ` @param dataType character. Type of data is "quantitative", "qualitative" or "composite". Set as NULL by default, type will
# ` be guessed depending on variables type.
# ` @param models a [\code{\linkS4class{Model}}] object defining the list of models to run. For quantitative data, the model
# ` "Gaussian_pk_Lk_C" is called (see mixmodGaussianModel() to specify other models). For qualitative data, the model
# ` "Binary_pk_Ekjh" is called (see mixmodMultinomialModel() to specify other models).
# ` @param strategy a [\code{\linkS4class{Strategy}}] object containing the strategy to run. Call mixmodStrategy()
# ` method by default.
# ` @param criterion list of character defining the criterion to select the best model. The best model is the one with the
# ` lowest criterion value. Possible values: "BIC", "ICL", "NEC", c("BIC", "ICL", "NEC"). Default is "BIC".
# ` @param weight numeric vector with n (number of individuals) rows. Weight is optional. This option is to be used when
# ` weight is associated to the data.
# ` @param knownLabels vector of size nbSample. it will be used for semi-supervised classification when labels are known.
# ` Each cell corresponds to a cluster affectation.
#'
#' @examples
#' ## A quantitative example with the famous geyser data set
#' data(geyser)
#' ## with default values
#' mixmodCluster(geyser, nbCluster = 2:6)
#'
#' ## A qualitative example with the birds data set
#' data(birds)
#' mixmodCluster(
#'   data = birds, nbCluster = 2:5, criterion = c("BIC", "ICL", "NEC"),
#'   model = mixmodMultinomialModel()
#' )
#'
#' ## use graphics functions
#' xem <- mixmodCluster(data = geyser, nbCluster = 3)
#' \dontrun{
#' plot(xem)
#' hist(xem)
#' }
#'
#' ## get summary
#' summary(xem)
#'
#' ## A composite example with a heterogeneous data set
#' data(heterodata)
#' mixmodCluster(heterodata, 2)
#' @author Florent Langrognet and Remi Lebret and Christian Poli ans Serge Iovleff, with contributions from C. Biernacki
#' and G. Celeux and G. Govaert \email{contact@@mixmod.org}
#' @return Returns an instance of the [\code{\linkS4class{MixmodCluster}}] class.
#' Those two attributes will contain all outputs:
#' \describe{
#'   \item{results}{a list of [\code{\linkS4class{MixmodResults}}] object containing all the results sorted in ascending order
#' according to the given criterion.}
#'   \item{bestResult}{a S4 [\code{\linkS4class{MixmodResults}}] object containing the best model results.}
#' }
#' @export
#'
# old_mixmodCluster <- function(data, nbCluster, dataType=NULL, models=NULL, strategy=mixmodStrategy(), criterion="BIC",
# weight=NULL, knownLabels=NULL) {
#  # check options
#  if(missing(data)){
#    stop("data is missing !")
#  }
#  if(missing(nbCluster)){
#    stop("nbCluster is missing!")
#  }
#  if (!is.data.frame(data) & !is.vector(data) & !is.vector(data) ){
#    stop("data must be a data.frame or a vector or a factor")
#  }
#
#  # create Mixmod object
#  xem <- new( "MixmodCluster", data=data, nbCluster=nbCluster, dataType=dataType, models=models, strategy=strategy,
# criterion=criterion, weight=weight, knownLabels=knownLabels)
mixmodCluster <- function(...) {
  xem <- new("MixmodCluster", ...)
  # call clusteringMain
  .Call("clusteringMain", xem, PACKAGE = "Rmixmod")
  # mixmod error?
  if (xem@error) warning("All models got errors!")

  # return MixmodClustering object
  return(xem)
}

# mixmodCluster.default <- function(data, nbCluster, dataType=NULL, models=NULL, strategy=mixmodStrategy(), criterion="BIC",
# weight=NULL, knownLabels=NULL) {
# stop("mixmodCluster.default: not implemented\n");
# }

#' Create an instance of the [\code{\linkS4class{MixmodCluster}}] class using new/initialize.
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
  signature = c("MixmodCluster"),
  definition = function(.Object, data = NULL, nbCluster = NULL, dataType = NULL, models = NULL, strategy = NULL,
                        criterion = NULL, weight = NULL, knownLabels = NULL, seed = -1, xmlIn = NULL, xmlOut = NULL,
                        trace = 0, massiccc = 0) {
    if (!missing(xmlIn)) {
      if (!missing(nbCluster) || !missing(strategy) || !missing(criterion)) {
        stop("xmlIn argument is mutually exclusive with all other arguments but xmlOut, seed and trace")
      }
    } else { # i.e. without xmlIn
      # get number of cluster
      if (!missing(nbCluster)) {
        .Object@nbCluster <- nbCluster
      } else {
        stop("nbCluster is missing!")
      }
    }
    # call initialize method of abstract class Mixmod
    # (.Object,data,dataType,models,weight,knownLabels, xmlIn, xmlOut, seed, trace)
    .Object <- callNextMethod(.Object,
      data = data, dataType = dataType, models = models, weight = weight,
      knownLabels = knownLabels, xmlIn = xmlIn, xmlOut = xmlOut, seed = seed, trace = trace, massiccc = massiccc
    )

    if (length(.Object@data)) {
      # create MixmodResults object
      if (.Object@dataType == "quantitative") {
        .Object@bestResult <- new("MixmodResults")
        .Object@bestResult@parameters <- new("GaussianParameter")
      } else if (.Object@dataType == "qualitative") {
        .Object@bestResult <- new("MixmodResults")
        .Object@bestResult@parameters <- new("MultinomialParameter")
      } else if (.Object@dataType == "composite") {
        .Object@bestResult <- new("MixmodResults")
        .Object@bestResult@parameters <- new("CompositeParameter")
        .Object@bestResult@parameters@factor <- as.integer(.Object@factor)
      }
      # create strategy
      if (missing(strategy)) {
        .Object@strategy <- new("Strategy")
      } else {
        .Object@strategy <- strategy
      }
      if (!missing(seed) && .Object@strategy@seed != -1) {
        stop(
          "'seed' value specification is inconsistent: mixmodCluster argument is ", seed,
          "since strategy@seed is ", .Object@strategy@seed
        )
      }
      # get criterion parameter
      if (missing(criterion)) {
        .Object@criterion <- "BIC"
      } else {
        .Object@criterion <- criterion
      }
      # call validity method
      validObject(.Object)
    } else if (!missing(xmlIn)) {
      .Object@bestResult@parameters <- new("CompositeParameter")
      .Object@strategy <- new("Strategy")
    }
    # return object
    return(.Object)
  }
)

#' @rdname print-methods
#' @aliases print print,MixmodCluster-method
#'
setMethod(
  f = "print",
  signature = c("MixmodCluster"),
  function(x, ...) {
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")
    callNextMethod()
    print(x@strategy)
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
#' @aliases show show,MixmodCluster-method
#'
setMethod(
  f = "show",
  signature = c("MixmodCluster"),
  function(object) {
    cat("****************************************\n")
    cat("*** INPUT:\n")
    cat("****************************************\n")
    callNextMethod()
    show(object@strategy)
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
#' @aliases [,MixmodCluster-method
#'
setMethod(
  f = "[",
  signature(x = "MixmodCluster"),
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
        "strategy" = {
          return(x@strategy)
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
        "partition" = {
          return(x@bestResult@partition)
        },
        "parameters" = {
          return(x@bestResult@parameters)
        },
        "proba" = {
          return(x@bestResult@proba)
        },
        "bestModel" = {
          return(x@bestResult@model)
        },
        "criterionValue" = {
          return(x@bestResult@criterionValue)
        },
        "likelihood" = {
          return(x@bestResult@likelihood)
        },
        "bestNbCluster" = {
          return(x@bestResult@nbCluster)
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
          return(x@knownLabels[j])
        },
        "weight" = {
          return(x@weight[j])
        },
        "results" = {
          return(x@results[[j]])
        },
        "partition" = {
          return(x@bestResult@partition[j])
        },
        "proba" = {
          return(x@bestResult@proba[, j])
        },
        "criterionValue" = {
          return(x@bestResult@criterionValue[j])
        },
        stop("This attribute doesn't exist !")
      )
    }
  }
)

# ' @name [
#' @rdname extract-methods
#' @aliases [<-,MixmodCluster-method
#'
setReplaceMethod(
  f = "[",
  signature(x = "MixmodCluster"),
  definition = function(x, i, j, value) {
    if (missing(j)) {
      switch(EXPR = i,
        "data" = {
          x@data <- value
        },
        "dataType" = {
          x@dataType <- value
        },
        "nbCluster" = {
          x@nbCluster <- value
        },
        "criterion" = {
          x@criterion <- value
        },
        "strategy" = {
          x@strategy <- value
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
        "nbCluster" = {
          x@nbCluster[j] <- value
        },
        "criterion" = {
          x@criterion[j] <- value
        },
        "data" = {
          x@data[, j] <- value
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
