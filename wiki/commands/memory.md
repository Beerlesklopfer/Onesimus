# memory -- Show Memory Usage Information

Display memory usage statistics of the Director daemon.

## Syntax

```
memory
```

## Arguments

None.

## Examples

### Show memory usage
```
* memory
Memory usage information not available (Rust runtime)
```

### In C++ Bareos
```
* memory
Pool Bytes     Max Bytes  Max Used  Get         Free
Main  1234567  2345678    1890000   45678       44000
```

### Check after extended operation
```
* memory
Memory usage information not available (Rust runtime)
```

## Behavior

- In C++ Bareos, displays the internal memory pool allocator statistics including current bytes, maximum bytes, and allocation/free counts.
- In the Rust implementation, Rust uses its own memory allocator (jemalloc or system malloc) which does not expose the same pool-based statistics, so this command reports that detailed info is not available.
- Useful for diagnosing memory leaks or capacity issues in long-running Director instances.
- This is not an audited command.

## Related Commands

- [status](status.md) -- Show Director status
- [version](version.md) -- Show Director version
