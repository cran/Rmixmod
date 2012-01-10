###################################################################################
##                                global.R                                       ##
###################################################################################

###################################################################################
##' Define function to check an integer
##'
##' @param x a numeric
##' @param tol a real which defines the tolerance
##'
##' @return a logical. TRUE if \code{x} is an integer, FALSE otherwise.
##' 
##' @keywords internal
##'
is.wholenumber <- function(x, tol = .Machine$double.eps^0.5)  abs(x - round(x)) < tol
###################################################################################
