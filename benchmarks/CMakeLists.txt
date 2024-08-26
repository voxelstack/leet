include(Benchmarks)

leet_benchmark(ds/bstree.c)

leet_chart(
    SOURCE ds/bstree.c
    NAME bstree_insert_fnptr
    RUNS fnptr nofnptr
)
