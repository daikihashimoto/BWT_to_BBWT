# In-Place BWT conversions

This project provides algorithm for converting an inpet text to a BWT variant or for converting between two BWT variants.
We focus on the following variants:

 - the standard BWT storing an artificial dollar sign to denote the end of the string
 - the bijective BWT (BBWT)
 - the BWT based on rotations (BWTC) not storting a dollar sign

The focus is on in-place conversions, using additional a constant number of variables besides the rewriteable input.
We have the following algorithms implemented:

 - an algorithm computing the BBWT from BWT (bwt_to_bbwt.cpp)
 - an algorithm computing BWTC from an input text (cbwt_constr.py)

