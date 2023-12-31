#define PY_SSIZE_T_CLEAN
#include <Python.h>


static PyObject *foreign_matrix_power(PyObject *self, PyObject *args) {
    PyObject *obj_list;
    int list_length;
    int degree;

    if (!PyArg_ParseTuple(args, "Oi", &obj_list, &degree))
        return NULL;

    list_length = PyObject_Length(obj_list);

    double copy[list_length][list_length];
    double mas[list_length][list_length];
    for (int i = 0; i < list_length; i++) {
        PyObject *lst = PyList_GetItem(obj_list, i);
        for (int j = 0; j < list_length; j++) {
            PyObject *item = PyList_GetItem(lst, j);
            double el = PyFloat_AsDouble(item);
            copy[i][j] = el;
            mas[i][j] = el;
        }
    }

    for (int c = 0; c < degree - 1; c++) {
        for (int i = 0; i < list_length; i++) { /* stroki */
            double str[list_length];
            for (int j = 0; j < list_length; j++) { /* stolbtci */
                double res = 0;
                for (int k = 0; k < list_length; k++) /* elementi */
                    res += mas[i][k] * copy[k][j];
                str[j] = res;
            }
            for (int j = 0; j < list_length; j++) /* stolbtci */
                mas[i][j] = str[j];
        }

        for (int i = 0; i < list_length; i++) {
            PyObject *lst = PyList_GetItem(obj_list, i);
            for (int j = 0; j < list_length; j++) {
                PyList_SetItem(lst, j, PyFloat_FromDouble(mas[i][j]));
            }
        }
    }
    return obj_list;
}

static PyMethodDef ForeignMethods[] = {
    {"foreign_matrix_power",
     foreign_matrix_power, METH_VARARGS,
     "Function for raising a square matrix to a power from C code."
    },
    {NULL, NULL, 0, NULL}   /* Sentinel */
};

static struct PyModuleDef foreignmodule = {
    PyModuleDef_HEAD_INIT,
    "foreign",      /* name of module */
    NULL,           /* module documentation, may be NULL */
    -1,             /* size of per-interpreter state of the module,
                       or -1 if the module keeps state in global variables. */
    ForeignMethods
};

PyMODINIT_FUNC PyInit_foreign(void) {
    return PyModule_Create(&foreignmodule);
}

