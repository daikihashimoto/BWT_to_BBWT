# constructing the BWT based on rotation (without $) in-place

def inplaceBWT(T):
	n = len(T)
	T[n-1], T[n-2] = T[n-2], T[n-1]
	print("1 " + "".join(T))

	lastpos = n-1
	lastchar = T[n-1]
	for startpos in range(n-2, 0, -1):
		print("lastpos = ", lastpos)
		print("lastchar = ", lastchar)
		print("startpos = ", startpos)
		c = len([x for x in T[startpos:] if x < lastchar])
		rank = len([x for x in T[startpos:lastpos] if x == lastchar])
		lastpos = startpos + c + rank 
		print("newlastpos = ", lastpos)
		print("rank= ", rank)
		print("c= ", c)
		lastchar = T[startpos-1]
		print(startpos-1, lastpos)

		print(n - startpos, "".join(T))
		for i in range(startpos-1, lastpos): T[i] = T[i+1]
		T[lastpos] = lastchar

		print(n - startpos, "".join(T))
	return T

def lynPerm(T):
	for i in range(len(T)):
		fulfill=True
		S = T[i:]+T[:i]
		for s in range(1, len(T)):
			if S >= S[s:]: fulfill = False
		if fulfill == True: break
	if fulfill == True: return i
	else: return -1

def inplace(t):
	T = list(t)
	pos = lynPerm(T)
	if pos == -1:
		print('there is no Lyndon conjugate!')
		quit()
	T = T[pos:]+T[:pos]
	print(T)
	return "".join(inplaceBWT(T))


import sys

t = sys.argv[1]
print('text = ' + t)
inbwt = inplace(t)

# for debug
# import bwt
# stdbwt = bwt.bwt(t)
# print(inbwt == stdbwt[0], inbwt, stdbwt)
