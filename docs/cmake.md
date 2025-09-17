# Configure and build the project using cmake

Quick start with the `clang_debug` preset:

```shell
rm -rf build/clang_debug/*
cmake --preset=clang_debug
cmake --build --preset=clang_debug
```

## List available presets

```shell
cmake --list-presets
```

List build presets:

```shell
cmake --build --list-presets
```

## Configure a preset

```shell
cmake --preset=<preset-name>
```

## Build a preset

```shell
cmake --build --preset=<preset-name>
```

## Clean build directory

```shell
cmake --build --preset=<preset-name> --target clean
```

Or

```shell
rm -rf build/clang_debug/*
cmake --preset=clang_debug
cmake --build --preset=clang_debug
```

## Run tests

```shell
ctest --preset=<test-preset-name>
```
