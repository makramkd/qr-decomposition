# QR Decomposition
The QR Decomposition algorithm is, as it's name suggests, a decomposition 
algorithm. Given a matrix `A`, the QR decomposition algorithm factors `A` into
`A = QR` where `Q` is an orthogonal matrix - meaning it's columns form an orthogonal basis - and `R` is an upper triangular matrix.

## Motivation
The reason I'm implementing this is because it is the base algorithm of least squares regression, and is also used in the QR eigenvalue algorithm, which I plan to implement as well, either in this repo or somewhere else. 

## Usage
This will be a library-type routine, so it will be used via header inclusion. In particular, I'll be recycling a matrix class I have used before verbatim from one of my other repositories, and it is that matrix class (which is very bare bones) that I will use to implement the QR algorithm.

