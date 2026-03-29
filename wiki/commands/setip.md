# setip -- Update Client IP Address

Update the client's IP address in the Director's in-memory configuration to the address of the currently connected console.

## Syntax

```
setip
```

## Arguments

None.

## Examples

### Update client IP
```
* setip
Client address updated
```

### Typical use for roaming clients
```
* whoami
*UserAgent*
* setip
Client address updated
```

### Verify with show
```
* setip
Client address updated
* show clients
Client: name=bareos-fd address=192.168.1.100 port=9102 enabled=yes
```

## Behavior

- Updates the client address in the Director's configuration to the IP address of the current console connection.
- Designed for roaming clients (e.g. laptops) that do not have a fixed IP address.
- The client must connect to the Director first (client-initiated connection), then the Director records the source IP.
- The change is in-memory only and does not persist across Director restarts.
- This is not an audited command.

## Related Commands

- [show](show.md) -- Show client configuration to verify the address
- [status](status.md) -- Check client connectivity
