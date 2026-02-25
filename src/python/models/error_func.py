import numpy as np
from scipy.special import erf, erfc

def skew_erf(x, A, mu=0.0, sigma=1.0, alpha=0.0, C=0.0):
    """
    Skewed error-function model for knife-edge scans.
    """
    width = sigma * (1 + alpha * (x - mu))
    width = np.abs(width) + 1e-15

    z = (x - mu) / width
    return A * 0.5 * (1 + erf(z)) + C

def skew_erfc(x, A, mu=0.0, sigma=1.0, alpha=0.0, C=0.0):
    width = sigma * (1 + alpha * (x - mu))
    width = np.abs(width) + 1e-15

    z = (x - mu) / width
    return A * 0.5 * erfc(-z) + C
