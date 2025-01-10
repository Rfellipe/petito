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

I made this project because curl is an important tool in every developer's workflow. So I wanted to remake it to better understand how HTTP requests work and to challenge my C knowledge.

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

Every argument that doesn't have flag will be treated as an URL.

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

### PUT

```sh
petito -x put https://httpbin.org/anything \
-H 'Content-Type: application/json' \
-d '{"user":"teste", "pass":123}'
```

### Help message:
```
-d, --data=STRING          HTTP POST data
-f, --file=FILE            Make request based on file
-H, --header=STRING        Add header to request
-o, --output=FILE          Write output to a file insted of stdo
-t, --template=FILE        Generate template file to make requests
-v, --verbose              Show more information on output
-x, --method=STRING        Specify request method
-?, --help                 Give this help list
--usage                    Give a short usage message
-V, --version              Print program version
```

You can also type:
```sh
petito --help or petito --usage
```
to see this 

## Contributing:

1. Fork this repo.
2. Create a new branch with the name of the feature you want to implement.
3. Commit your changes.
4. Push to the new branch.
5. Then create a pull request.

### Some TODO tasks:

1. Add support for file I/O.
2. Add support for proxies.
3. Improve the code documentation.
