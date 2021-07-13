import numpy as np
from uncertainties import ufloat
from uncertainties.umath import *

l = 8

equil = 5
qunch = 1
nconf = 4
decor = 1

#k0 = 1
#k1 = 20

k0s = [1]
k1s = [0]

for k0 in k0s:
    for k1 in k1s:
        if k0 == 0 and k1 == 0: continue
        xr = []
        for cpu in range(1, 11):
            data = np.fromfile("{}-{}-{}-{}/glass_l{:03d}_k{:03d}{:02d}_{:02d}.dat".
                format(equil, qunch, nconf, decor, l, k0, k1, cpu), dtype=np.int32)
            data = data.reshape(10**(qunch+nconf), l, l)

            x = np.sum(data, axis=(1,2))
            x = np.mean(x ** 2) / l ** 2
            xr.append(x)

        x = ufloat(np.mean(xr), np.std(xr, ddof=1)/np.sqrt(10))
        temp = k0+k1/100
        val = log10(x)
        print(temp, np.log10(temp), val.nominal_value, val.std_dev)
