#!/usr/bin/env python3
"""
Open Problem 2 — Tester & Visualizer
=====================================
Conjecture: There exists a constant C (independent of E, n, k) such that
    U(T^k(A_n(α, E))) ≤ C
where:
  A_n(α, E)  — binary circle encoding  (1 if i·α mod 1 ∈ E, else 0)
  R(G)       — recurring seq: indices where G[i+1] == G[i]
  Gaps(R)    — gaps seq:      consecutive differences of R
  T          — Gaps ∘ R
  U(G)       — number of distinct values in G
"""

import argparse
import sys
import time
import numpy as np
import matplotlib
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from matplotlib.colors import LinearSegmentedColormap
from typing import NamedTuple

# ── Core operators ─────────────────────────────────────────────────────────────

def circle_encoding(alpha: float, a: float, b: float, n: int) -> np.ndarray:
    """A_n(α, [a,b)): binary array length n.  i = 1..n."""
    pts = (np.arange(1, n + 1, dtype=np.float64) * alpha) % 1.0
    if a <= b:
        return (pts >= a) & (pts < b)
    else:  # interval wraps around: [a,1) ∪ [0,b)
        return (pts >= a) | (pts < b)


def recurring(G: np.ndarray) -> np.ndarray:
    """R(G): sorted indices 0 ≤ i ≤ |G|-2 where G[i+1] == G[i]."""
    if len(G) < 2:
        return np.empty(0, dtype=np.int64)
    return np.flatnonzero(G[:-1] == G[1:]).astype(np.int64)


def gaps(R: np.ndarray) -> np.ndarray:
    """Gaps(R): consecutive differences — always positive when R is sorted."""
    if len(R) < 2:
        return np.empty(0, dtype=np.int64)
    return np.diff(R)


def T_op(G: np.ndarray) -> np.ndarray:
    """T = Gaps ∘ R."""
    return gaps(recurring(G))


def get_T_k(G: np.ndarray, k: int) -> np.ndarray:
    """Return the sequence T^k(G)."""
    cur = G
    for _ in range(k):
        cur = T_op(cur)
        if cur.size == 0:
            break
    return cur


def iterate_T(G: np.ndarray, max_k: int):
    """
    Apply T up to max_k times (stop early if sequence collapses).
    Returns (unique_counts, seq_lengths), both 1-D int arrays of length ≤ max_k+1.
    """
    u_counts = [int(np.unique(G).size)]
    lengths  = [len(G)]
    cur = G
    for _ in range(max_k):
        cur = T_op(cur)
        if cur.size == 0:
            break
        u_counts.append(int(np.unique(cur).size))
        lengths.append(cur.size)
    return np.array(u_counts, dtype=np.int32), np.array(lengths, dtype=np.int64)


# ── Named alphas ───────────────────────────────────────────────────────────────

ALPHAS = {
    "φ−1":   (1 + 5**0.5) / 2 - 1,   # ≈ 0.618034
    "√2−1":  2**0.5 - 1,              # ≈ 0.414214
    "√3−1":  3**0.5 - 1,              # ≈ 0.732051
    "π−3":   np.pi - 3,               # ≈ 0.141593
    "1/e":   1 / np.e,                # ≈ 0.367879
    "ln 2":  np.log(2),               # ≈ 0.693147
}


# ── Experiments ────────────────────────────────────────────────────────────────

def experiment_u_vs_k(alpha: float, a: float, b: float, n: int, max_k: int):
    """Return (u_counts, lengths) for one (α, E, n) triple."""
    G = circle_encoding(alpha, a, b, n)
    return iterate_T(G, max_k)


def sweep_interval_sizes(alpha: float, n: int, max_k: int,
                          num_sizes: int = 20, a_start: float = 0.0):
    """
    For a range of interval sizes |E| = len, with E = [a_start, a_start+len),
    return a matrix  max_U[i] = max_k U(T^k(A_n(α, E_i))).
    """
    sizes  = np.linspace(0.02, 0.98, num_sizes)
    max_us = np.zeros(num_sizes, dtype=np.int32)
    for i, sz in enumerate(sizes):
        b = (a_start + sz) % 1.0
        a = a_start % 1.0
        u, _ = experiment_u_vs_k(alpha, a, b, n, max_k)
        max_us[i] = u.max()
    return sizes, max_us


def sweep_alpha_and_size(alphas: list, n: int, max_k: int,
                          num_sizes: int = 40, num_starts: int = 8):
    """
    For each α and a grid of interval sizes, compute max_k U(T^k).
    Returns dict  alpha_name → (sizes, max_u_array).
    """
    results = {}
    starts = np.linspace(0.0, 1.0, num_starts, endpoint=False)
    for name, alpha in alphas.items():
        sizes  = np.linspace(0.02, 0.98, num_sizes)
        max_us = np.zeros(num_sizes, dtype=np.int32)
        for i, sz in enumerate(sizes):
            # average over multiple interval starts to reduce bias
            vals = []
            for a0 in starts:
                b = (a0 + sz) % 1.0
                u, _ = experiment_u_vs_k(alpha, a0, b, n, max_k)
                vals.append(u.max())
            max_us[i] = max(vals)
        results[name] = (sizes, max_us)
    return results


def heatmap_data(alpha: float, n: int, max_k: int,
                 grid: int = 60):
    """
    2-D grid: rows = interval_start ∈ [0,1), cols = interval_size ∈ (0,1).
    Value = max over k of U(T^k(A_n(α, E))).
    Returns (starts, sizes, Z) where Z.shape = (grid, grid).
    """
    starts = np.linspace(0.0, 1.0, grid, endpoint=False)
    sizes  = np.linspace(0.02, 0.98, grid)
    Z = np.zeros((grid, grid), dtype=np.int32)
    for i, a in enumerate(starts):
        for j, sz in enumerate(sizes):
            b = (a + sz) % 1.0
            u, _ = experiment_u_vs_k(alpha, a, b, n, max_k)
            Z[i, j] = u.max()
    return starts, sizes, Z


# ── Plotting ───────────────────────────────────────────────────────────────────

PALETTE = plt.cm.tab10.colors

def fig_u_vs_k(alpha_name: str, alpha: float, n: int, max_k: int,
               interval_sizes=(0.1, 0.2, 0.3, 0.4, 0.5, 0.618)):
    """
    Line plot: U(T^k) vs k for several interval sizes.
    Also plots sequence length on a twin axis.
    """
    fig, axes = plt.subplots(1, 2, figsize=(14, 5))
    fig.suptitle(
        f"Open Problem 2 — α = {alpha_name} ({alpha:.6f}),  n = {n}",
        fontsize=13, fontweight="bold"
    )
    ax_u, ax_len = axes

    for idx, sz in enumerate(interval_sizes):
        a = 0.0
        b = sz
        u, lengths = experiment_u_vs_k(alpha, a, b, n, max_k)
        ks = np.arange(len(u))
        col = PALETTE[idx % len(PALETTE)]
        label = f"|E| = {sz:.3f}"
        ax_u.plot(ks, u, marker="o", markersize=3, color=col,
                  linewidth=1.5, label=label)
        ax_len.semilogy(ks, lengths, marker="s", markersize=3,
                        color=col, linewidth=1.5, label=label)

    ax_u.set_xlabel("k  (number of T applications)")
    ax_u.set_ylabel("U(Tᵏ(·))  — unique element count")
    ax_u.set_title("Unique counts vs iteration k")
    ax_u.legend(fontsize=8, ncol=2)
    ax_u.grid(alpha=0.3)
    ax_u.yaxis.get_major_locator().set_params(integer=True)

    ax_len.set_xlabel("k")
    ax_len.set_ylabel("Sequence length  (log scale)")
    ax_len.set_title("Sequence length shrinkage")
    ax_len.legend(fontsize=8, ncol=2)
    ax_len.grid(alpha=0.3, which="both")

    plt.tight_layout()
    return fig


def fig_max_u_vs_size(results: dict, n: int, max_k: int):
    """
    For each α: max_k U vs interval size.
    """
    fig, ax = plt.subplots(figsize=(12, 5))
    fig.suptitle(
        f"Open Problem 2 — max_k U(Tᵏ) vs |E|   (n={n}, max_k={max_k})",
        fontsize=13, fontweight="bold"
    )
    for idx, (name, (sizes, max_us)) in enumerate(results.items()):
        ax.plot(sizes, max_us, linewidth=2, marker=".",
                color=PALETTE[idx % len(PALETTE)], label=f"α = {name}")

    ax.set_xlabel("|E|  (interval size)")
    ax.set_ylabel("max_k  U(Tᵏ(A_n(α,E)))")
    ax.set_title("Is max_k U bounded independently of |E| and α?")
    ax.legend(fontsize=9)
    ax.grid(alpha=0.3)
    ax.yaxis.get_major_locator().set_params(integer=True)
    plt.tight_layout()
    return fig


def fig_heatmap(alpha_name: str, alpha: float, n: int, max_k: int,
                starts, sizes, Z):
    """
    2-D heatmap: x = interval size, y = interval start,
    colour = max_k U(T^k).
    """
    fig, ax = plt.subplots(figsize=(9, 7))
    fig.suptitle(
        f"Open Problem 2 Heatmap — α={alpha_name},  n={n},  max_k={max_k}",
        fontsize=13, fontweight="bold"
    )
    cmap = LinearSegmentedColormap.from_list(
        "cool_warm", ["#2b83ba", "#ffffbf", "#d7191c"], N=256
    )
    im = ax.pcolormesh(sizes, starts, Z, cmap=cmap, shading="auto")
    cb = fig.colorbar(im, ax=ax, label="max_k  U(Tᵏ(A_n(α,E)))")
    cb.ax.yaxis.set_major_locator(plt.MaxNLocator(integer=True))
    ax.set_xlabel("|E|  (interval size)")
    ax.set_ylabel("interval start  a")
    ax.set_title("Each cell = max unique count over all k")
    plt.tight_layout()
    return fig


def fig_distribution(alpha_name: str, alpha: float,
                     ns: list, max_k: int, num_samples: int = 200):
    """
    For several n values, sample random (a, size) pairs and
    collect all U(T^k) values → histogram of observed unique counts.
    Helps answer: is C ≤ some small integer?
    """
    fig, axes = plt.subplots(1, len(ns), figsize=(5 * len(ns), 4), sharey=True)
    fig.suptitle(
        f"Distribution of U(Tᵏ) — α={alpha_name}  (all k, n values, random E)",
        fontsize=12, fontweight="bold"
    )
    rng = np.random.default_rng(42)

    for ax, n in zip(np.atleast_1d(axes), ns):
        all_u = []
        starts = rng.uniform(0, 1, num_samples)
        sizes  = rng.uniform(0.01, 0.99, num_samples)
        for a0, sz in zip(starts, sizes):
            b = (a0 + sz) % 1.0
            u, _ = experiment_u_vs_k(alpha, a0, b, n, max_k)
            all_u.extend(u.tolist())
        all_u = np.array(all_u)
        vals, cnts = np.unique(all_u, return_counts=True)
        ax.bar(vals, cnts / cnts.sum(), color="#4878d0", edgecolor="k",
               linewidth=0.5, width=0.7)
        ax.set_xlabel("U(Tᵏ)")
        ax.set_ylabel("Relative frequency" if n == ns[0] else "")
        ax.set_title(f"n = {n}")
        ax.set_xticks(vals)
        ax.grid(axis="y", alpha=0.3)
        ax.annotate(f"max = {all_u.max()}", xy=(0.98, 0.95),
                    xycoords="axes fraction", ha="right", fontsize=9,
                    color="crimson", fontweight="bold")
    plt.tight_layout()
    return fig


def fig_global_max_vs_n(alpha_name: str, alpha: float,
                         ns: list, max_k: int,
                         num_samples: int = 150):
    """
    Shows how the empirical max of U(T^k) over random E grows (or not) with n.
    """
    rng = np.random.default_rng(0)
    global_maxes = []
    for n in ns:
        mx = 0
        for _ in range(num_samples):
            a0 = rng.uniform(0, 1)
            sz = rng.uniform(0.01, 0.99)
            b  = (a0 + sz) % 1.0
            u, _ = experiment_u_vs_k(alpha, a0, b, n, max_k)
            if u.max() > mx:
                mx = u.max()
        global_maxes.append(mx)

    fig, ax = plt.subplots(figsize=(8, 4))
    fig.suptitle(
        f"Does max U grow with n? — α={alpha_name}",
        fontsize=12, fontweight="bold"
    )
    ax.plot(ns, global_maxes, "o-", color="#e6550d", linewidth=2, markersize=6)
    ax.set_xlabel("n  (sequence length)")
    ax.set_ylabel("Empirical max of U(Tᵏ)  over random E and all k")
    ax.set_xscale("log")
    ax.grid(alpha=0.3, which="both")
    ax.yaxis.get_major_locator().set_params(integer=True)
    ax.axhline(y=global_maxes[-1], color="gray", linestyle="--",
               linewidth=1, label=f"last value = {global_maxes[-1]}")
    ax.legend()
    plt.tight_layout()
    return fig


# ── CLI ────────────────────────────────────────────────────────────────────────

def parse_args():
    p = argparse.ArgumentParser(
        description="Open Problem 2: compute max(U(T^k)) over random α × E combinations."
    )
    # ── random sweep mode ────────────────────────────────────────────────────
    p.add_argument("--alphas", type=int, default=None, metavar="K",
                   help="Number of random α values to sample from (0,1)")
    p.add_argument("--intervals", type=int, default=None, metavar="M",
                   help="Number of random intervals E to sample")
    p.add_argument("--seed", type=int, default=42,
                   help="RNG seed for reproducibility (default: 42)")
    # ── single test mode ─────────────────────────────────────────────────────
    p.add_argument("--alpha", type=float, default=None,
                   help="Single α value to test")
    p.add_argument("--a", type=float, default=None,
                   help="Interval start a  (requires --alpha and --b)")
    p.add_argument("--b", type=float, default=None,
                   help="Interval end b    (requires --alpha and --a)")
    # ── shared ───────────────────────────────────────────────────────────────
    p.add_argument("--n", type=int, default=5000,
                   help="Initial sequence length (default: 5000)")
    p.add_argument("--max-k", type=int, default=40,
                   help="Max number of T iterations (default: 40)")
    return p.parse_args()


def main():
    args = parse_args()

    # ── single test mode ─────────────────────────────────────────────────────
    if args.alpha is not None:
        if args.a is None or args.b is None:
            print("Error: --alpha requires both --a and --b")
            return
        G = circle_encoding(args.alpha, args.a, args.b, args.n)
        u, _ = iterate_T(G, args.max_k)
        k_max = int(u.argmax())
        seq   = get_T_k(G, k_max)
        unique_vals = sorted(int(v) for v in np.unique(seq))
        print(f"α={args.alpha},  E=[{args.a}, {args.b}),  n={args.n},  max_k={args.max_k}")
        print(f"max U(T^k) = {u.max()}  (achieved at k={k_max})")
        print(f"Unique values in T^{k_max}: {unique_vals}")
        return

    # ── random sweep mode ────────────────────────────────────────────────────
    if args.alphas is None or args.intervals is None:
        print("Error: provide either --alpha --a --b (single test) "
              "or --alphas --intervals (random sweep)")
        return

    rng    = np.random.default_rng(args.seed)
    alphas = rng.uniform(0.0, 1.0, args.alphas)
    starts = rng.uniform(0.0, 1.0, args.intervals)
    sizes  = rng.uniform(0.01, 0.99, args.intervals)

    t0 = time.time()
    print(f"Open Problem 2 — alphas={args.alphas}, intervals={args.intervals}, "
          f"n={args.n}, max_k={args.max_k}, seed={args.seed}")
    print("=" * 60)
    print(f"{'α':>14}  {'E = [a, b)':>22}  {'max U(T^k)':>12}")
    print("-" * 54)

    overall_max = 0
    for alpha in alphas:
        for a, sz in zip(starts, sizes):
            b = (a + sz) % 1.0
            u, _ = experiment_u_vs_k(alpha, a, b, args.n, args.max_k)
            mx = u.max()
            overall_max = max(overall_max, mx)
            print(f"{alpha:>14.8f}  [{a:.4f}, {b:.4f})  {mx:>12}")

    print("=" * 60)
    print(f"Overall max U(T^k) across all combinations: {overall_max}")
    print(f"Done in {time.time()-t0:.1f}s")


if __name__ == "__main__":
    main()
