import os
import sys
sys.path.insert(0, os.path.abspath("../../../../GENETIS_HPol/Evolutionary_loop/Loop_Parts/Part_B"))
# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'GENETIS_docs'
copyright = '2023, OSU GENETIS Team'
author = 'OSU GENETIS Team'
release = '0.1'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [
    'sphinxnotes.strike',
    'sphinx.ext.todo',
    'sphinx_panels',
    'sphinx.ext.autodoc'
]

todo_include_todos = True

templates_path = ['_templates']
exclude_patterns = []



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output
html_theme = 'furo'
html_title = 'GENETIS documentation'
html_static_path = ['_static']
