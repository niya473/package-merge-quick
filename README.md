# Logistics Package Sorting: Merge Sort vs Quick Sort (C)

## Problem
Packages (ID, weight): P1(20) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P8(10).
Sort by weight using Merge Sort and Quick Sort; keep equal-weight packages in original order; analyse and choose the better algorithm.

## Repository contents
| File | Purpose |
|---|---|
| `sort_packages.c` | Source code (Merge Sort, Quick Sort, stable Quick Sort, stability verifier) |
| `input.txt` | Input data (n, then `id weight` pairs) |
| `output.txt` | Program output (traces, comparison counts, verification) |
| `README.md` | Trace tables, complexity analysis, comparison, conclusion |

Run: `gcc sort_packages.c -o sort_packages && ./sort_packages`

## Trace tables

### Merge Sort (bottom-up view of the recursion, ties take the LEFT element)
| Step | Merge range | Result |
|---|---|---|
| Initial | - | P1(20) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P8(10) |
| 1 | [0..1] | P2(15) P1(20) |
| 2 | [2..3] | P4(10) P3(20) |
| 3 | [0..3] | P4(10) P2(15) P1(20) P3(20) |
| 4 | [4..5] | P5(15) P6(20) |
| 5 | [6..7] | P8(10) P7(25) |
| 6 | [4..7] | P8(10) P5(15) P6(20) P7(25) |
| 7 | [0..7] | P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25) |

Comparisons = **16**. Result is stable.

### Quick Sort, standard (last element pivot, compares weight only)
| Step | Range | Pivot | Pivot final index | Array after partition |
|---|---|---|---|---|
| 1 | [0..7] | P8(10) | 0 | P8(10) P2(15) P3(20) P4(10) P5(15) P6(20) P7(25) P1(20) |
| 2 | [1..7] | P1(20) | 4 | P2(15) P4(10) P5(15) P1(20) P6(20) P7(25) P3(20) |
| 3 | [1..3] | P5(15) | 2 | P4(10) P5(15) P2(15) |
| 4 | [5..7] | P3(20) | 5 | P3(20) P7(25) P6(20) |
| 5 | [6..7] | P6(20) | 6 | P6(20) P7(25) |

Final: P8(10) P4(10) P5(15) P2(15) P1(20) P3(20) P6(20) P7(25). Comparisons = **18**.
**Not stable**: P8 is before P4, and P5 is before P2 (the original order was P4 before P8, P2 before P5).

### (b) Modified Quick Sort (compare weight, then package ID = original position)
| Step | Range | Pivot | Pivot final index | Array after partition |
|---|---|---|---|---|
| 1 | [0..7] | P8(10) | 1 | P4(10) P8(10) P3(20) P1(20) P5(15) P6(20) P7(25) P2(15) |
| 2 | [2..7] | P2(15) | 2 | P2(15) P1(20) P5(15) P6(20) P7(25) P3(20) |
| 3 | [3..7] | P3(20) | 5 | P1(20) P5(15) P3(20) P7(25) P6(20) |
| 4 | [3..4] | P5(15) | 3 | P5(15) P1(20) |
| 5 | [6..7] | P6(20) | 6 | P6(20) P7(25) |

Final: P4(10) P8(10) P2(15) P5(15) P1(20) P3(20) P6(20) P7(25). Comparisons = **18**.

### Verification using package IDs
| Weight | Original ID order | Merge Sort | Standard Quick | Modified Quick |
|---|---|---|---|---|
| 10 | P4, P8 | P4, P8 ✔ | P8, P4 ✘ | P4, P8 ✔ |
| 15 | P2, P5 | P2, P5 ✔ | P5, P2 ✘ | P2, P5 ✔ |
| 20 | P1, P3, P6 | P1, P3, P6 ✔ | P1, P3, P6 ✔ | P1, P3, P6 ✔ |
| 25 | P7 | P7 ✔ | P7 ✔ | P7 ✔ |

## Analysis (c)

**Duplicate values.** Weights 10, 15 and 20 repeat (20 occurs three times). Merge Sort handles duplicates naturally. Quick Sort with a 2-way partition handles many duplicates poorly: elements equal to the pivot all fall on one side, which can cause unbalanced splits (worst case O(n²) if all values are equal). A 3-way partition fixes this.

**Stability.** Merge Sort is stable because on a tie (`<=`) it copies from the left sub-array first. Standard Quick Sort is not stable because partition swaps move elements across long distances (e.g. P4 jumped behind P8). Making it stable here required adding the package ID (original position) as a tie-breaker, which makes every key unique. An alternative is a stable partition using an extra array, which costs O(n) space.

**Number of comparisons (n = 8).** Merge Sort 16, Quick Sort 18. Merge Sort always makes between (n/2)log₂n and n log₂n − n + 1 comparisons, whereas Quick Sort's count depends on pivot quality.

| Criterion | Merge Sort | Quick Sort (standard) | Quick Sort (stable via ID) |
|---|---|---|---|
| Best time | O(n log n) | O(n log n) | O(n log n) |
| Average time | O(n log n) | O(n log n) | O(n log n) |
| Worst time | O(n log n) | O(n²) | O(n²) |
| Space | O(n) auxiliary | O(log n) stack (O(n) worst) | O(log n) stack, but needs an ID field |
| Stable | Yes | No | Yes (with tie-break key) |
| Comparisons (this data) | 16 | 18 | 18 |
| Handles duplicates | Well | Poorly (2-way) | Well (keys unique) |

## Final conclusion
For this input, both algorithms sort the weights correctly, but standard Quick Sort breaks the original order of equal-weight packages (P8 before P4, P5 before P2). Merge Sort preserved the order with no change, made fewer comparisons (16 vs 18), and has a guaranteed O(n log n) worst case. Its cost is O(n) extra space, which is acceptable for a logistics system.

**Merge Sort is the more suitable algorithm when the original order of equal-weight packages must be maintained.** Quick Sort is preferable only when memory is tight and stability is not required; it can be made stable by tie-breaking on package ID, as shown in part (b).
