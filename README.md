# ccres

The blog comment filter.

# Build

Build system is using Makefile.

```
$ git clone https://github.com/narupo/ccres
$ cd ccres
$ make init && make
$ ./build/ccres -h
```

# Usage

`ccres` need text from standard input stream.
`ccres` parse that text and output by specify format.

```
$ curl blog-url | ccres --auto --json
```

# Comment format

Blog comment format is the following.

```
Number Name Date Youbi Time ID
Comment
```

If you not need `Youbi` then set option of `--off-youbi`.
Same for `Time`, `ID`, etc.
If you need detail then see output of `--help`.

# License

MIT
