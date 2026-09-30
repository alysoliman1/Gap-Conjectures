/*
 * open_problem_2_fast.c — Open Problem 2, C + OpenMP
 *
 * Computes max(U(T^k(A_n(α,E)))) for every (α, E) combination from random samples.
 *
 * Compile — Linux / GCC:
 *   gcc -O3 -march=native -fopenmp -lm -o open_problem_2_fast open_problem_2_fast.c
 *
 * Compile — macOS, Homebrew GCC (recommended):
 *   brew install gcc
 *   gcc-14 -O3 -march=native -fopenmp -lm -o open_problem_2_fast open_problem_2_fast.c
 *
 * Compile — macOS, Apple Clang + libomp:
 *   brew install libomp
 *   clang -O3 -march=native -Xpreprocessor -fopenmp -lomp -lm \
 *         -I$(brew --prefix libomp)/include -L$(brew --prefix libomp)/lib \
 *         -o open_problem_2_fast open_problem_2_fast.c
 *
 * Usage:
 *   ./open_problem_2_fast --alphas K --intervals M [--n N] [--max-k K] [--seed S] [--workers W]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <getopt.h>
#include <omp.h>

/* ── RNG: splitmix64 (fast, high quality, trivially seedable) ────────────── */

static uint64_t sm64_next(uint64_t *s) {
    uint64_t z = (*s += UINT64_C(0x9e3779b97f4a7c15));
    z = (z ^ (z >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    z = (z ^ (z >> 27)) * UINT64_C(0x94d049bb133111eb);
    return z ^ (z >> 31);
}

/* uniform double in [0, 1) */
static double rng_f64(uint64_t *s) {
    return (sm64_next(s) >> 11) * (1.0 / (double)(UINT64_C(1) << 53));
}

/* ── Core operators ──────────────────────────────────────────────────────── */

/*
 * circle_encoding — fill out[0..n-1] with 1 if (i·α mod 1) ∈ [a,b), else 0.
 * i runs 1..n.  Handles wrap-around (a > b): interval = [a,1) ∪ [0,b).
 */
static void circle_encoding(double alpha, double a, double b,
                             int n, int8_t *out) {
    if (a <= b) {
        for (int i = 1; i <= n; i++) {
            double pt = fmod((double)i * alpha, 1.0);
            out[i-1] = (pt >= a && pt < b) ? 1 : 0;
        }
    } else {
        for (int i = 1; i <= n; i++) {
            double pt = fmod((double)i * alpha, 1.0);
            out[i-1] = (pt >= a || pt < b) ? 1 : 0;
        }
    }
}

/* unique count for int8_t (bool) array — branchless early exit */
static int unique_bool(const int8_t *g, int n) {
    int has0 = 0, has1 = 0;
    for (int i = 0; i < n; i++) {
        has1 |= g[i];
        has0 |= !g[i];
        if (has0 & has1) return 2;
    }
    return has0 + has1;
}

/*
 * unique_int — count distinct values in g[0..n-1].
 * `seen` is a caller-managed zero array of size >= n+1 (values bounded by n-1).
 * Resets only the touched entries before returning — O(n) total.
 */
static int unique_int(const int *g, int n, int8_t *seen) {
    int count = 0;
    for (int i = 0; i < n; i++) {
        if (!seen[g[i]]) { seen[g[i]] = 1; count++; }
    }
    for (int i = 0; i < n; i++) seen[g[i]] = 0;
    return count;
}

/*
 * recurring_bool — indices i where g[i] == g[i+1].
 * Returns count of such indices written into out[].
 */
static int recurring_bool(const int8_t *g, int n, int *out) {
    int c = 0;
    for (int i = 0; i < n - 1; i++)
        if (g[i] == g[i+1]) out[c++] = i;
    return c;
}

static int recurring_int(const int *g, int n, int *out) {
    int c = 0;
    for (int i = 0; i < n - 1; i++)
        if (g[i] == g[i+1]) out[c++] = i;
    return c;
}

/*
 * gaps — consecutive differences of r[0..nr-1].
 * Returns nr-1 (count of differences written into out[]).
 */
static int gaps(const int *r, int nr, int *out) {
    if (nr < 2) return 0;
    for (int i = 0; i < nr - 1; i++) out[i] = r[i+1] - r[i];
    return nr - 1;
}

/*
 * max_u_tk — main per-task computation.
 *
 * Returns max over k = 0..max_k of U(T^k(A_n(alpha, [a,b)))).
 *
 * All scratch buffers are supplied by the caller (pre-allocated, size >= n).
 * This function performs zero heap allocation.
 */
static int max_u_tk(double alpha, double a, double b, int n, int max_k,
                    int8_t *gbool,          /* circle encoding scratch  */
                    int    *rbuf,           /* recurring output scratch  */
                    int    *buf0, int *buf1,/* two alternating T buffers */
                    int    *r2buf,          /* recurring scratch for T^k */
                    int8_t *seen)           /* unique-count scratch      */
{
    /* k = 0 */
    circle_encoding(alpha, a, b, n, gbool);
    int max_u = unique_bool(gbool, n);

    /* first T: recurring on bool → gaps */
    int nr   = recurring_bool(gbool, n, rbuf);
    int *cur = buf0, *nxt = buf1;
    int ncur = gaps(rbuf, nr, cur);

    for (int k = 1; ncur > 0; k++) {
        int u = unique_int(cur, ncur, seen);
        if (u > max_u) max_u = u;
        if (k >= max_k) break;

        /* next T */
        int nr2  = recurring_int(cur, ncur, r2buf);
        int nnxt = gaps(r2buf, nr2, nxt);

        /* swap cur ↔ nxt (pointer swap, no data copy) */
        int *tmp = cur; cur = nxt; nxt = tmp;
        ncur = nnxt;
    }
    return max_u;
}

/* ── Result ──────────────────────────────────────────────────────────────── */

typedef struct { double alpha, a, b; int max_u; } Result;

static int cmp_result(const void *x, const void *y) {
    const Result *rx = (const Result *)x, *ry = (const Result *)y;
    if (rx->alpha != ry->alpha) return (rx->alpha < ry->alpha) ? -1 : 1;
    if (rx->a     != ry->a)     return (rx->a     < ry->a)     ? -1 : 1;
    return 0;
}

/* ── CLI ─────────────────────────────────────────────────────────────────── */

static void usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s --alphas K --intervals M [options]\n\n"
        "  --alphas    K   Number of random alpha values        (required)\n"
        "  --intervals M   Number of random intervals E         (required)\n"
        "  --n         N   Initial sequence length              (default: 5000)\n"
        "  --max-k     K   Max T iterations                     (default: 40)\n"
        "  --seed      S   RNG seed                             (default: 42)\n"
        "  --workers   W   OpenMP thread count                  (default: all cores)\n",
        prog);
}

int main(int argc, char *argv[]) {
    int      n_alphas    = -1;
    int      n_intervals = -1;
    int      n           = 5000;
    int      max_k       = 40;
    uint64_t seed        = 42;
    int      workers     = omp_get_max_threads();

    static struct option long_opts[] = {
        {"alphas",    required_argument, 0, 'A'},
        {"intervals", required_argument, 0, 'I'},
        {"n",         required_argument, 0, 'N'},
        {"max-k",     required_argument, 0, 'K'},
        {"seed",      required_argument, 0, 'S'},
        {"workers",   required_argument, 0, 'W'},
        {0, 0, 0, 0}
    };

    int opt, idx;
    while ((opt = getopt_long(argc, argv, "", long_opts, &idx)) != -1) {
        switch (opt) {
            case 'A': n_alphas    = atoi(optarg);          break;
            case 'I': n_intervals = atoi(optarg);          break;
            case 'N': n           = atoi(optarg);          break;
            case 'K': max_k       = atoi(optarg);          break;
            case 'S': seed        = (uint64_t)atoll(optarg); break;
            case 'W': workers     = atoi(optarg);          break;
            default:  usage(argv[0]); return 1;
        }
    }
    if (n_alphas < 1 || n_intervals < 1) {
        fprintf(stderr, "Error: --alphas and --intervals are required (>= 1)\n");
        usage(argv[0]); return 1;
    }

    omp_set_num_threads(workers);

    /* ── Sample random alphas and intervals ─────────────────────────────── */
    uint64_t rng = seed;

    double *alphas = malloc(n_alphas    * sizeof(double));
    double *starts = malloc(n_intervals * sizeof(double));
    double *sizes  = malloc(n_intervals * sizeof(double));
    if (!alphas || !starts || !sizes) { perror("malloc"); return 1; }

    for (int i = 0; i < n_alphas;    i++) alphas[i] = rng_f64(&rng);
    for (int i = 0; i < n_intervals; i++) starts[i] = rng_f64(&rng);
    for (int i = 0; i < n_intervals; i++) sizes[i]  = 0.01 + rng_f64(&rng) * 0.98;

    int     total   = n_alphas * n_intervals;
    Result *results = malloc(total * sizeof(Result));
    if (!results) { perror("malloc"); return 1; }

    printf("Open Problem 2 (C+OpenMP) — alphas=%d, intervals=%d, n=%d, "
           "max_k=%d, seed=%llu, workers=%d\n",
           n_alphas, n_intervals, n, max_k, (unsigned long long)seed, workers);
    printf("Total combinations: %d\n", total);
    printf("============================================================\n");

    double t0 = omp_get_wtime();

    /* ── Parallel computation ────────────────────────────────────────────── */
#pragma omp parallel
    {
        /* Each thread owns its scratch buffers — allocated once, reused per job */
        int8_t *gbool = malloc(n       * sizeof(int8_t));
        int    *rbuf  = malloc(n       * sizeof(int));
        int    *buf0  = malloc(n       * sizeof(int));
        int    *buf1  = malloc(n       * sizeof(int));
        int    *r2buf = malloc(n       * sizeof(int));
        int8_t *seen  = calloc(n + 1,    sizeof(int8_t));  /* zero-init required */

        if (!gbool || !rbuf || !buf0 || !buf1 || !r2buf || !seen) {
            fprintf(stderr, "thread malloc failed\n"); exit(1);
        }

#pragma omp for schedule(dynamic, 4)
        for (int i = 0; i < total; i++) {
            int ai = i / n_intervals;
            int ii = i % n_intervals;

            double alpha = alphas[ai];
            double a     = starts[ii];
            double b     = fmod(a + sizes[ii], 1.0);

            results[i] = (Result){
                .alpha = alpha, .a = a, .b = b,
                .max_u = max_u_tk(alpha, a, b, n, max_k,
                                  gbool, rbuf, buf0, buf1, r2buf, seen)
            };
        }

        free(gbool); free(rbuf); free(buf0); free(buf1); free(r2buf); free(seen);
    }

    double elapsed = omp_get_wtime() - t0;

    /* ── Sort by (alpha, a) for deterministic output ─────────────────────── */
    qsort(results, total, sizeof(Result), cmp_result);

    printf("%-14s  %-22s  %12s\n", "α", "E = [a, b)", "max U(T^k)");
    printf("------------------------------------------------------\n");

    int overall_max = 0;
    for (int i = 0; i < total; i++) {
        printf("%14.8f  [%7.4f, %7.4f)  %12d\n",
               results[i].alpha, results[i].a, results[i].b, results[i].max_u);
        if (results[i].max_u > overall_max) overall_max = results[i].max_u;
    }

    printf("============================================================\n");
    printf("Overall max U(T^k) across all combinations: %d\n", overall_max);
    printf("Done in %.3fs\n", elapsed);

    /* ── Alphas achieving U(T^k) > 5 ────────────────────────────────────── */
    printf("\n── Alphas with max U(T^k) >= 5 ─────────────────────────────\n");
    int found = 0;
    for (int i = 0; i < total; i++) {
        if (results[i].max_u >= 5) {
            printf("  α = %.8f  E = [%.4f, %.4f)  max U = %d\n",
                   results[i].alpha, results[i].a, results[i].b, results[i].max_u);
            found++;
        }
    }
    if (!found) printf("  (none)\n");

    free(alphas); free(starts); free(sizes); free(results);
    return 0;
}
