# estrtoul

`estrtoul` is a more flexible strtoul. If the input string is `strtoul`-parseable, that result will be returned. Otherwise, if the input is of the form `"1 << 8"`, etc., then that is evaluated by `estrtoul`.

`main` demonstrates the effectiveness of the function.

Strings are evaluated left-to-right the same as `1 << 8 >> 1` would be by iterating through arrays of operators (`<<` or `>>`) and values (`1 8 1`). So if you do something somewhat nonsensical (from C's perspective), like `"1 << >> 8 1"`, it will be interpreted as `"1 << 8 >> 1"`.

Despite this, `estrtoul` will fail and `return 0` if the string does not start with a `strtoul`-parseable value. Excess operators and values are ignored, starting from the back (i.e. `"1 << 8 >> 1 1"` and `"1 << >> << 8 1"` both evaluate to `"1 << 8 >> 1"`).

If you are using this while `strtok`-ing elsewhere, you will need to convert everything to `strtok_r`, or else your position in your string will be ruined.

### Yes, it can handle string literals!
