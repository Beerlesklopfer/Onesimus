# version — Show Director Version

Display the Director daemon version, build info, and operating system.

## Syntax

```
version
```

## Example

```
* version
bareos-dir Version: 0.25.0 (Debian GNU/Linux 13 (trixie))
```

## Notes

The version string format is `bareos-dir Version: <version> (<os>)` for protocol compatibility with C++ bconsole and Bareos WebUI. The actual daemon binary is `onesimusd`.
