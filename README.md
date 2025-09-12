# arduino_first_tests

## Quick Commands Reference

### Build and Flash a Program

Build a program:
```
make PROGRAM=hello_world
```

Flash a program:

```
make PROGRAM=hello_world flash
```

Clean build files for a program:

```
make PROGRAM=hello_world clean
```

### View Serial Output

```
screen /dev/ttyACM0 9600
```

### Kill Screen Session

Press `Ctrl+A` then `k`
