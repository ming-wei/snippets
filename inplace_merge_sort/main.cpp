#include <cstddef>
#include <utility>
#include <algorithm>
#include <iostream>
#include <vector>

using dtype = int;
using std::size_t;

// merge two ordered arrays
// elements are swapped so the set of elements among {a, b, c} does not change.
void merge(dtype *a, size_t m, dtype *b, size_t n, dtype *c)
{
    size_t i = 0, j = 0, k = 0;
    while (i < m || j < n) {
        if (j == n || (i < m && a[i] < b[j])) {
            std::swap(a[i++], c[k++]);
        } else {
            std::swap(b[j++], c[k++]);
        }
    }
}

// running an ordinary merge sort
void merge_sort_by_workspace(dtype *arr, dtype *workspace, size_t n)
{
    if (n == 1) return;
    size_t m = n / 2;
    merge_sort_by_workspace(arr, workspace, m);
    merge_sort_by_workspace(arr + m, workspace, n - m);
    merge(arr, m, arr + m, n - m, workspace);
    for (size_t i = 0; i < n; ++i) std::swap(arr[i], workspace[i]);
}

void inplace_merge_sort(dtype *arr, size_t n)
{
    if (n == 1) return;
    // half size (ceil)
    size_t m = (n+1) / 2;
    merge_sort_by_workspace(arr + m, arr, n - m);

    // arr[m..n) is sorted now
    while (m > 1) {
        size_t q = m / 2;
        // arr[0..q) is sorted now
        merge_sort_by_workspace(arr, arr + q, q);
        merge(arr, q, arr + m, n - m, arr + (m-q));
        // arr[(m-q)..n) is sorted now
        m -= q;
    }
    {
        size_t i = 0;
        while (i+1 < n && arr[i] > arr[i+1]) {
            std::swap(arr[i], arr[i+1]);
            ++i;
        }
    }
}

int main()
{
    for (size_t n = 1; n <= 100; ++n) {
        std::vector<dtype> v(n);
        for (size_t i = 0; i < n; ++i) v[i] = i;
        std::random_shuffle(v.begin(), v.end());
        inplace_merge_sort(v.data(), n);
        for (size_t i = 0; i < n; ++i) {
            if (v[i] != static_cast<int>(i)) {
                std::cout << "v[" << i << "]=" << v[i] << std::endl;
                __builtin_trap();
            }
        }
    }
}
