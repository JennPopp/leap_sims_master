import h5py
import numpy as np

testfile = "SimonsTestFile.h5"
f = h5py.File(testfile)
dset = f["data"]["14388"]["particles"]["epol"]

# print(list(dset.keys()))

print(list(dset["weighting"][0:10]))
# print(dset["x"].len())
