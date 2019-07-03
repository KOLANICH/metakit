// PyProperty.cpp --
// $Id: PyProperty.cpp 1230 2007-03-09 15:58:53Z jcw $
// This is part of MetaKit, the homepage is http://www.equi4.com/metakit.html
// Copyright (C) 1999-2004 Gordon McMillan and Jean-Claude Wippler.
//
//  Property class implementation

#include "PyProperty.h"
#include <PWONumber.h>
#include <PWOSequence.h>

static void PyProperty_dealloc(PyProperty *o) {
  delete o;
}

static PyObject *PyProperty_getattr(PyProperty *o, char *nm) {
  try {
    if (nm[0] == 'n' && strcmp(nm, "name") == 0) {
      PWOString rslt(o->Name());
      return rslt.disOwn();
    }
    if (nm[0] == 't' && strcmp(nm, "type") == 0) {
      char s = o->Type();
      PWOString rslt(&s, 1);
      return rslt.disOwn();
    }
    if (nm[0] == 'i' && strcmp(nm, "id") == 0) {
      PWONumber rslt(o->GetId());
      return rslt.disOwn();
    }
    return PyObject_GenericGetAttr(o, PyUnicode_FromString(nm));
  } catch (...) {
    return 0;
  }
}

static PyObject* PyProperty_richcompare(PyProperty *o, PyObject *ob, int op) {
  PyProperty *other;
  int myid, hisid;
  try {
    if (!PyProperty_Check(ob))
      return nullptr;
    other = (PyProperty*)ob;
    myid = o->GetId();
    hisid = other->GetId();
    bool res;
    switch(op){
        case Py_LT:
        res=myid < hisid;
        case Py_LE:
        res=myid <= hisid;
        case Py_EQ:
        res=myid == hisid;
        case Py_NE:
        res=myid != hisid;
        case Py_GT:
        res=myid > hisid;
        case Py_GE:
        res=myid >= hisid;
    }
    return res?Py_True:Py_False;
  } catch (...) {
    return nullptr;
  }
}

PyTypeObject PyPropertytype =  {
  .ob_base=PyVarObject_HEAD_INIT(&PyType_Type, 0)
  .tp_name="PyProperty",
  .tp_basicsize=sizeof(PyProperty),
  .tp_itemsize=0,
  .tp_dealloc=reinterpret_cast<destructor>(PyProperty_dealloc),
  .tp_getattr=reinterpret_cast<getattrfunc>(PyProperty_getattr),
  .tp_setattr=nullptr,
  .tp_as_async=nullptr,
  .tp_repr=nullptr,
  .tp_as_number=nullptr,
  .tp_as_sequence=nullptr,
  .tp_as_mapping=nullptr,
  .tp_hash=nullptr,
  .tp_call=nullptr,
  .tp_str=nullptr,
  .tp_getattro=nullptr,
  .tp_setattro=nullptr,
  .tp_as_buffer=nullptr,
  .tp_flags=0,
  .tp_doc=nullptr,
  .tp_traverse=nullptr,
  .tp_clear=nullptr,
  .tp_richcompare=reinterpret_cast<richcmpfunc>(PyProperty_richcompare),
};


PyObject *PyProperty_new(PyObject *o, PyObject *_args) {
  try {
    PWOSequence args(_args);
    PWOString typ(args[0]);
    PWOString nam(args[1]);
    return new PyProperty(*(const char*)typ, nam);
  } catch (...) {
    return 0;
  }
}
