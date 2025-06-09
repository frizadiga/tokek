# Tokek: Token LLM Generation Simulator

Tokek is a simple token generation simulator built in C. This program simulates the generation of tokens by LLM over a defined period and displays a progress bar to show the progress of the generation process.

## Features

- **Cross-Platform**: Works on both Windows and UNIX-like systems.
- **Progress Bar**: Displays the current generation progress in a visually appealing manner.
- **Environment Variables Support**: Allows customization through environment variables for tokens per second and total tokens to generate.

## Getting Started

### Prerequisites

- C compiler (e.g., `clang` or `gcc`). clang is recommended for slightly better compilation performance.

### Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/frizadiga/tokek.git
   cd tokek
   ```

2. Compile the program:
   ```bash
   make
   ```

### Running the Simulator

You can run the simulator after compiling. Use the command:

```bash
./tokek
```

You'll be prompted to enter the tokens per second and the total tokens to generate. Alternatively, you can set the following environment variables before running the program:

- `TOKEK_TOKENS_PER_SECOND`: The number of tokens to generate per second.
- `TOKEK_TOKENS_TOTAL`: The total number of tokens to generate.

### Example Usage

```bash
export TOKEK_TOKENS_PER_SECOND=5
export TOKEK_TOKENS_TOTAL=50
./tokek
```

## Makefile

A `Makefile` is provided for easy compilation. 
- Run `make` to build the program.
- Use `make dev` to compile and run the program in development mode.

## Contributing

Contributions are welcome! If you have suggestions for improvements or new features, feel free to open an issue or submit a pull request.
