# Visualization Robotics Programming Platform 🤖

Yo, welcome to the **Visualization Robotics Programming Platform**! This dope project is all about making robotics programming super easy and intuitive, even if you’re just getting started. It’s a slick platform that ties together a bunch of subsystems to help you code, visualize, and control robots like a pro. Whether you’re a dev ready to build some next-level robot magic or just curious about what’s up, this README’s got all the details to get you rolling.

## What’s This All About? 🤔

The Visualization Robotics Programming Platform is a modular beast that brings together multiple subsystems to handle everything from data processing to user interfaces and background services. Think of it like a Swiss Army knife for robotics programming. Each subsystem does its own thing but works together like a squad to get the job done.

### Subsystems Breakdown 🛠️

Here’s the scoop on the three main subsystems:

- **Editor**: The heart of the platform, where you can visualize your code in real-time, making programming robots as smooth as a breeze.
- **Application Runtime**: A slick, user-friendly interface built with [React/Vue/whatever you’re using] to make interaction smooth as butter.
- **MCU Runtime**: The glue that connects everything, serving up data and handling requests like a pro.

Each subsystem’s got its own folder in the repo, so it’s easy to find what you’re looking for.

## Getting Started: Let’s Roll! 🎉

Wanna dive in? Here’s how to get this bad boy up and running on your machine.

### Prerequisites

Before you start, make sure you’ve got these tools ready to go:

- **Python** (3.11+) – because who doesn’t love Python?
- **CMake** (3.20+) – to build the C++ parts.
- **Git** – to clone this repo and keep it real.
- **Visual Studio 2022** – for all your C++ needs (make sure to include the Desktop development with C++ workload).
- **MinGW** - install via `MSYS2` for a solid GCC toolchain.

### Installation

1. **Clone the repo**

   ```bash
   git clone git@github.com:threezinedine/robotics-programming-platform.git
   cd robotics-programming-platform
   ```

2. **Run**
   - For the editor:

     ```bash
     python config.py -p libraries build
     python config.py -p editor run
     ```

   - For the graphical application:

     ```bash
     python config.py -p gruntime build
     python config.py -p gruntime run
     ```

   - For the MCU runtime:

     ```bash
     # currently not available
     ```

3. **Run tests** (optional but recommended):

   ```bash
   python config.py -p libraries build # if not built yet
   python config.py -p libraries test # run all library tests
   ```

## Project Structure 📂

Here’s how the repo’s laid out, so you don’t get lost:

```bash
robotics-programming-platform/
├── autogen/             # Create bindings code from the C++ core
├── cmake/               # CMake scripts and modules
├── config/              # Tools for `config.py` which is used for managing the project
├── editor/              # The main editor application
├── gruntime/            # Graphical application runtime
├── libraries/           # Core C++ libraries and modules
├── runtime/             # MCU runtime (not available yet)
├── config.py            # Main script to build/run/test the project
├── dependencies.json    # List of dependencies for the project
└── README.md            # You’re reading it, fam!
```

Each folder has its own `README.md` with specific setup and usage instructions, so check those out if you’re diving deep into one subsystem.

## Contributing: Join the Squad! 🙌

Wanna help out? That’s what’s up! Here’s how to contribute:

1. Fork the repo and create a new branch:

   ```bash
   git checkout -b feature/your-cool-idea
   ```

2. Make your changes and commit with clear messages:

   ```bash
   git commit -m "Added some dope feature to the backend"
   ```

3. Push it up and open a pull request:

   ```bash
   git push origin feature/your-cool-idea
   ```

4. Wait for the crew to review it. We’ll give you feedback ASAP.

Check out `CONTRIBUTING.md` for more details on coding standards and all that jazz.

## Troubleshooting: Got Issues? 🤕

- If you stuck at generating the bindings, make sure you've setup the `clang` toolchain properly.

## License 📜

This project’s licensed under the MIT License. Do your thing, but keep it legal. See `LICENSE` for details.

## Shoutouts 🙏

Big thanks to the crew who’s been grinding on this project and to you for checking it out. Let’s keep building something dope together!
