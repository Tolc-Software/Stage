# Stage #

Stage is a C++ helper library for creating temporary project environment to test how a library interacts in a "real" scenario.

It helps with an overview of operations:

1. You give `Stage` a path to a `base` (e.g. `tests/testStage`)
  * A `base` is a project that will be used as a template
  * The `base` should contain the files you want to use in *all* your stages
  * This creates a unique temporary directory as `tests/testStage/stages/xxxxxx`
2. You give `Stage` a set of files from the `base` (e.g. `{tests/testStage/CMakeLists.txt, tests/testStage/cmake/Module.cmake}`)
3. You give `Stage` a set of source files with content (this will be your test)
4. You let `Stage` build the project and you run some arbitrary commands inside the stage
5. The `Stage` object goes out of scope and the temporary directory is destroyed

## Installation ##

TODO

## Usage ##

TODO

## License ##
[MIT](https://choosealicense.com/licenses/mit/)
