###################################################################################
##                             MultinomialModel.R                                ##
###################################################################################

###################################################################################
##' @include Model.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{MultinomialModel}}] class
##'
##' This class defines a multinomial Model. Inherits the [\code{\linkS4class{Model}}] class.
##' 
##' \describe{
##'
##'   \item{variable.independency}{logical}
##'
##'   \item{component.independency}{logical}
##'
##' }
##'
##' @examples
##'   new("MultinomialModel")
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name MultinomialModel-class
##' @rdname MultinomialModel-class
##' @exportClass MultinomialModel
##'
setClass(
    Class="MultinomialModel",
    representation=representation(
        variable.independency = "logical",
        component.independency = "logical"
    ),
    contains=c("Model"),
    prototype=prototype(
        listModels = "Binary_pk_Ekjh",
        free.proportions = TRUE,
        equal.proportions = FALSE,
        variable.independency = logical(0),
        component.independency = logical(0)
    )
)
###################################################################################


###################################################################################
##' Create an instance of the [\code{\linkS4class{MultinomialModel}}] class
##'
##' Define a list of gaussian model to test in MIXMOD.
##'
##' In the multinomial mixture model, the multinomial distribution is associated to the \eqn{j}th variable of the \eqn{k}th component is reparameterized by a center \eqn{a_k^j} and the dispersion \eqn{\varepsilon_k^j} around this center. Thus, it allows us to give an interpretation similar to the center and the variance matrix used for continuous data in the Gaussian mixture context. In the following, this model will be denoted by \eqn{[\varepsilon_k^j]}. In this context, three other models can be easily deduced. We note \eqn{[\varepsilon_k]} the model where \eqn{\varepsilon_k^j} is independent of the variable \eqn{j}, \eqn{[\varepsilon^j]} the model where \eqn{\varepsilon_k^j} is independent of the component \eqn{k} and, finally, \eqn{[\varepsilon]} the model where \eqn{\varepsilon_k^j} is independent of both the variable $j$ and the component \eqn{k}.  In order to maintain some unity in the notation, we will denote also \eqn{[\varepsilon_k^{jh}]} the most general model introduced at the previous section.
##' 
##' @param free.proportions logical to include models with free proportions. Default is TRUE.
##' @param equal.proportions logical to include models with equal proportions. Default is FALSE.
##' @param variable.independency logical to include models where \eqn{[\varepsilon_k^j]} is independent of the variable \eqn{j}. Optionnal.
##' @param component.independency logical to include models where \eqn{[\varepsilon_k^j]} is independent of the component \eqn{k}. Optionnal.
##'
##' @return an object of [\code{\linkS4class{MultinomialModel}}] containing some of the 10 Binary Models:
##' \tabular{rlll}{
##'     Model \tab Prop. \tab Var. \tab Comp. \cr
##'     Binary_p_E     \tab Equal \tab TRUE \tab TRUE \cr
##'     Binary_p_Ej    \tab \tab FALSE \tab TRUE \cr
##'     Binary_p_Ek    \tab \tab TRUE \tab FALSE \cr
##'     Binary_p_Ekj   \tab \tab FALSE \tab FALSE \cr
##'     Binary_p_Ekjh  \tab \tab FALSE \tab FALSE \cr
##'     Binary_pk_E    \tab  Free \tab TRUE \tab TRUE \cr
##'     Binary_pk_Ej   \tab \tab FALSE \tab TRUE \cr
##'     Binary_pk_Ek   \tab \tab TRUE \tab FALSE \cr
##'     Binary_pk_Ekj  \tab \tab FALSE \tab FALSE  \cr
##'     Binary_pk_Ekjh \tab \tab FALSE \tab FALSE \cr
##' }
##'
##' @references C. Biernacki, G. Celeux, G. Govaert, F. Langrognet. "Model-Based Cluster and Discriminant Analysis with the MIXMOD Software". Computational Statistics and Data Analysis, vol. 51/2, pp. 587-600. (2006)
##' @examples
##'   mixmodMultinomialModel()
##'   mixmodMultinomialModel(equal.proportions=TRUE)
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @export
##'
mixmodMultinomialModel<- function( free.proportions=TRUE, equal.proportions=TRUE, variable.independency, component.independency ){
    # check proportions parameters validity
    if ( !equal.proportions & !free.proportions )
      stop("equal.proportions and free.porportions cannot be both as FALSE !")
    
    if ( !missing(variable.independency) & !missing(component.independency)){
      if ( variable.independency & component.independency){
        free=c("Binary_pk_E")
        equal=c("Binary_p_E")
      }else if ( !variable.independency & !component.independency){
        free=c("Binary_pk_Ekj","Binary_pk_Ekjh")
        equal=c("Binary_p_Ekj","Binary_pk_Ekjh")
      }else if ( !variable.independency & component.independency){
        free=c("Binary_pk_Ej")
        equal=c("Binary_p_Ej")
      }else if ( variable.independency & !component.independency){
        free=c("Binary_pk_Ek")
        equal=c("Binary_p_Ek")
      }
    }
    else if ( !missing(component.independency) ){
      if ( component.independency ){
        free=c("Binary_pk_Ej","Binary_pk_E")
        equal=c("Binary_p_Ej","Binary_p_E")
      }else{
        free=c("Binary_pk_Ek","Binary_pk_Ekj","Binary_pk_Ekjh")
        equal=c("Binary_p_Ek","Binary_p_Ekj","Binary_p_Ekjh")
      }
      variable.independency <- logical(0)
    }
    else if ( !missing(variable.independency) ){
      if ( variable.independency ){
        free=c("Binary_pk_Ek","Binary_pk_E")
        equal=c("Binary_p_Ek","Binary_p_E")
      }else{
        free=c("Binary_pk_Ej","Binary_pk_Ekj","Binary_pk_Ekjh")
        equal=c("Binary_p_Ej","Binary_p_Ekj","Binary_p_Ekjh")
      }
      component.independency <- logical(0)
    }
    else{
      # all multinomial models with free proportions
      free=c( "Binary_pk_E", "Binary_pk_Ej", "Binary_pk_Ek", "Binary_pk_Ekj", "Binary_pk_Ekjh")
      # all multinomial models with equal proportions
      equal=c( "Binary_p_E", "Binary_p_Ej", "Binary_p_Ek", "Binary_p_Ekj", "Binary_p_Ekjh")
      variable.independency <- logical(0)
      component.independency <- logical(0)
    }
      
    # create the list of models depending on the proportions option
    if ( free.proportions & equal.proportions ){
        listModels=c(free, equal)
    }
    else if ( free.proportions ){
        listModels=free
    }
    else if ( equal.proportions ){
        listModels=equal
    }
    new("MultinomialModel", listModels=listModels, free.proportions=free.proportions, equal.proportions=equal.proportions, variable.independency=variable.independency, component.independency=component.independency)
}
###################################################################################

