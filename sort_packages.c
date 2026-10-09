/* Logistics package sorting: Merge Sort vs Quick Sort (unstable and stable) */
#include <stdio.h>
#include <string.h>

#define MAX 100

typedef struct { int id; int weight; } Package;

static long cmp_count;
static int step;

static void print_arr(const char *label, Package a[], int lo, int hi) {
    printf("    %s[", label);
    for (int i = lo; i <= hi; i++) printf(" P%d(%d)", a[i].id, a[i].weight);
    printf(" ]\n");
}

/* ---------------- Merge Sort (stable: take left on ties) ---------------- */
static void merge(Package a[], int l, int m, int r) {
    Package tmp[MAX];
    int i = l, j = m + 1, k = 0;
    while (i <= m && j <= r) {
        cmp_count++;
        if (a[i].weight <= a[j].weight) tmp[k++] = a[i++];   /* <= keeps order */
        else tmp[k++] = a[j++];
    }
    while (i <= m) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (k = 0, i = l; i <= r; i++, k++) a[i] = tmp[k];
    printf("  Step %2d MERGE [%d..%d]: ", ++step, l, r);
    print_arr("", a, l, r);
}
static void merge_sort(Package a[], int l, int r) {
    if (l >= r) return;
    int m = (l + r) / 2;
    merge_sort(a, l, m);
    merge_sort(a, m + 1, r);
    merge(a, l, m, r);
}

/* ---------------- Quick Sort ---------------- */
/* stable_mode=0 : compare weight only (Lomuto, last element pivot) -> unstable
   stable_mode=1 : compare (weight, id); id = original position -> stable result */
static int stable_mode;

static int less(Package x, Package y) {          /* x < y ? counts a comparison */
    cmp_count++;
    if (x.weight != y.weight) return x.weight < y.weight;
    return stable_mode ? x.id < y.id : 0;
}
static int partition(Package a[], int lo, int hi) {
    Package pivot = a[hi];
    int i = lo - 1;
    for (int j = lo; j < hi; j++) {
        if (less(a[j], pivot)) {
            i++;
            Package t = a[i]; a[i] = a[j]; a[j] = t;
        }
    }
    Package t = a[i + 1]; a[i + 1] = a[hi]; a[hi] = t;
    printf("  Step %2d PARTITION [%d..%d] pivot=P%d(%d) -> pos %d : ",
           ++step, lo, hi, pivot.id, pivot.weight, i + 1);
    print_arr("", a, lo, hi);
    return i + 1;
}
static void quick_sort(Package a[], int lo, int hi) {
    if (lo < hi) {
        int p = partition(a, lo, hi);
        quick_sort(a, lo, p - 1);
        quick_sort(a, p + 1, hi);
    }
}

/* ---------------- Stability check using package IDs ---------------- */
static int verify_stable(Package a[], int n) {
    int ok = 1;
    for (int i = 0; i + 1 < n; i++)
        if (a[i].weight == a[i + 1].weight && a[i].id > a[i + 1].id) {
            printf("    ORDER VIOLATION: P%d appears before P%d (weight %d)\n",
                   a[i].id, a[i + 1].id, a[i].weight);
            ok = 0;
        }
    return ok;
}

static void run(const char *name, Package orig[], int n, int algo) {
    Package a[MAX];
    memcpy(a, orig, sizeof(Package) * n);
    cmp_count = 0; step = 0;
    printf("\n=== %s ===\n", name);
    print_arr("Initial: ", a, 0, n - 1);
    if (algo == 0) merge_sort(a, 0, n - 1);
    else { stable_mode = (algo == 2); quick_sort(a, 0, n - 1); }
    print_arr("Sorted : ", a, 0, n - 1);
    printf("    Comparisons = %ld\n", cmp_count);
    printf("    Stability check (equal weights keep ID order): %s\n",
           verify_stable(a, n) ? "PASS (stable)" : "FAIL (not stable)");
}

int main(void) {
    Package p[MAX]; int n;
    FILE *f = fopen("input.txt", "r");
    if (!f || fscanf(f, "%d", &n) != 1) { printf("Cannot read input.txt\n"); return 1; }
    for (int i = 0; i < n; i++) fscanf(f, "%d %d", &p[i].id, &p[i].weight);
    fclose(f);

    run("(a) MERGE SORT", p, n, 0);
    run("(a) QUICK SORT (standard, weight only)", p, n, 1);
    run("(b) QUICK SORT (modified: tie-break on package ID)", p, n, 2);
    return 0;
}
