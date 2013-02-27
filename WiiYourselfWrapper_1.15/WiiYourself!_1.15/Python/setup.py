#!/usr/bin/env python

from distutils.core import setup, Extension

exts = Extension('_wiimote', sources=['wiimote_wrap.cxx', '../wiimote.cpp'])

setup (name = 'wiimote',
       version = '1.15RC2',
       author      = "gl.tter, nneonneo",
       description = """WiiYourself! python bindings""",
       ext_modules = [exts],
       py_modules = ["wiimote"],
       )
