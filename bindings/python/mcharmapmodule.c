#include <Python.h>

#include <config.h>

#include <mcharmap/mcharmap.h>

/* include this first, before NO_IMPORT_PYGOBJECT is defined */
#include <pygobject.h>

#include <pygtk/pygtk.h>

void pymcharmap_register_classes(PyObject *d);
void pymcharmap_add_constants(PyObject *module, const gchar *strip_prefix);

extern PyMethodDef pymcharmap_functions[];

DL_EXPORT(void) initmcharmap (void);

DL_EXPORT(void)
initmcharmap (void)
{
	PyObject *m, *d;

	init_pygobject();
	init_pygtk ();

	m = Py_InitModule ("mcharmap", pymcharmap_functions);
	d = PyModule_GetDict (m);

	pymcharmap_register_classes (d);
        pymcharmap_add_constants(m, "MCHARMAP_");
}
