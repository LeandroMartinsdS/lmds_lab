import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns
from models import peakmodels, error_func
from xray_aligment import rocking, knife_edge

def test_rocking(add_noise: bool = True, seed: int = 0):
    # If you have real implementations:
    # from models import peakmodels
    # from xray_aligment import rocking
    # theta = np.linspace(0, 10, 1000)
    # counts_std_gaussian = peakmodels.skew_super_gaussian(theta, A=100.0, mu=2.5, sigma=0.5)
    # For this example we’ll generate a synthetic skew super-gaussian:

    rng = np.random.default_rng(seed)
    theta = np.linspace(0, 10, 1000)

    # Ground-truth parameters for synthetic data
    A_true = 100.0
    mu_true = 6
    sigma_true = 0.5
    m_true = 2.0      # gaussian-like
    skew_true = 0.15  # slight asymmetry
    C_true = 20.0      # small baseline

    counts_clean = peakmodels.skew_super_gaussian(theta, A_true, mu_true, sigma_true, m_true, skew_true, C_true)

    if add_noise:
        noise = rng.normal(0.0, 5, size=theta.size)  # adjust noise level if desired
        counts = counts_clean + noise
    else:
        counts = counts_clean

    # ---- Fit using optimize_peak ----
    popt, metrics, fw, model_func = rocking.optimize_peak(theta, counts, model="skew_super_gaussian", prominence_frac=1)
    print("Optimal parameters [A, mu, sigma_like, m, skew, C]:")
    print(np.array2string(popt, precision=4))
    print("Fit metrics on the window:")
    for k, v in metrics.items():
        print(f"  {k}: {v:.5f}")

    # Evaluate the fitted model on full axis (optional, helpful for plotting)
    counts_fit_full = model_func(theta, *popt)

    # ---- Plot data + fit (window highlighted) ----
    sns.set_style("whitegrid")
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 7), sharex=True,
                                   gridspec_kw={"height_ratios": [3, 1]})

    # Top: data and fit
    sns.lineplot(x=theta, y=counts, ax=ax1, color="tab:blue", linewidth=1.5, label="Data")
    sns.lineplot(x=theta, y=counts_fit_full, ax=ax1, color="tab:red", linewidth=2.0, label="Fit (full)")

    # Highlight the fit window
    xw = fw["x"]
    ax1.axvspan(xw[0], xw[-1], color="k", alpha=0.06, label="Fit window")

    ax1.set_xlabel("Theta (deg)")
    ax1.set_ylabel("Counts (a.u.)")
    ax1.set_title("Simulated Rocking Curve with Peak Fit")
    ax1.legend(loc="best")

    # Bottom: residuals on the window
    # Create residuals only on the window for clarity
    resid_w = fw["y"] - fw["y_fit"]
    ax2.plot(xw, resid_w, color="tab:gray", lw=1.5)
    ax2.axhline(0, color="k", lw=1.0, alpha=0.5)
    ax2.set_ylabel("Residuals")
    ax2.set_xlabel("Theta (deg)")

    # Annotate metrics
    text = f"RMSE: {metrics['rmse']:.3f}\nR²: {metrics['r2']:.4f}\nAdj R²: {metrics['adj_r2']:.4f}"
    ax2.text(0.99, 0.95, text, ha="right", va="top", transform=ax2.transAxes,
             bbox=dict(boxstyle="round", fc="white", ec="lightgray"))

    plt.tight_layout()
    plt.show()

def test_knife_edge(add_noise: bool = True, seed: int = 0):
    # If you have real implementations:
    # from models import peakmodels
    # from xray_aligment import rocking
    # theta = np.linspace(0, 10, 1000)
    # counts_std_gaussian = peakmodels.skew_super_gaussian(theta, A=100.0, mu=2.5, sigma=0.5)
    # For this example we’ll generate a synthetic skew super-gaussian:

    rng = np.random.default_rng(seed)
    theta = np.linspace(0, 10, 1000)

    # Ground-truth parameters for synthetic data
    A_true = 100.0
    mu_true = 6
    sigma_true = 0.707
    skew_true = 0.3  # slight asymmetry
    C_true = 20.0      # small baseline

    counts_clean = error_func.skew_erf(theta, A_true, mu_true, sigma_true, skew_true, C_true)

    if add_noise:
        noise = rng.normal(0.0, 10, size=theta.size)  # adjust noise level if desired
        counts = counts_clean + noise
    else:
        counts = counts_clean

    # ---- Fit using optimize_peak ----
    popt, metrics, fw, model_func = knife_edge.fit_knife_edge(theta, counts, model="skew_erf", prominence_frac=0)
    print("Optimal parameters [A, mu, sigma_like, skew, C]:")
    print(np.array2string(popt, precision=4))
    print("Fit metrics on the window:")
    for k, v in metrics.items():
        print(f"  {k}: {v:.5f}")

    # # Evaluate the fitted model on full axis (optional, helpful for plotting)
    counts_fit_full = model_func(theta, *popt)

    # ---- Plot data + fit (window highlighted) ----
    sns.set_style("whitegrid")
    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(8, 7), sharex=True,
                                   gridspec_kw={"height_ratios": [3, 1]})

    # Top: data and fit
    sns.lineplot(x=theta, y=counts, ax=ax1, color="tab:blue", linewidth=1.5, label="Data")
    sns.lineplot(x=theta, y=counts_fit_full, ax=ax1, color="tab:red", linewidth=2.0, label="Fit (full)")

    # Highlight the fit window
    xw = fw["x"]
    ax1.axvspan(xw[0], xw[-1], color="k", alpha=0.06, label="Fit window")

    ax1.set_xlabel("Theta (deg)")
    ax1.set_ylabel("Counts (a.u.)")
    ax1.set_title("Simulated Rocking Curve with Peak Fit")
    ax1.legend(loc="best")

    # Bottom: residuals on the window
    # Create residuals only on the window for clarity
    resid_w = fw["y"] - fw["y_fit"]
    ax2.plot(xw, resid_w, color="tab:gray", lw=1.5)
    ax2.axhline(0, color="k", lw=1.0, alpha=0.5)
    ax2.set_ylabel("Residuals")
    ax2.set_xlabel("Theta (deg)")

    # Annotate metrics
    text = f"RMSE: {metrics['rmse']:.3f}\nR²: {metrics['r2']:.4f}\nAdj R²: {metrics['adj_r2']:.4f}"
    ax2.text(0.99, 0.95, text, ha="right", va="top", transform=ax2.transAxes,
             bbox=dict(boxstyle="round", fc="white", ec="lightgray"))

    plt.tight_layout()
    plt.show()


def main():
    print(f"Hello from {main.__name__}")
    # test_rocking(add_noise=True, seed=2)
    test_knife_edge(add_noise=True, seed=2)


if __name__ == "__main__":
    main()
