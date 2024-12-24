<div align="center">
  <pre>
         _   _ _       
 ___ ___| |_|_| |_ ___ 
| . | -_|  _| |  _| . |
|  _|___|_| |_|_| |___|
|_|                    
  </pre>

</div>

## About this project

I made this project because curl is an important tool in every developer's workflow. I wanted to remake it to better understand how HTTP requests work and to challenge my C knowledge.

## Installation

```sh
git clone https://github.com/Rfellipe/petito.git
cd petito
make
```

### Add to Path

You can either:

1. Add the `petito` directory to your `$PATH`:
```sh
export PATH=$PATH:/path/to/petito
```
*(Add this line to your shell rc file)*

2. Copy the binary to a directory in your `$PATH`:
```sh
sudo cp petito /usr/local/bin
```

## Usage

### GET

```sh
petito https://catfact.ninja/fact
```

### POST

```sh
petito https://httpbin.org/anything \
-H 'Content-Type: application/json' \
-d '{"user":"teste", "pass":123}'
```

### Help message:

You can also type:
```sh
petito --help
```
to see more about the available commands.

## Contributing:

1. Fork this repo.
2. Create a new branch with the name of the feature you want to implement.
3. Commit your changes.
4. Push to the new branch.
5. Then create a pull request.

### Some TODO tasks:

1. Add support to more HTTP verbs, e.g: PUT, DELETE, etc.
2. Add support for file I/O.
3. Add support for proxies.
4. Improve the code documentation.
