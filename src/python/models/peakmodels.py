import numpy as np


def skew_super_gaussian(x, A, mu=0.0, sigma=1.0, p=2, alpha=0.0, C=0.0, verbose=True):
    """
    Skewed super-Gaussian (generalized Gaussian) peak model.
    """
    width = sigma * (1 + alpha * (x - mu))
    width = np.abs(width) + 1e-15
    exponent = (np.abs(x - mu) / width)**p
    return A * np.exp(-exponent) + C


def skew_super_lorentzian(x, A, mu=0.0, gamma=1.0, p=2.0, alpha=0.0, C=0.0, verbose=True):
    """
    Skewed super-Lorentzian (generalized Lorentzian) peak model.
    """

    # Asymmetric width modulation
    width = gamma * (1.0 + alpha * (x - mu))

    # Numerical stabilization (preserve asymmetry)
    width = np.abs(width) + 1e-15

    # Generalized Lorentzian exponent
    exponent = (np.abs(x - mu) / width)**p

    return A / (1.0 + exponent) + C
