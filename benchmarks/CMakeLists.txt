include(Benchmarks)

add_benchmark(ds/bstree.c)

add_chart(
    SOURCE ds/bstree.c
    NAME bstree_insert_fnptr
    RUNS fnptr nofnptr
)
