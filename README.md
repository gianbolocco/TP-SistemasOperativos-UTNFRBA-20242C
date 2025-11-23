# The Last of C - Operating Systems Project

This project was developed for the "Sistemas Operativos" (Operating Systems) course at UTN FRBA (National Technological University, Buenos Aires Regional Faculty) during the second semester of 2024.

## Project Objectives

The main goal of this project is to simulate a distributed system composed of several distinct modules that interact with each other to manage resources, execute instructions, and handle file operations. The system mimics the behavior of a real operating system, handling concurrency, memory management, and process scheduling.

## Modules

The system is divided into four main modules:

*   **Kernel**: The central component that manages processes (PCB), scheduling (Long Term, Short Term), and interfaces with the CPU, Memory, and Filesystem. It handles system calls and resource management.
*   **CPU**: Responsible for executing instructions received from the Kernel. It fetches instructions, decodes them, and executes them, interacting with Memory (via MMU) and the Kernel (for interrupts and system calls).
*   **Memoria (Memory)**: Manages the system's RAM. It handles user processes' instructions and data, serving requests from the CPU and Kernel. It implements paging/segmentation as required by the architecture.
*   **Filesystem**: Manages the storage of files. It handles file creation, reading, writing, and deletion, interacting with the Kernel to persist data.

## Prerequisites

To compile and run this project, you need the [so-commons-library](https://github.com/sisoputnfrba/so-commons-library) installed.

## Installation and Setup

We provide helper scripts to streamline the setup process.

### 1. Install Dependencies

Run the following script to clone and install the required commons library and set up necessary directories:

```bash
./1_install_libs.sh
```

### 2. Compilation

To compile all modules (Kernel, CPU, Memory, Filesystem), run:

```bash
./2_compile_all.sh
```

Alternatively, you can go into each module's directory and run `make`.

### 3. Configuration

You need to configure the IP addresses for the modules to communicate. Run the configuration script:

```bash
./3_config_ips.sh
```

This script will ask for the IP addresses of the machine(s) hosting each module and update the configuration files accordingly.

### 4. Running Tests

To set up the environment for specific test cases (e.g., Planning, Race Condition, Memory Management), use:

```bash
./4_set_test_env.sh
```

Select the test scenario you want to run from the menu. This will copy the appropriate configuration files to each module.

## Execution

After compilation and configuration, you can run each module from its respective directory. It is recommended to start them in the following order (or as specified by your specific architecture requirements, typically Memory and FS first):

```bash
# Terminal 1
cd memoria
./bin/memoria

# Terminal 2
cd filesystem
./bin/filesystem

# Terminal 3
cd cpu
./bin/cpu

# Terminal 4
cd kernel
./bin/kernel
```

## Deployment

For deploying to a remote Ubuntu Server, you can use the `so-deploy` tool. See the [official documentation](https://github.com/sisoputnfrba/so-deploy) for details.

## Useful Links

- [Compilation Errors Guide](https://docs.utnso.com.ar/primeros-pasos/primer-proyecto-c#errores-de-compilacion)
- [Debugging Guide](https://docs.utnso.com.ar/guias/herramientas/debugger)
- [VS Code Configuration](https://docs.utnso.com.ar/guias/herramientas/code)
- [Deployment Guide](https://docs.utnso.com.ar/guías/herramientas/deploy)
