# `donk/core`

`donk/core` is the base implementation of the Donk object tree. It is currently
*API-unstable*. It includes the base types used to represent objects and procs
in the BYOND runtime. Notably:

- `donk::iota_t` is the base type for all object representations.
- `donk::var_t` is the container type for representing variables.
- `donk::proc_table_t` stores references to procs, which are all free C++ functions
  with the signature `void name(donk::proc_ctxt_t&, donk::proc_args_t&)`.

Additionally, the interface for the interpreter is defined in this library.

The source code includes further documentation.

## License

Donk Project is Copyright 2021 (c) Warriorstar Orion and released under the
MIT license.