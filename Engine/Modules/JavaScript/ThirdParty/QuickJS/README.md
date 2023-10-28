# QuickJS
This version of QuickJS includes some patches to make it a bit more usable for our needs.

- Added `JS_CopyStringToNewBuffer`
- Exposed previously hidden `JS_VALUE_GET_STRING`
- Removed all dumping code

On another note: let this code be a lesson as to why C is awful. Hopefully I can replace this with something meaningfully better eventually.
