For a sequence $G$, let $|G|$ denote the length of $G$ and let $U(G)$ denote the number of unique elements in $G$. If $G=(g_i)_{i=0}^{n-1}$ is a sequence of integers then let $S(G)=\sum_{i=0}^{n-1}g_i$.
#### Recurring Sequence
* Given a sequence $G=(g_i)_{i=0}^{g-1}$  of length $g\geq 1$ over some finite alphabet $\Sigma$, define the sequence $\mathcal{R}(G)=(r_i)_{i=0}^{n-1}$ as the sequence of indices $0\leq i\leq g-2$ such that $g_{i+1}=g_i$.
* We call $\mathcal{R}(G)$ the ***recurring sequence*** of $G$.
* For example, for $G=(1,2,2,1,0,1,1)$, we get $\mathcal{R}(G)=(1,5)$. 
* Note that $|\mathcal{R}(G)|\leq |G|-1$.
#### Gaps Sequence
* Given a sequence $R=(r_i)_{i=0}^{n-1}$ of length $n$, define the sequence $\mathcal{G}(R)=(r_{i+1}-r_{i})_{i=0}^{n-2}$ of length $n-1$. 
* We call $\mathcal{G}(R)$ the ***gaps sequence*** of $R$. 
#### The T operator
* Define the $T$ operator as $T=\mathcal{G}\circ\mathcal{R}$. 
* Note that $|T(G)|\leq |G|-2$.
#### Circle Encodings
* Given a real number $\alpha$ and a collection of intervals $\{I_i\}_{i=0}^m$ of intervals $I_i$ partitioning $[0,1]$, define the sequence $A_n(\alpha,\{I_i\})_{i=0}^m)=(a_i)_{i=1}^n$ where $a_i=a$ if $i\alpha \in I_a$. We call $A_n(\alpha,\{I_i\})_{i=0}^m)$ a circle encoding of the intervals $\{I_i\}_{i=0}^m$.
#### Open Problem 1
* Let $E\subset [0,1]$ be an interval and let $A_n(\alpha,E)=A_n(\alpha,\{E,E^C\})$
* There exists a constant $C$ independent of $E,n,k$ such that $T^k(A_n(\alpha,E))$ is the circle encoding of at most $C$ intervals.
#### Open Problem 2
* Let $E\subset [0,1]$ be an interval and let $A_n(\alpha,E)=A_n(\alpha,\{E,E^C\})$
* There exists a constant $C$ independent of $E,n,k$ such that $U(T^k(A_n(\alpha,E)))\leq C$
