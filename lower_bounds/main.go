package main

import (
	"fmt"
	"time"
)

// We're computing L(xi, k) for all 1 <= k < N

// L(xi, k) is a lower bound on the number of sets
// in [k] = {0, ..., k - 1} that are of the form
// A(alpha, (0.5 - xi, 0.5 + xi)) \cap [k] for some
// alpha. Elements in L(xi, k) are subsets of [k] and
// can be represented as paths in a binary tree of depth k.

func main() {
	var (
		now = time.Now()
	)

	elapsed := time.Since(now).Minutes()
	fmt.Println(elapsed)
}
