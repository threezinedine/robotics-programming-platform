# Autogen
Autogen is a powerful tool designed to extract and transform C/C++ source code into various formats using Jinja2 templates. Its primary purpose is to facilitate binding C/C++ libraries for use as backends in other programming languages, enabling seamless integration with frontends while maintaining control over code visibility through annotations.

## Overview
Autogen simplifies the process of binding C/C++ libraries to serve as backends for other programming languages, such as Python. By leveraging `Jinja2` templates, it transforms C/C++ source code into customized formats suitable for integration. A key feature is its annotation system, which allows developers to hide specific code sections from the frontend, ensuring clean and secure interfaces for cross-language development.

## Features

- **Code Extraction**: Efficiently parses and extracts C/C++ source code for processing.
- **Jinja2 Templating**: Utilizes Jinja2 templates to generate flexible, user-defined output formats.
- **Automation**: Streamlines repetitive tasks, such as code transformation and binding generation.
- **Annotation Support**: Enables developers to hide designated code sections from frontend exposure, enhancing security and clarity.
[Your Additional Feature Here]: (Please specify any other key features of Autogen.)

## Usage
Autogen can be run manually or automatically as part of a C/C++ project compilation process, with caching support for efficiency. The primary interface is through a config.py script located in the root directory. Below are example commands:

```shell
python config.py -p autogen                # Run Autogen manually
python config.py -p autogen test           # Execute end-to-end tests
python config.py -p autogen test --filter "k=ParserTest"  # Run tests with a specific filter
```

## Todo

- [ ] Create binding for generating python dll.