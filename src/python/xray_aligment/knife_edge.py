import numpy as np
from typing import Callable, Dict, Tuple
from scipy.optimize import curve_fit, least_squares
from scipy.signal import find_peaks, peak_widths

from models.error_func import skew_erf

PrepResult = Tuple[Callable, list, Tuple[list, list]]  # (func, p0, bounds)
#def skew_erf(x, A, mu=0.0, sigma=1.0, alpha=0.0, C=0.0):

def _prep_skew_erf(y, center_x, width_est, C_guess) -> PrepResult:
    func = skew_erf
    # Parameter order: [A, mu, sigma_like, m, skew, C]
    A0 = max(float(np.max(y) - C_guess), 1e-6)
    sigma0 = max(float(width_est / 1.665), 1e-9)  # guarded width
    skew0 = 0.0
    p0 = [A0, center_x, sigma0, skew0, float(C_guess)]

    # Reasonable bounds to keep search well-posed
    lower = [0.0,   -np.inf, 1e-12,  -2.0, -np.inf]
    upper = [np.inf, np.inf,  np.inf, 2.0,  np.inf]
    bounds = (lower, upper)
    return func, p0, bounds

_MODEL_PREP: Dict[str, Callable] = {
    "skew_erf": _prep_skew_erf
}

def _goodness_of_fit(y_obs: np.ndarray, y_fit: np.ndarray, n_params: int):
    y_obs = np.asarray(y_obs, dtype=float)
    y_fit = np.asarray(y_fit, dtype=float)
    if y_obs.shape != y_fit.shape or y_obs.size == 0:
        return {"rmse": np.nan, "r2": np.nan, "adj_r2": np.nan}

    residuals = y_obs - y_fit
    ss_res = float(np.sum(residuals ** 2))
    ss_tot = float(np.sum((y_obs - np.mean(y_obs)) ** 2))
    r2 = 1.0 - ss_res / ss_tot if ss_tot > 0 else np.nan
    rmse = np.sqrt(ss_res / y_obs.size)
    denom = (y_obs.size - n_params - 1)
    adj_r2 = 1.0 - (1.0 - r2) * ((y_obs.size - 1) / denom) if (denom > 0 and np.isfinite(r2)) else np.nan
    return {"rmse": float(rmse), "r2": float(r2), "adj_r2": float(adj_r2)}

def fit_knife_edge(x: np.ndarray,
                   y: np.ndarray,
                   model: str = "skew_erf",
                   prominence_frac: float = 0.1,
                   window_scale: float = 3.0,
                   max_nfev: int = 100000):
    """
    Robust peak fit with a two-stage strategy:
      Stage 1: fit [A, mu, sigma, C] with m=2, skew=0 using a reduced model
      Stage 2: free all params [A, mu, sigma, m, skew, C], start from Stage 1
    Returns:
        popt, metrics, fit_window, model_func
    """
    # ---- 0) Sanitize input ----
    x = np.asarray(x, dtype=float)
    y = np.asarray(y, dtype=float)
    mask_finite = np.isfinite(x) & np.isfinite(y)
    if not np.all(mask_finite):
        x = x[mask_finite]; y = y[mask_finite]

    if x.ndim != 1 or y.ndim != 1 or x.size != y.size or x.size < 5:
        raise ValueError("x and y must be 1D arrays of same length (>=5)")

    # Ensure strictly increasing x
    if not np.all(np.diff(x) > 0):
        idx = np.argsort(x)
        x, y = x[idx], y[idx]

    # ---- 1) Peak + width ----
    dy = np.gradient(y, x)
    prom = prominence_frac * (np.percentile(y, 95) - np.percentile(y, 5))
    prom = max(prom, 1e-12)  # avoid zero
    peaks, props = find_peaks(y, prominence=prom)

    if len(peaks) == 0:
        raise RuntimeError("No peaks found with given prominence threshold.")
    peak = peaks[0]
    # center_x = x[peak]
    center_x = 0.0
    widths, _, left, right = peak_widths(dy, peaks, rel_height=0.5)
    li = int(np.clip(left[0], 0, x.size - 1))
    ri = int(np.clip(right[0], 0, x.size - 1))
    width_est = x[ri] - x[li]
    # Guard minimum width ~ a few bins
    dx = float(np.mean(np.diff(x)))
    width_est = float(max(width_est, 3.0 * dx))

    window = float(window_scale) * width_est
    window_mask = (x > center_x - window) & (x < center_x + window)
    if np.count_nonzero(window_mask) < 10:
        # Expand once if too few points
        window_mask = (x > center_x - 2 * window) & (x < center_x + 2 * window)

    xw = x[window_mask]
    yw = y[window_mask]
    # HACK
    xw = x
    yw = y

    # ---- 2) Baseline guess (local) ----
    # C_guess = float(np.percentile(yw, 5)) if yw.size > 0 else float(np.min(y))
    C_guess = np.min(yw)
    # C_guess = 0.0

    # ---- 3) Prep model and initial params ----
    try:
        func, p0, bounds = _MODEL_PREP[model](y, center_x, width_est, C_guess)
    except KeyError:
        raise ValueError(f"Unknown model '{model}'. Available: {list(_MODEL_PREP)}")

    # ---- 4) Stage 1: reduced model with fixed m=2, skew=0 ----
    skew_fixed = 0.0

    def func_s1(x_in, A, mu, sigma, C):
        # Reduced 4-parameter model
        return func(x_in, A, mu, sigma, skew_fixed, C)

    # Build p0 and bounds for the reduced parameter set [A, mu, sigma, C]
    p0_s1 = [p0[0], p0[1], p0[2], p0[4]]
    bounds_s1 = ([bounds[0][0], bounds[0][1], bounds[0][2], bounds[0][4]],
                 [bounds[1][0], bounds[1][1], bounds[1][2], bounds[1][4]])

    try:
        popt_s1, _ = curve_fit(
            func_s1, xw, yw, p0=p0_s1, bounds=bounds_s1,
            method='trf', max_nfev=max_nfev
        )
    except RuntimeError:
        # Fallback: try dogbox
        popt_s1, _ = curve_fit(
            func_s1, xw, yw, p0=p0_s1, bounds=bounds_s1,
            method='dogbox', max_nfev=max_nfev
        )

    # Compose starting point for Stage 2: [A, mu, sigma, skew, C]
    p0_s2 = [popt_s1[0], popt_s1[1], popt_s1[2], skew_fixed, popt_s1[3]]

    # ---- 5) Stage 2: full model ----
    try:
        popt, _ = curve_fit(
            func, xw, yw, p0=p0_s2, bounds=bounds,
            method='trf', max_nfev=max_nfev
        )
    except RuntimeError:
        # Robust fallback using soft_l1 to handle outliers/noise
        def residuals(p):
            return func(xw, *p) - yw
        res = least_squares(
            residuals, x0=p0_s2, bounds=bounds,
            method='trf', max_nfev=max_nfev,
            loss='soft_l1', f_scale=1.0
        )
        popt = res.x

    # ---- 6) Metrics on the window ----
    yhat_w = func(xw, *popt)
    metrics = _goodness_of_fit(yw, yhat_w, n_params=len(popt))
    fit_window = {"x": xw, "y": yw, "y_fit": yhat_w, "mask": window_mask}
    return popt, metrics, fit_window, func
