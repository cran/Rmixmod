\docType{data}
\name{titanic}
\alias{titanic}
\title{Qualitative data: Survival of passengers on the Titanic}
\format{A data frame with 2201 observations on the following 4 variables.

\describe{

  \item{\code{Class}}{0 = crew, 1 = first, 2 = second, 3 = third, which denote the economic status of the subject}

  \item{\code{Age}}{1 = adult, 0 = child, which denote if the subject is an adult or a child}

  \item{\code{Sex}}{1 = male, 0 = female, which denote the sex of the subject}

  \item{\code{Survived}}{1 = yes, 0 = no, which denote if the subject lived through the fatal maiden voyage of the ocean liner Titanic}

}}
\source{
  The source provides a data set recording class, sex, age,
  and survival status for each person on board of the
  Titanic, and is based on data originally collected by the
  British Board of Trade and reprinted in: British Board of
  Trade (1990), "Report on the Loss of the Titanic (S.S.)".
  British Board of Trade Inquiry Report (reprint).
  Gloucester, UK: Allan Sutton Publishing.
}
\description{
  For each person on board the fatal maiden voyage of the
  ocean liner Titanic, this dataset records: sex, age
  [adult/child], economic status [first/second/third class,
  or crew] and whether or not that person survived. Values
  are aligned and delimited by blanks. There are no missing
  values.
}
\examples{
data(titanic)
}
\keyword{datasets}

