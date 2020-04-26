#include <iostream>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

void printPyErrorIfExists()
{
    if (PyErr_Occurred())
    {
        PyErr_Print();
    }
}

int main()
{
    const char* moduleName = "MyPython";
    const char* functionName = "add";
    const char* importPath = "E:/Videos/C++ Tutorial/Episode Bonus 023 - Python von C++ aus aufrufen/PythonVonCpp/PythonVonCpp";

    Py_Initialize();

    PyObject* pathList = PySys_GetObject("path");
    if (!pathList)
    {
        std::cerr << "Could not get path variable" << std::endl;
        return 1;
    }

    PyObject* pathName = PyUnicode_DecodeFSDefault(importPath);
    if (!pathName)
    {
        std::cerr << "Could not decode path" << std::endl;
        return 1;
    }

    int appendError = PyList_Append(pathList, pathName);
    Py_DECREF(pathName);
    if (appendError)
    {
        std::cerr << "Could not append to list!" << std::endl;
        return 1;
    }

    PyObject* pyModuleName = PyUnicode_DecodeFSDefault(moduleName);
    if (!pyModuleName)
    {
        std::cerr << "Could not decode moduleName!" << std::endl;
        return 1;
    }

    PyObject* pyModule = PyImport_Import(pyModuleName);
    Py_DECREF(pyModuleName);
    if (!pyModule)
    {
        std::cerr << "Could not import module" << std::endl;
        printPyErrorIfExists();
        return 1;
    }

    PyObject* pyFunction = PyObject_GetAttrString(pyModule, functionName);
    Py_DECREF(pyModule);
    if (!pyFunction)
    {
        std::cerr << "Could not find function name!" << std::endl;
        return 1;
    }
    if (!PyCallable_Check(pyFunction))
    {
        std::cerr << "Function is not callable!" << std::endl;
        Py_DECREF(pyFunction);
        return 1;
    }

    PyObject* pyFunctionArgs = PyTuple_New(2);
    PyTuple_SetItem(pyFunctionArgs, 0, PyLong_FromLong(17));
    PyTuple_SetItem(pyFunctionArgs, 1, PyLong_FromLong(19));
    PyObject* returnValue = PyObject_CallObject(pyFunction, pyFunctionArgs);
    Py_DECREF(pyFunction);
    Py_DECREF(pyFunctionArgs);

    if (!returnValue)
    {
        std::cerr << "Call to function failed" << std::endl;
        printPyErrorIfExists();
        return 1;
    }

    std::cout << "The result: " << PyLong_AsLong(returnValue);
    Py_DECREF(returnValue);

    if (Py_FinalizeEx() == -1)
    {
        std::cerr << "Something went wrong while finalize" << std::endl;
        return 1;
    }

    return 0;
}
