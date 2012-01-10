###################################################################################
##                          GaussianModel.R                                      ##
###################################################################################

###################################################################################
##' @include Model.R
NULL
###################################################################################

###################################################################################
##' Constructor of [\code{\linkS4class{GaussianModel}}] class
##'
##' This class defines a gaussian Model. Inherits the [\code{\linkS4class{Model}}] class.
##' 
##' \describe{
##'
##'   \item{family}{character defining a family of models.}
##'
##' }
##'
##' @examples
##'   new("GaussianModel")
##'   new("GaussianModel",family="general")
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @name GaussianModel-class
##' @rdname GaussianModel-class
##' @exportClass GaussianModel
##'
setClass(
    Class="GaussianModel",
    representation=representation(
        family = "character"
    ),
    contains=c("Model"),
    prototype=prototype(
        listModels = "Gaussian_pk_Lk_C",
        family = "general",
        free.proportions = TRUE,
        equal.proportions = FALSE
    )
)
###################################################################################

###################################################################################
##' Create an instance of the [\code{\linkS4class{GaussianModel}}] class
##'
##' Define a list of gaussian model to test in MIXMOD.
##' 
##' In the Gaussian mixture model, following Banfield and Raftery (1993) and Celeux and Govaert (1995), we consider a parameterization of the variance matrices of the mixture components consisting of expressing the variance matrix \eqn{\Sigma_{k}} in terms of its eigenvalue decomposition \deqn{ \Sigma_{k}= \lambda_{k} D_{k} A_{k}D'_{k}} where \eqn{\lambda_{k}=|\Sigma_{k}|^{1/d}, D_{k}} is the matrix of eigenvectors of \eqn{\Sigma_{k}} and \eqn{A_{k}} is a diagonal matrix, such that \eqn{| A_{k} |=1}, with the normalized eigenvalues of \eqn{\Sigma_{k}} on the diagonal in a decreasing order. The parameter \eqn{\lambda_{k}} determines the \emph{volume} of the \eqn{k}th cluster, \eqn{D_{k}} its \emph{orientation} and \eqn{A_{k}} its \emph{shape}.  By allowing some but not all of these quantities to vary between clusters, we obtain parsimonious and easily interpreted models which are appropriate to describe various clustering situations.
##' 
##' In general family, we can allow the volumes, the shapes and the orientations of clusters to vary or to be equal between clusters. Variations on assumptions on the parameters \eqn{\lambda_{k}, D_{k}} and \eqn{A_{k}} \eqn{(1 \leq k \leq K)} lead to 8 general models of interest. For instance, we can assume different volumes and keep the shapes and orientations equal by requiring that \eqn{A_{k}=A} (\eqn{A} unknown) and \eqn{D_{k}=D} (\eqn{D} unknown) for \eqn{k=1,\ldots,K}. We denote this model \eqn{[\lambda_{k}DAD']}. With this convention, writing \eqn{[\lambda D_{k}AD'_{k}]} means that we consider the mixture model with equal volumes, equal shapes and different orientations.
##' In diagonal family, we assume that the variance matrices \eqn{\Sigma_{k}} are diagonal. In the parameterization, it means that the orientation matrices \eqn{D_{k}} are permutation matrices. We write \eqn{\Sigma_{k}=\lambda_{k}B_{k}} where \eqn{B_{k}} is a diagonal matrix with \eqn{| B_{k}|=1}.  This particular parameterization gives rise to 4 models: \eqn{[\lambda B]}, \eqn{[\lambda_{k}B]}, \eqn{[\lambda B_{k}]} and \eqn{[\lambda_{k}B_{k}]}.
##' 
##' In spherical family, we assume spherical shapes, namely \eqn{A_{k}=I}, \eqn{I} denoting the identity matrix. In such a case, two parsimonious models are in competition: \eqn{[\lambda I]} and \eqn{[\lambda_{k}I]}.
##'
##' @param family character defining a family of models. "general" for the general family, "diagonal" for the diagonal family, "spherical" for the spherical family and "all" for all families. Default is "general".
##' @param free.proportions logical to include models with free proportions. Default is TRUE.
##' @param equal.proportions logical to include models with equal proportions. Default is TRUE.
##'
##' @return an object of [\code{\linkS4class{GaussianModel}}] which contains some of the 28 Gaussian Models:
##' \tabular{rlllll}{
##'     Model  \tab Family \tab Prop. \tab Voume \tab Shape \tab Orient. \cr
##'     Gaussian_p_L_C         \tab General \tab Equal \tab Equal \tab Equal  \tab Equal \cr
##'     Gaussian_p_Lk_C        \tab \tab \tab Free \tab Equal \tab Equal \cr
##'     Gaussian_p_L_D_Ak_D    \tab  \tab \tab Equal \tab Free \tab Equal \cr
##'     Gaussian_p_Lk_D_Ak_D   \tab  \tab \tab Free \tab Free \tab Equal \cr
##'     Gaussian_p_L_Dk_A_Dk   \tab  \tab \tab Equal \tab Equal \tab Free \cr
##'     Gaussian_p_Lk_Dk_A_Dk  \tab  \tab \tab Free \tab Equal \tab Free \cr
##'     Gaussian_p_L_Ck        \tab  \tab \tab Equal \tab Free \tab Free \cr
##'     Gaussian_p_Lk_Ck       \tab  \tab \tab Free \tab Free \tab Free \cr
##'     Gaussian_p_L_B         \tab Diagonal  \tab Equal \tab Equal \tab Equal \tab Axes \cr
##'     Gaussian_p_Lk_B        \tab  \tab \tab Free \tab Equal \tab Axes \cr
##'     Gaussian_p_L_Bk        \tab  \tab \tab Equal \tab Free \tab Axes \cr
##'     Gaussian_p_Lk_Bk       \tab  \tab \tab Free \tab Free \tab Axes \cr
##'     Gaussian_p_L_I         \tab Spherical \tab Equal \tab Equal \tab Equal \tab NA \cr
##'     Gaussian_p_Lk_I        \tab \tab \tab Free \tab Equal \tab NA \cr
##'     Gaussian_pk_L_C        \tab General  \tab Free \tab Equal \tab Equal \tab Equal \cr
##'     Gaussian_pk_Lk_C       \tab \tab \tab Free \tab Equal \tab Equal \cr
##'     Gaussian_pk_L_D_Ak_D   \tab \tab \tab Equal \tab Free \tab Equal \cr
##'     Gaussian_pk_Lk_D_Ak_D  \tab \tab \tab Free \tab Free \tab Equal \cr
##'     Gaussian_pk_L_Dk_A_Dk  \tab \tab \tab Equal \tab Equal \tab Free \cr
##'     Gaussian_pk_Lk_Dk_A_Dk \tab \tab \tab Free \tab Equal \tab Free \cr
##'     Gaussian_pk_L_Ck       \tab \tab \tab Equal \tab Free \tab Free \cr
##'     Gaussian_pk_Lk_Ck      \tab \tab \tab Free \tab Free \tab Free \cr
##'     Gaussian_pk_L_B        \tab  Diagonal  \tab Free \tab Equal \tab Equal \tab Axes \cr
##'     Gaussian_pk_Lk_B       \tab \tab \tab Free \tab Equal \tab Axes \cr
##'     Gaussian_pk_L_Bk       \tab \tab \tab Equal \tab Free \tab Axes \cr
##'     Gaussian_pk_Lk_Bk      \tab \tab \tab Free \tab Free \tab Axes \cr
##'     Gaussian_pk_L_I        \tab Spherical  \tab Free \tab Equal \tab Equal \tab NA \cr
##'     Gaussian_pk_Lk_I       \tab \tab \tab Free \tab Equal \tab NA \cr 
##' }
##'
##' @references C. Biernacki, G. Celeux, G. Govaert, F. Langrognet. "Model-Based Cluster and Discriminant Analysis with the MIXMOD Software". Computational Statistics and Data Analysis, vol. 51/2, pp. 587-600. (2006)
##' @examples
##'   mixmodGaussianModel()
##'   mixmodGaussianModel(family="all",free.proportions=FALSE)
##' @author Remi Lebret \email{remi.lebret@@math.univ-lille1.fr}
##' @export
##'
mixmodGaussianModel<- function( family="all", free.proportions=TRUE, equal.proportions=TRUE ){
    # check proportions parameters validity
    if ( !equal.proportions & !free.proportions )
    stop("equal.proportions and free.porportions cannot be both as FALSE !")
    # all spherical models
    if ( family=="spherical" ){
        free=c("Gaussian_pk_L_I", "Gaussian_pk_Lk_I")
        equal=c("Gaussian_p_L_I", "Gaussian_p_Lk_I")
    }
    # all diagonal models
    else if ( family=="diagonal" ){
        free=c( "Gaussian_pk_L_B", "Gaussian_pk_Lk_B", "Gaussian_pk_L_Bk", "Gaussian_pk_Lk_Bk")
        equal=c( "Gaussian_p_L_B", "Gaussian_p_Lk_B", "Gaussian_p_L_Bk", "Gaussian_p_Lk_Bk")
    }
    # all general models
    else if ( family=="general" ){
        free=c( "Gaussian_pk_L_C"
        , "Gaussian_pk_Lk_C"
        , "Gaussian_pk_L_D_Ak_D"
        , "Gaussian_pk_Lk_D_Ak_D"
        , "Gaussian_pk_L_Dk_A_Dk"
        , "Gaussian_pk_Lk_Dk_A_Dk"
        , "Gaussian_pk_L_Ck"
        , "Gaussian_pk_Lk_Ck"
        )
        equal=c( "Gaussian_p_L_C"
        , "Gaussian_p_Lk_C"
        , "Gaussian_p_L_D_Ak_D"
        , "Gaussian_p_Lk_D_Ak_D"
        , "Gaussian_p_L_Dk_A_Dk"
        , "Gaussian_p_Lk_Dk_A_Dk"
        , "Gaussian_p_L_Ck"
        , "Gaussian_p_Lk_Ck"
        )
    }
    # all gaussian models
    else if (family=="all"){
        free=c( "Gaussian_pk_L_I"
        , "Gaussian_pk_Lk_I"
        , "Gaussian_pk_L_B"
        , "Gaussian_pk_Lk_B"
        , "Gaussian_pk_L_Bk"
        , "Gaussian_pk_Lk_Bk"
        , "Gaussian_pk_L_C"
        , "Gaussian_pk_Lk_C"
        , "Gaussian_pk_L_D_Ak_D"
        , "Gaussian_pk_Lk_D_Ak_D"
        , "Gaussian_pk_L_Dk_A_Dk"
        , "Gaussian_pk_Lk_Dk_A_Dk"
        , "Gaussian_pk_L_Ck"
        , "Gaussian_pk_Lk_Ck"
        )
        equal=c( "Gaussian_p_L_I"
        , "Gaussian_p_Lk_I"
        , "Gaussian_p_L_B"
        , "Gaussian_p_Lk_B"
        , "Gaussian_p_L_Bk"
        , "Gaussian_p_Lk_Bk"
        , "Gaussian_p_L_C"
        , "Gaussian_p_Lk_C"
        , "Gaussian_p_L_D_Ak_D"
        , "Gaussian_p_Lk_D_Ak_D"
        , "Gaussian_p_L_Dk_A_Dk"
        , "Gaussian_p_Lk_Dk_A_Dk"
        , "Gaussian_p_L_Ck"
        , "Gaussian_p_Lk_Ck"
        )
    }
    else{
        stop("unknown family name !")
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
    new("GaussianModel", listModels=listModels, family=family, free.proportions=free.proportions, equal.proportions=equal.proportions)
}
###################################################################################
