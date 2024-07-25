#include "../tests.h"

#include <ds/mat.h>

int
main()
{
        start();

        test(make);
        test(set);
        test(at);

        end();
}

int
make()
{
        size_t el_size = sizeof(char);
        size_t ino = 4;
        size_t jno = 4;
        struct mat m = { 0 };

        mat_make(&m, el_size, ino, jno);

        should(eq(m._ino, ino), "row number was not initialized");
        should(eq(m._jno, jno), "column number was not initialized");
        should(eq(m._el_size, el_size), "el_size was not initialized");
        should(!eq(m._data, NULL), "data was null");

        mat_del(&m);
        return 0;
}

int
idxof()
{
        size_t el_size = 2;
        size_t ino = 4;
        size_t jno = 4;
        struct mat m = { 0 };

        mat_make(&m, el_size, ino, jno);

        should(eq(mat_idxof(&m, 0, 0), 0),
               "offset of the first element was not zero");
        should(eq(mat_idxof(&m, 0, 2), 2 * el_size),
               "offset of element at the first row was incorrect");
        should(eq(mat_idxof(&m, 2, 2), 2 * jno * el_size + 2 * el_size),
               "offset of element at the third row was incorrect");

        mat_del(&m);
        return 0;
}

int
set()
{
        size_t el_size = 3 * sizeof(char);
        size_t ino = 4;
        size_t jno = 4;
        struct mat m = { 0 };

        char* z0 = "00";
        char* t2 = "22";
        char* z1 = "01";
        char* t3 = "23";

        mat_make(&m, el_size, ino, jno);

        mat_set(&m, 0, 0, z0);
        mat_set(&m, 2, 2, t2);
        mat_set(&m, 0, 1, z1);
        mat_set(&m, 2, 3, t3);

        should(
            !strcmp(m._data + mat_idxof(&m, 0, 0), z0),
            "element inserted at 0,0 was not copied to correct destination");
        should(
            !strcmp(m._data + mat_idxof(&m, 2, 2), t2),
            "element inserted at 2,2 was not copied to correct destination");
        should(
            !strcmp(m._data + mat_idxof(&m, 0, 1), z1),
            "element inserted at 0,1 was not copied to correct destination");
        should(
            !strcmp(m._data + mat_idxof(&m, 2, 3), t3),
            "element inserted at 2,3 was not copied to correct destination");

        mat_del(&m);
        return 0;
}

int
at()
{
        size_t el_size = 3 * sizeof(char);
        size_t ino = 4;
        size_t jno = 4;
        struct mat m = { 0 };

        char* z0 = "00";
        char* t2 = "22";
        char* z1 = "01";
        char* t3 = "23";

        mat_make(&m, el_size, ino, jno);

        mat_set(&m, 0, 0, z0);
        mat_set(&m, 2, 2, t2);
        mat_set(&m, 0, 1, z1);
        mat_set(&m, 2, 3, t3);

        should(!strcmp(mat_at(&m, 0, 0), z0),
               "view into 0,0 did not contain the correct element");
        should(!strcmp(mat_at(&m, 2, 2), t2),
               "view into 2,2 did not contain the correct element");
        should(!strcmp(mat_at(&m, 0, 1), z1),
               "view into 0,1 did not contain the correct element");
        should(!strcmp(mat_at(&m, 2, 3), t3),
               "view into 2,3 did not contain the correct element");

        mat_del(&m);
        return 0;
}
